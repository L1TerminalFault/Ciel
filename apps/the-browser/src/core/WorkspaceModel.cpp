#include "WorkspaceModel.hpp"
#include "ProfileManager.hpp"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUuid>

#include <utility>

namespace {
QString activeWorkspaceKey() { return QStringLiteral("active_workspace_id"); }
QString activeTabKey(const QString &workspaceId) {
  return QStringLiteral("active_tab:") + workspaceId;
}
} // namespace

WorkspaceModel::WorkspaceModel(QObject *parent) : QAbstractListModel(parent) {
  auto *pm = ProfileManager::instance();
qInfo() << "[workspaces] ctor, connected to ProfileManager" << pm;

  connect(pm, &ProfileManager::activeProfileAboutToChange, this,
          &WorkspaceModel::unloadWorkspaces);

  connect(pm, &ProfileManager::activeProfileChanged, this,
          &WorkspaceModel::loadWorkspaces, Qt::QueuedConnection);

  loadWorkspaces();
}

int WorkspaceModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid()) {
    return 0;
  }
  return m_workspaces.size();
}

QVariant WorkspaceModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() < 0 ||
      index.row() >= m_workspaces.size()) {
    return {};
  }

  const auto &ws = m_workspaces.at(index.row());
  switch (role) {
  case IdRole:
    return ws.id;
  case NameRole:
    return ws.name;
  case ColorRole:
    return ws.color;
  case IconRole:
    return ws.icon;
  case PresetIdRole:
    return ws.presetId;
  default:
    return {};
  }
}

QHash<int, QByteArray> WorkspaceModel::roleNames() const {
  return {{IdRole, "id"},
          {NameRole, "name"},
          {ColorRole, "color"},
          {IconRole, "icon"},
          {PresetIdRole, "presetId"}};
}

int WorkspaceModel::count() const { return m_workspaces.size(); }

int WorkspaceModel::currentIndex() const { return m_currentIndex; }

void WorkspaceModel::setCurrentIndex(int index) {
  if (index >= 0 && index < m_workspaces.size() && m_currentIndex != index) {
    m_currentIndex = index;
    persistCurrentWorkspace();
    emit currentIndexChanged();
  }
}

QString WorkspaceModel::currentWorkspaceId() const {
  if (m_currentIndex >= 0 && m_currentIndex < m_workspaces.size()) {
    return m_workspaces.at(m_currentIndex).id;
  }
  return {};
}

TabModel *WorkspaceModel::tabModel(const QString &workspaceId) {
  if (workspaceId.isEmpty()) {
    return nullptr;
  }

  bool owned = false;
  for (const auto &ws : std::as_const(m_workspaces)) {
    if (ws.id == workspaceId) {
      owned = true;
      break;
    }
  }
  if (!owned) {
    return nullptr;
  }

  if (!m_tabModels.contains(workspaceId)) {
    auto *model = new TabModel(this);
    model->setWorkspaceId(workspaceId);
    m_tabModels.insert(workspaceId, model);
  }
  return m_tabModels.value(workspaceId);
}

void WorkspaceModel::releaseTabModels() {
  for (TabModel *model : std::as_const(m_tabModels)) {
    model->setWorkspaceId(QString());
    model->deleteLater();
  }
  m_tabModels.clear();
}

void WorkspaceModel::unloadWorkspaces() {
qInfo() << "[workspaces] unload (profile about to change)";
  releaseTabModels();

  beginResetModel();
  m_workspaces.clear();
  m_currentIndex = 0;
  endResetModel();

  emit countChanged();
  emit currentIndexChanged();
}

void WorkspaceModel::persistCurrentWorkspace() {
  const QString id = currentWorkspaceId();
  if (!id.isEmpty()) {
    ProfileManager::instance()->setStateValue(activeWorkspaceKey(), id);
  }
}

void WorkspaceModel::persistWorkspaceOrder() {
  Database *db = ProfileManager::instance()->database();
  for (int i = 0; i < m_workspaces.size(); ++i) {
    db->execute(QStringLiteral(
                    "UPDATE workspaces SET sort_order = :order WHERE id = :id;"),
                {{QStringLiteral(":order"), i},
                 {QStringLiteral(":id"), m_workspaces.at(i).id}});
  }
}

WorkspaceItem WorkspaceModel::insertWorkspaceRecord(const QString &name,
                                                    const QString &color,
                                                    const QString &icon,
                                                    const QString &presetId) {
  Database *db = ProfileManager::instance()->database();

  QVariantList orderRows = db->query(QStringLiteral(
      "SELECT COALESCE(MAX(sort_order), -1) + 1 AS next_order FROM "
      "workspaces;"));
  int order = m_workspaces.size();
  if (!orderRows.isEmpty()) {
    order =
        orderRows.first().toMap().value(QStringLiteral("next_order")).toInt();
  }

  WorkspaceItem item{QUuid::createUuid().toString(QUuid::WithoutBraces), name,
                     color, icon, presetId};

  db->execute(
      QStringLiteral(
          "INSERT INTO workspaces (id, name, color, icon, preset_id, "
          "sort_order) "
          "VALUES (:id, :name, :color, :icon, :preset_id, :sort_order);"),
      {{QStringLiteral(":id"), item.id},
       {QStringLiteral(":name"), name},
       {QStringLiteral(":color"), color},
       {QStringLiteral(":icon"), icon},
       {QStringLiteral(":preset_id"),
        presetId.isEmpty() ? QVariant() : presetId},
       {QStringLiteral(":sort_order"), order}});

  return item;
}

void WorkspaceModel::loadWorkspaces() {
  releaseTabModels();

  beginResetModel();
  m_workspaces.clear();
  m_currentIndex = 0;

  ProfileManager *pm = ProfileManager::instance();
  Database *db = pm->database();
  QVariantList rows =
      db->query(QStringLiteral("SELECT id, name, color, icon, preset_id FROM "
                               "workspaces ORDER BY sort_order ASC;"));
qInfo() << "[workspaces] profile" << pm->activeProfileId()
        << "rows:" << rows.size();
qInfo() << "[workspaces] load for profile" << pm->activeProfileId()
        << "rows:" << rows.size();
for (const auto &r : rows)
  qInfo() << "   " << r.toMap().value(QStringLiteral("id")).toString();

  for (const auto &r : rows) {
    QVariantMap map = r.toMap();
    m_workspaces.append({map.value(QStringLiteral("id")).toString(),
                         map.value(QStringLiteral("name")).toString(),
                         map.value(QStringLiteral("color")).toString(),
                         map.value(QStringLiteral("icon")).toString(),
                         map.value(QStringLiteral("preset_id")).toString()});
  }

  if (m_workspaces.isEmpty()) {
    m_workspaces.append(insertWorkspaceRecord(QStringLiteral("Workspace 1"),
                                              QStringLiteral("#3B82F6"),
                                              QStringLiteral("browser"),
                                              QString()));
    m_workspaces.append(insertWorkspaceRecord(QStringLiteral("Workspace 2"),
                                              QStringLiteral("#8B5CF6"),
                                              QStringLiteral("browser"),
                                              QString()));
  } else {
    const QString savedId = pm->stateValue(activeWorkspaceKey());
    for (int i = 0; i < m_workspaces.size(); ++i) {
      if (m_workspaces.at(i).id == savedId) {
        m_currentIndex = i;
        break;
      }
    }
  }

  endResetModel();
  emit countChanged();
  emit currentIndexChanged();
qInfo() << "[workspaces] loaded, count =" << m_workspaces.size()
        << "current =" << currentWorkspaceId();
}

void WorkspaceModel::createWorkspace(const QString &name, const QString &color,
                                     const QString &icon,
                                     const QString &presetId) {
  WorkspaceItem item = insertWorkspaceRecord(name, color, icon, presetId);

  beginInsertRows(QModelIndex(), m_workspaces.size(), m_workspaces.size());
  m_workspaces.append(item);
  endInsertRows();

  emit countChanged();
  setCurrentIndex(m_workspaces.size() - 1);
}

void WorkspaceModel::removeWorkspace(int index) {
  if (m_workspaces.size() <= 1 || index < 0 || index >= m_workspaces.size()) {
    return;
  }

  const QString id = m_workspaces.at(index).id;
  Database *db = ProfileManager::instance()->database();

  db->execute(QStringLiteral("DELETE FROM tabs WHERE workspace_id = :id;"),
              {{QStringLiteral(":id"), id}});
  db->execute(QStringLiteral("DELETE FROM workspaces WHERE id = :id;"),
              {{QStringLiteral(":id"), id}});
  ProfileManager::instance()->removeStateValue(activeTabKey(id));

  TabModel *tabs = m_tabModels.take(id);

  beginRemoveRows(QModelIndex(), index, index);
  m_workspaces.removeAt(index);
  endRemoveRows();

  if (tabs) {
    tabs->setWorkspaceId(QString());
    tabs->deleteLater();
  }

  persistWorkspaceOrder();
  emit countChanged();

  if (index < m_currentIndex) {
    m_currentIndex--;
  } else if (index == m_currentIndex) {
    m_currentIndex = qMin(index, m_workspaces.size() - 1);
  }
  persistCurrentWorkspace();
  emit currentIndexChanged();
}

void WorkspaceModel::saveAsPreset(int index, const QString &presetName) {
  if (index < 0 || index >= m_workspaces.size()) {
    return;
  }

  const auto &ws = m_workspaces.at(index);
  Database *db = ProfileManager::instance()->database();

  QVariantList tabRows = db->query(
      QStringLiteral("SELECT url, title, is_pinned FROM tabs WHERE "
                     "workspace_id = :ws_id ORDER BY sort_order ASC;"),
      {{QStringLiteral(":ws_id"), ws.id}});

  QJsonArray tabsArray;
  for (const auto &tr : tabRows) {
    QVariantMap tm = tr.toMap();
    QJsonObject tabObj;
    tabObj[QStringLiteral("url")] = tm.value(QStringLiteral("url")).toString();
    tabObj[QStringLiteral("title")] =
        tm.value(QStringLiteral("title")).toString();
    tabObj[QStringLiteral("is_pinned")] =
        tm.value(QStringLiteral("is_pinned")).toBool();
    tabsArray.append(tabObj);
  }

  QJsonObject presetData;
  presetData[QStringLiteral("color")] = ws.color;
  presetData[QStringLiteral("icon")] = ws.icon;
  presetData[QStringLiteral("tabs")] = tabsArray;

  QString presetId = QUuid::createUuid().toString(QUuid::WithoutBraces);
  db->execute(
      QStringLiteral("INSERT INTO presets (id, name, color, icon, data_json) "
                     "VALUES (:id, :name, :color, :icon, :data_json);"),
      {{QStringLiteral(":id"), presetId},
       {QStringLiteral(":name"), presetName.isEmpty() ? ws.name : presetName},
       {QStringLiteral(":color"), ws.color},
       {QStringLiteral(":icon"), ws.icon},
       {QStringLiteral(":data_json"),
        QString::fromUtf8(
            QJsonDocument(presetData).toJson(QJsonDocument::Compact))}});
}
