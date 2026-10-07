#include "TabModel.hpp"
#include "ProfileManager.hpp"

#include <QUuid>

TabModel::TabModel(QObject *parent) : QAbstractListModel(parent) {}

int TabModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid()) {
    return 0;
  }
  return m_tabs.size();
}

QVariant TabModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() < 0 || index.row() >= m_tabs.size()) {
    return {};
  }

  const auto &tab = m_tabs.at(index.row());
  switch (role) {
  case IdRole:
    return tab.id;
  case TitleRole:
    return tab.title;
  case UrlRole:
    return tab.url;
  case LoadingRole:
    return tab.loading;
  case ProgressRole:
    return tab.progress;
  case CanGoBackRole:
    return tab.canGoBack;
  case CanGoForwardRole:
    return tab.canGoForward;
  case IsPinnedRole:
    return tab.isPinned;
  default:
    return {};
  }
}

QHash<int, QByteArray> TabModel::roleNames() const {
  return {{IdRole, "id"},
          {TitleRole, "title"},
          {UrlRole, "url"},
          {LoadingRole, "loading"},
          {ProgressRole, "progress"},
          {CanGoBackRole, "canGoBack"},
          {CanGoForwardRole, "canGoForward"},
          {IsPinnedRole, "isPinned"}};
}

int TabModel::currentIndex() const { return m_currentIndex; }

void TabModel::setCurrentIndex(int index) {
  if (index >= 0 && index < m_tabs.size() && m_currentIndex != index) {
    m_currentIndex = index;
    persistCurrentTab();
    emit currentIndexChanged();
  }
}

int TabModel::count() const { return m_tabs.size(); }

QString TabModel::workspaceId() const { return m_workspaceId; }

void TabModel::setWorkspaceId(const QString &wsId) {
  if (m_workspaceId == wsId) {
    return;
  }

  m_workspaceId = wsId;
  loadTabs();
  emit workspaceIdChanged();
}

void TabModel::loadTabs() {
  beginResetModel();
  m_tabs.clear();
  m_currentIndex = 0;

  if (m_workspaceId.isEmpty()) {
    endResetModel();
    emit countChanged();
    return;
  }

  Database *db = ProfileManager::instance()->database();
  QVariantList rows = db->query(
      QStringLiteral(
          "SELECT id, url, title, is_pinned FROM tabs WHERE workspace_id = "
          ":ws_id ORDER BY is_pinned DESC, sort_order ASC;"),
      {{QStringLiteral(":ws_id"), m_workspaceId}});

  for (const auto &r : rows) {
    QVariantMap map = r.toMap();
    TabItem tab;
    tab.id = map.value(QStringLiteral("id")).toString();
    tab.url = QUrl(map.value(QStringLiteral("url")).toString());
    tab.title = map.value(QStringLiteral("title")).toString();
    tab.isPinned = map.value(QStringLiteral("is_pinned")).toInt() == 1;
    m_tabs.append(tab);
  }

  if (m_tabs.isEmpty()) {
    endResetModel();
    addTab(QUrl(QStringLiteral("about:blank")));
    return;
  }

  const QString savedId = ProfileManager::instance()->stateValue(
      QStringLiteral("active_tab:") + m_workspaceId);
  for (int i = 0; i < m_tabs.size(); ++i) {
    if (m_tabs.at(i).id == savedId) {
      m_currentIndex = i;
      break;
    }
  }
  endResetModel();
  emit countChanged();
  emit currentIndexChanged();
}

void TabModel::addTab(const QUrl &url) {
  QString id = QUuid::createUuid().toString(QUuid::WithoutBraces);
  int order = m_tabs.size();

  TabItem tab;
  tab.id = id;
  tab.url = url;
  if (url.toString() == QStringLiteral("ciel://history") ||
      url.toString() == QStringLiteral("about:history")) {
    tab.title = QStringLiteral("History");
  } else if (url.toString() == QStringLiteral("ciel://profiles")) {
    tab.title = QStringLiteral("Profiles");
  } else if (url.isEmpty() || url == QUrl(QStringLiteral("about:blank"))) {
    tab.title = QStringLiteral("New Tab");
  } else {
    tab.title = url.toString();
  }
  tab.isPinned = false;

  if (!m_workspaceId.isEmpty()) {
    Database *db = ProfileManager::instance()->database();
    db->execute(
        QStringLiteral(
            "INSERT INTO tabs (id, workspace_id, url, title, is_pinned, "
            "sort_order) "
            "VALUES (:id, :workspace_id, :url, :title, 0, :sort_order);"),
        {{QStringLiteral(":id"), id},
         {QStringLiteral(":workspace_id"), m_workspaceId},
         {QStringLiteral(":url"), tab.url.toString()},
         {QStringLiteral(":title"), tab.title},
         {QStringLiteral(":sort_order"), order}});
  }

  beginInsertRows(QModelIndex(), m_tabs.size(), m_tabs.size());
  m_tabs.append(tab);
  endInsertRows();

  emit countChanged();
  setCurrentIndex(m_tabs.size() - 1);
}

void TabModel::closeTab(int index) {
  if (m_tabs.size() <= 1 || index < 0 || index >= m_tabs.size()) {
    return;
  }

  QString id = m_tabs.at(index).id;
  if (!m_workspaceId.isEmpty()) {
    Database *db = ProfileManager::instance()->database();
    db->execute(QStringLiteral("DELETE FROM tabs WHERE id = :id;"),
                {{QStringLiteral(":id"), id}});
  }

  beginRemoveRows(QModelIndex(), index, index);
  m_tabs.removeAt(index);
  endRemoveRows();

  emit countChanged();

  persistOrder();

  if (m_currentIndex >= m_tabs.size()) {
    setCurrentIndex(m_tabs.size() - 1);
  } else if (m_currentIndex > index) {
    m_currentIndex--;
    emit currentIndexChanged();
  } else if (m_currentIndex == index) {
    emit currentIndexChanged();
  }
  persistCurrentTab();
}

void TabModel::updateTitle(int index, const QString &title) {
  if (index < 0 || index >= m_tabs.size()) {
    return;
  }
  if (m_tabs[index].title != title) {
    m_tabs[index].title = title;

    if (!m_workspaceId.isEmpty()) {
      Database *db = ProfileManager::instance()->database();
      db->execute(
          QStringLiteral("UPDATE tabs SET title = :title WHERE id = :id;"),
          {{QStringLiteral(":title"), title},
           {QStringLiteral(":id"), m_tabs[index].id}});
    }

    emit dataChanged(this->index(index), this->index(index),
                     QList<int>{TitleRole});
  }
}

void TabModel::updateUrl(int index, const QUrl &url) {
  if (index < 0 || index >= m_tabs.size()) {
    return;
  }
  if (m_tabs[index].url != url) {
    m_tabs[index].url = url;

    if (!m_workspaceId.isEmpty()) {
      Database *db = ProfileManager::instance()->database();
      db->execute(QStringLiteral("UPDATE tabs SET url = :url WHERE id = :id;"),
                  {{QStringLiteral(":url"), url.toString()},
                   {QStringLiteral(":id"), m_tabs[index].id}});

      ProfileManager::instance()->recordVisit(
          url.toString(), m_tabs[index].title, m_workspaceId);
    }

    emit dataChanged(this->index(index), this->index(index),
                     QList<int>{UrlRole});
  }
}

void TabModel::updateLoading(int index, bool loading) {
  if (index < 0 || index >= m_tabs.size()) {
    return;
  }
  if (m_tabs[index].loading != loading) {
    m_tabs[index].loading = loading;
    emit dataChanged(this->index(index), this->index(index),
                     QList<int>{LoadingRole});
  }
}

void TabModel::updateProgress(int index, int progress) {
  if (index < 0 || index >= m_tabs.size()) {
    return;
  }
  if (m_tabs[index].progress != progress) {
    m_tabs[index].progress = progress;
    emit dataChanged(this->index(index), this->index(index),
                     QList<int>{ProgressRole});
  }
}

void TabModel::updateNavigation(int index, bool canGoBack, bool canGoForward) {
  if (index < 0 || index >= m_tabs.size()) {
    return;
  }
  m_tabs[index].canGoBack = canGoBack;
  m_tabs[index].canGoForward = canGoForward;
  emit dataChanged(this->index(index), this->index(index),
                   QList<int>{CanGoBackRole, CanGoForwardRole});
}

void TabModel::moveTab(int from, int to) {
  if (from == to || from < 0 || to < 0 || from >= m_tabs.size() ||
      to >= m_tabs.size()) {
    return;
  }

  int destination = (to > from) ? to + 1 : to;
  beginMoveRows(QModelIndex(), from, from, QModelIndex(), destination);
  m_tabs.move(from, to);
  endMoveRows();

  persistOrder();

  if (m_currentIndex == from) {
    m_currentIndex = to;
    emit currentIndexChanged();
  } else if (from < m_currentIndex && to >= m_currentIndex) {
    m_currentIndex--;
    emit currentIndexChanged();
  } else if (from > m_currentIndex && to <= m_currentIndex) {
    m_currentIndex++;
    emit currentIndexChanged();
  }

  persistCurrentTab();
}

void TabModel::togglePin(int index) {
  if (index < 0 || index >= m_tabs.size()) {
    return;
  }
  setPinned(index, !m_tabs[index].isPinned);
}

void TabModel::setPinned(int index, bool pinned) {
  if (index < 0 || index >= m_tabs.size()) {
    return;
  }

  if (m_tabs[index].isPinned != pinned) {
    m_tabs[index].isPinned = pinned;

    int oldIndex = index;
    int targetIndex = index;

    if (pinned) {
      targetIndex = 0;
      for (int i = 0; i < m_tabs.size(); ++i) {
        if (m_tabs[i].isPinned && i != index) {
          targetIndex = i + 1;
        }
      }
    } else {
      targetIndex = m_tabs.size() - 1;
      for (int i = m_tabs.size() - 1; i >= 0; --i) {
        if (!m_tabs[i].isPinned && i != index) {
          targetIndex = i;
        }
      }
    }

    if (targetIndex != index && targetIndex >= 0 &&
        targetIndex < m_tabs.size()) {
      int dest = (targetIndex > index) ? targetIndex + 1 : targetIndex;
      beginMoveRows(QModelIndex(), index, index, QModelIndex(), dest);
      m_tabs.move(index, targetIndex);
      endMoveRows();
      index = targetIndex;

      if (m_currentIndex == oldIndex) {
        m_currentIndex = targetIndex;
        emit currentIndexChanged();
      } else if (oldIndex < m_currentIndex && targetIndex >= m_currentIndex) {
        m_currentIndex--;
        emit currentIndexChanged();
      } else if (oldIndex > m_currentIndex && targetIndex <= m_currentIndex) {
        m_currentIndex++;
        emit currentIndexChanged();
      }
    }

    if (!m_workspaceId.isEmpty()) {
      Database *db = ProfileManager::instance()->database();
      db->execute(
          QStringLiteral("UPDATE tabs SET is_pinned = :pinned WHERE id = :id;"),
          {{QStringLiteral(":pinned"), pinned ? 1 : 0},
           {QStringLiteral(":id"), m_tabs[index].id}});
    }

    persistOrder();
    emit dataChanged(this->index(index), this->index(index),
                     QList<int>{IsPinnedRole});
  }

  persistCurrentTab();
}

void TabModel::persistOrder() {
  if (m_workspaceId.isEmpty()) {
    return;
  }

  Database *db = ProfileManager::instance()->database();
  for (int i = 0; i < m_tabs.size(); ++i) {
    db->execute(
        QStringLiteral("UPDATE tabs SET sort_order = :order WHERE id = :id;"),
        {{QStringLiteral(":order"), i},
         {QStringLiteral(":id"), m_tabs.at(i).id}});
  }
}

void TabModel::persistCurrentTab() {
  if (m_workspaceId.isEmpty() || m_currentIndex < 0 ||
      m_currentIndex >= m_tabs.size())
    return;
  ProfileManager::instance()->setStateValue(
      QStringLiteral("active_tab:") + m_workspaceId,
      m_tabs.at(m_currentIndex).id);
}
