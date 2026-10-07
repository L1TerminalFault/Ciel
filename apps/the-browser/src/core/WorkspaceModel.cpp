#include "WorkspaceModel.hpp"
#include "ProfileManager.hpp"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <QUuid>

#include <utility>

namespace {

constexpr bool kAllowUserWorkspaces = false;

struct DefaultWorkspace {
  const char *name;
  const char *color;
};

constexpr DefaultWorkspace kDefaultWorkspaces[] = {
    {"Workspace 1", "#3B82F6"}, {"Workspace 2", "#8B5CF6"},
    {"Workspace 3", "#10B981"}, {"Workspace 4", "#F59E0B"},
    {"Workspace 5", "#EF4444"},
};

QString activeWorkspaceKey() { return QStringLiteral("active_workspace_id"); }
QString activeTabKey(const QString &workspaceId) {
  return QStringLiteral("active_tab:") + workspaceId;
}

} // namespace

WorkspaceModel::WorkspaceModel(QObject *parent) : QAbstractListModel(parent) {
  auto *pm = ProfileManager::instance();

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

QString WorkspaceModel::uniqueName(const QString &requested) const {
  static const QRegularExpression numbered(
      QStringLiteral("^Workspace (\\d+)$"));

  bool taken = false;
  int maxN = 0;
  for (const auto &ws : std::as_const(m_workspaces)) {
    if (ws.name == requested)
      taken = true;
    const auto m = numbered.match(ws.name);
    if (m.hasMatch())
      maxN = qMax(maxN, m.captured(1).toInt());
  }

  if (requested.trimmed().isEmpty())
    return QStringLiteral("Workspace %1").arg(maxN + 1);
  if (!taken)
    return requested;
  if (numbered.match(requested).hasMatch())
    return QStringLiteral("Workspace %1").arg(maxN + 1);

  for (int i = 2;; ++i) {
    const QString candidate = QStringLiteral("%1 (%2)").arg(requested).arg(i);
    bool exists = false;
    for (const auto &ws : std::as_const(m_workspaces))
      exists = exists || ws.name == candidate;
    if (!exists)
      return candidate;
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

  WorkspaceItem item{QUuid::createUuid().toString(QUuid::WithoutBraces),
                     uniqueName(name), color, icon, presetId};

  db->execute(
      QStringLiteral(
          "INSERT INTO workspaces (id, name, color, icon, preset_id, "
          "sort_order) "
          "VALUES (:id, :name, :color, :icon, :preset_id, :sort_order);"),
      {{QStringLiteral(":id"), item.id},
       {QStringLiteral(":name"), item.name},
       {QStringLiteral(":color"), color},
       {QStringLiteral(":icon"), icon},
       {QStringLiteral(":preset_id"),
        presetId.isEmpty() ? QVariant() : presetId},
       {QStringLiteral(":sort_order"), order}});

  return item;
}

void WorkspaceModel::readWorkspaceRows() {
  m_workspaces.clear();

  Database *db = ProfileManager::instance()->database();
  QVariantList rows = db->query(
      QStringLiteral("SELECT id, name, color, icon, preset_id FROM "
                     "workspaces ORDER BY sort_order ASC, rowid ASC;"));

  for (const auto &r : rows) {
    QVariantMap map = r.toMap();
    m_workspaces.append({map.value(QStringLiteral("id")).toString(),
                         map.value(QStringLiteral("name")).toString(),
                         map.value(QStringLiteral("color")).toString(),
                         map.value(QStringLiteral("icon")).toString(),
                         map.value(QStringLiteral("preset_id")).toString()});
  }
}

void WorkspaceModel::seedDefaultWorkspaces() {
  Database *db = ProfileManager::instance()->database();

  int order = 0;
  for (const auto &def : kDefaultWorkspaces) {
    db->execute(
        QStringLiteral(
            "INSERT OR IGNORE INTO workspaces (id, name, color, icon, "
            "preset_id, sort_order) "
            "VALUES (:id, :name, :color, :icon, NULL, :sort_order);"),
        {{QStringLiteral(":id"),
          QStringLiteral("default-workspace-%1").arg(order + 1)},
         {QStringLiteral(":name"), QString::fromLatin1(def.name)},
         {QStringLiteral(":color"), QString::fromLatin1(def.color)},
         {QStringLiteral(":icon"), QStringLiteral("browser")},
         {QStringLiteral(":sort_order"), order}});
    ++order;
  }
}

void WorkspaceModel::loadWorkspaces() {
  releaseTabModels();

  beginResetModel();
  m_currentIndex = 0;

  readWorkspaceRows();

  if (m_workspaces.isEmpty()) {
    seedDefaultWorkspaces();
    readWorkspaceRows();
  } else {
    persistWorkspaceOrder();
  }

  const QString savedId =
      ProfileManager::instance()->stateValue(activeWorkspaceKey());
  for (int i = 0; i < m_workspaces.size(); ++i) {
    if (m_workspaces.at(i).id == savedId) {
      m_currentIndex = i;
      break;
    }
  }

  endResetModel();
  emit countChanged();
  emit currentIndexChanged();
}

void WorkspaceModel::createWorkspace(const QString &name, const QString &color,
                                     const QString &icon,
                                     const QString &presetId) {
  if (!kAllowUserWorkspaces) {
    qInfo() << "[workspaces] createWorkspace ignored (fixed default set):"
            << name;
    return;
  }

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
