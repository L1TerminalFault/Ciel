#include "TabManager.hpp"
#include <QFileInfo>
#include <qhashfunctions.h>
#include <qqmlengine.h>
#include <quuid.h>

TabManager *TabManager::create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) {
  return new TabManager(qmlEngine);
}

TabManager::TabManager(QObject *parent) : QAbstractListModel(parent) {}

QHash<int, QByteArray> TabManager::roleNames() const {
  return {{IdRole, "id"},
          {TitleRole, "title"},
          {PathRole, "path"},
          {IconRole, "icon"}};
}

int TabManager::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;
  return m_tabs.size();
}

QVariant TabManager::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() < 0 || index.row() >= m_tabs.size())
    return QVariant();

  const TabItem &tab = m_tabs.at(index.row());

  switch (role) {
  case IdRole:
    return tab.id.toString();
  case TitleRole:
  case Qt::DisplayRole:
    return tab.title;
  case PathRole:
    return tab.path;
  case IconRole:
    return tab.icon;
  default:
    return QVariant();
  }
}

int TabManager::indexOf(const QUuid &id) const {
  for (int i = 0; i < m_tabs.size(); ++i) {
    if (m_tabs[i].id == id)
      return i;
  }
  return -1;
}

QString TabManager::currentTabId() const { return m_activeTabId.toString(); }

int TabManager::currentIndex() const { return indexOf(m_activeTabId); }

QString TabManager::currentPath() const {
  int idx = currentIndex();
  return (idx >= 0) ? m_tabs[idx].path : QString();
}

void TabManager::setCurrentTabId(const QString &uuid) {
  QUuid id(uuid);
  if (id == m_activeTabId)
    return;

  int idx = indexOf(id);
  if (idx < 0)
    return;

  m_activeTabId = id;

  m_history.removeAll(id);
  m_history.append(id);

  emit currentTabIdChanged();
  emit currentIndexChanged();
  emit currentPathChanged();
}

void TabManager::setCurrentPath(const QString &path) {
  int idx = currentIndex();
  if (idx < 0 || m_tabs[idx].path == path)
    return;

  m_tabs[idx].path = path;

  QString folderName = QFileInfo(path).fileName();
  m_tabs[idx].title = folderName.isEmpty() ? path : folderName;

  emit currentPathChanged();

  QModelIndex modelIdx = index(idx);
  emit dataChanged(modelIdx, modelIdx, {TitleRole, PathRole});
}

void TabManager::addTab(const QString &path) {
  QString folderName = QFileInfo(path).fileName();
  if (folderName.isEmpty())
    folderName = path.isEmpty() ? QStringLiteral("New Tab") : path;

  TabItem item;
  item.id = QUuid::createUuid();
  item.title = folderName;
  item.path = path;
  // left empty for now probably default to some unique options later on
  item.icon = "";

  m_activeTabId = item.id;
  m_history.removeAll(item.id);
  m_history.append(item.id);

  emit currentTabIdChanged();
  emit currentPathChanged();

  int newIndex = m_tabs.size();
  beginInsertRows(QModelIndex(), newIndex, newIndex);
  m_tabs.append(item);
  endInsertRows();

  emit currentIndexChanged();
}

void TabManager::closeTab(const QString &uuid) {
  QUuid closedTabId(uuid);
  int idx = indexOf(closedTabId);
  if (idx < 0)
    return;

  bool wasActive = (m_activeTabId == closedTabId);

  beginRemoveRows(QModelIndex(), idx, idx);
  m_tabs.removeAt(idx);
  endRemoveRows();

  m_history.removeAll(closedTabId);

  if (!wasActive) {
    emit currentIndexChanged();
    return;
  }

  QUuid nextId;
  while (!m_history.isEmpty()) {
    QUuid candidateId = m_history.last();
    if (indexOf(candidateId) >= 0) {
      nextId = candidateId;
      break;
    }
    m_history.removeLast();
  }

  if (nextId.isNull() && !m_tabs.isEmpty()) {
    int fallbackIndex = qBound(0, idx - 1, m_tabs.size() - 1);
    nextId = m_tabs[fallbackIndex].id;
  }

  m_activeTabId = nextId;

  emit currentTabIdChanged();
  emit currentIndexChanged();
  emit currentPathChanged();
}
