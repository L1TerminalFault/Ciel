#include "WorkspaceModel.hpp"
#include "ProfileManager.hpp"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUuid>

WorkspaceModel::WorkspaceModel(QObject *parent) : QAbstractListModel(parent) {
  connect(ProfileManager::instance(), &ProfileManager::activeProfileChanged,
          this, &WorkspaceModel::loadWorkspaces);
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

  if (!m_tabModels.contains(workspaceId)) {
    auto *model = new TabModel(this);
    model->setWorkspaceId(workspaceId);
    m_tabModels.insert(workspaceId, model);
  }
  return m_tabModels.value(workspaceId);
}

void WorkspaceModel::loadWorkspaces() {
  beginResetModel();
  m_workspaces.clear();

  Database *db = ProfileManager::instance()->database();
  QVariantList rows =
      db->query(QStringLiteral("SELECT id, name, color, icon, preset_id FROM "
                               "workspaces ORDER BY sort_order ASC;"));

  for (const auto &r : rows) {
    QVariantMap map = r.toMap();
    m_workspaces.append({map.value(QStringLiteral("id")).toString(),
                         map.value(QStringLiteral("name")).toString(),
                         map.value(QStringLiteral("color")).toString(),
                         map.value(QStringLiteral("icon")).toString(),
                         map.value(QStringLiteral("preset_id")).toString()});
  }

  if (m_workspaces.isEmpty()) {
    endResetModel();
    createWorkspace(QStringLiteral("Workspace 1"), QStringLiteral("#3B82F6"),
                    QStringLiteral("browser"));
    createWorkspace(QStringLiteral("Workspace 2"), QStringLiteral("#8B5CF6"),
                    QStringLiteral("browser"));
    return;
  }

  m_currentIndex = 0;
  endResetModel();
  emit countChanged();
  emit currentIndexChanged();
}

void WorkspaceModel::createWorkspace(const QString &name, const QString &color,
                                     const QString &icon,
                                     const QString &presetId) {
  QString id = QUuid::createUuid().toString(QUuid::WithoutBraces);
  int order = m_workspaces.size();

  Database *db = ProfileManager::instance()->database();
  db->execute(
      QStringLiteral(
          "INSERT INTO workspaces (id, name, color, icon, preset_id, "
          "sort_order) "
          "VALUES (:id, :name, :color, :icon, :preset_id, :sort_order);"),
      {{QStringLiteral(":id"), id},
       {QStringLiteral(":name"), name},
       {QStringLiteral(":color"), color},
       {QStringLiteral(":icon"), icon},
       {QStringLiteral(":preset_id"),
        presetId.isEmpty() ? QVariant() : presetId},
       {QStringLiteral(":sort_order"), order}});

  beginInsertRows(QModelIndex(), m_workspaces.size(), m_workspaces.size());
  m_workspaces.append({id, name, color, icon, presetId});
  endInsertRows();

  emit countChanged();
  setCurrentIndex(m_workspaces.size() - 1);
}

void WorkspaceModel::removeWorkspace(int index) {
  if (m_workspaces.size() <= 1 || index < 0 || index >= m_workspaces.size()) {
    return;
  }

  QString id = m_workspaces.at(index).id;
  Database *db = ProfileManager::instance()->database();
  db->execute(QStringLiteral("DELETE FROM workspaces WHERE id = :id;"),
              {{QStringLiteral(":id"), id}});

  if (m_tabModels.contains(id)) {
    delete m_tabModels.take(id);
  }

  beginRemoveRows(QModelIndex(), index, index);
  m_workspaces.removeAt(index);
  endRemoveRows();

  emit countChanged();

  if (m_currentIndex >= m_workspaces.size()) {
    setCurrentIndex(m_workspaces.size() - 1);
  } else if (m_currentIndex == index) {
    emit currentIndexChanged();
  }
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
