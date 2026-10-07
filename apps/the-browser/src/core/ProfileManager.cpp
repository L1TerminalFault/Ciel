#include "ProfileManager.hpp"

#include <random>

#include <QSettings>
#include <QCoreApplication>
#include <QString>
#include <QColor>
#include <QDir>
#include <QStandardPaths>
#include <QUuid>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

ProfileManager *ProfileManager::s_instance = nullptr;

static QJsonObject loadProfilesMeta();
static QString profilesMetaPath();

static QString appStatePath() {
  return QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) +
         QStringLiteral("/ciel/browser/state.ini");
}

static QString loadLastProfileId() {
  QSettings s(appStatePath(), QSettings::IniFormat);
  return s.value(QStringLiteral("lastActiveProfile")).toString();
}

static void saveLastProfileId(const QString &id) {
  QDir().mkpath(QFileInfo(appStatePath()).absolutePath());
  QSettings s(appStatePath(), QSettings::IniFormat);
  s.setValue(QStringLiteral("lastActiveProfile"), id);
  s.sync();
}

ProfileManager *ProfileManager::create(QQmlEngine *, QJSEngine *) {
  ProfileManager *pm = instance();
  QJSEngine::setObjectOwnership(pm, QJSEngine::CppOwnership);
  return pm;
}

ProfileManager *ProfileManager::instance() {
  if (!s_instance) {
    new ProfileManager(QCoreApplication::instance());
  }
  return s_instance;
}

ProfileManager::~ProfileManager() {
  endSession();
  m_database.close();
  if (s_instance == this)
    s_instance = nullptr;
}

ProfileManager::ProfileManager(QObject *parent) : QObject(parent) {
  s_instance = this;

  QString startId = loadLastProfileId();
  if (startId.isEmpty() ||
      (startId != QStringLiteral("default") &&
       !loadProfilesMeta().contains(startId))) {
    startId = QStringLiteral("default");
  }
  switchProfile(startId);
}

QString ProfileManager::activeProfileId() const { return m_activeProfileId; }

QString ProfileManager::currentSessionId() const { return m_currentSessionId; }

QString ProfileManager::profilePath() const { return m_profilePath; }

Database *ProfileManager::database() { return &m_database; }

QString ProfileManager::profilePathFor(const QString &profileId) const {
    return QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
           + QStringLiteral("/ciel/browser/profiles/") + profileId;
}

QString ProfileManager::webEngineStoragePathFor(const QString &profileId) const {
    return profilePathFor(profileId) + QStringLiteral("/webengine");
}

QString ProfileManager::webEngineStoragePath() const {
    return webEngineStoragePathFor(m_activeProfileId);
}

static QString profilesMetaPath() {
    return QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
           + QStringLiteral("/ciel/browser/profiles.json");
}

static QJsonObject loadProfilesMeta() {
    QFile f(profilesMetaPath());
    if (!f.open(QIODevice::ReadOnly))
        return {};
    return QJsonDocument::fromJson(f.readAll()).object();
}

static bool saveProfilesMeta(const QJsonObject &root) {
    QFile f(profilesMetaPath());
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;
    f.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    return true;
}

QVariantList ProfileManager::listProfiles() const {
    QVariantList result;
    QJsonObject root = loadProfilesMeta();

    if (!root.contains("default")) {
        root.insert("default", QJsonObject{
            {"displayName", "Default"},
            {"color", ""},
            {"profileImage", ""}
        });
        saveProfilesMeta(root);
    }

    for (auto it = root.begin(); it != root.end(); ++it) {
        const QString id = it.key();
        const QJsonObject obj = it.value().toObject();

        result.append(QVariantMap{
            {"id",           id},
            {"displayName",  obj.value("displayName").toString(id)},
            {"color",        obj.value("color").toString()},
            {"profileImage", obj.value("profileImage").toString()},
            {"isActive",     m_activeProfileId == id}
        });
    }
    return result;
}

static QString generateRandomHexColor() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    std::uniform_int_distribution<int> hueDist(0, 359);
    
    std::uniform_int_distribution<int> satDist(165, 215); 
    
    std::uniform_int_distribution<int> lightDist(204, 230);

    int h = hueDist(gen);
    int s = satDist(gen);
    int l = lightDist(gen);

    QColor color = QColor::fromHsl(h, s, l);
    return color.name(QColor::HexRgb).toUpper(); 
}

QString ProfileManager::createProfile(const QString &displayName,
                                      const QString &color,
                                      const QString &profileImage) {
    if (displayName.trimmed().isEmpty())
        return {};

    const QString id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    const QString _color = color.isEmpty() ? generateRandomHexColor() : color;

    const QString path = profilePathFor(id);
    if (!QDir().mkpath(path + QStringLiteral("/webengine/cache")))
        return {};

    QJsonObject root = loadProfilesMeta();
    root.insert(id, QJsonObject{
        {"displayName",  displayName.trimmed()},
        {"color",        _color},
        {"profileImage", profileImage}
    });
    if (!saveProfilesMeta(root))
        return {};

    emit profilesChanged();
    return id;
}

bool ProfileManager::updateProfile(const QString &id,
                                   const QString &displayName,
                                   const QString &color,
                                   const QString &profileImage)
{
    if (id.isEmpty() || displayName.trimmed().isEmpty())
        return false;

    QJsonObject root = loadProfilesMeta();
    if (!root.contains(id))
        return false;

    root.insert(id, QJsonObject{
        {"displayName",  displayName.trimmed()},
        {"color",        color},
        {"profileImage", profileImage}
    });

    if (!saveProfilesMeta(root))
        return false;

    emit profilesChanged();

    return true;
}

bool ProfileManager::deleteProfile(const QString &id) {
    if (id == "default" || id == m_activeProfileId)
        return false;

    const QString path = profilePathFor(id);
    if (QDir(path).exists())
        QDir(path).removeRecursively();

    QJsonObject root = loadProfilesMeta();
    root.remove(id);

    if (!saveProfilesMeta(root))
        return false;

    emit profilesChanged();

    return saveProfilesMeta(root);
}

QVariantMap ProfileManager::profileInfo(const QString &id) const {
    QJsonObject root = loadProfilesMeta();
    if (!root.contains(id))
        return {};

    const QJsonObject obj = root.value(id).toObject();
    return QVariantMap{
        {"id",           id},
        {"displayName",  obj.value("displayName").toString(id)},
        {"color",        obj.value("color").toString()},
        {"profileImage", obj.value("profileImage").toString()},
        {"isActive",     m_activeProfileId == id}
    };
}

void ProfileManager::switchProfile(const QString &profileId) {
qInfo() << "[profile] switchProfile on instance" << this << "->" << profileId
        << "| static instance =" << s_instance
        << "| listeners:" << receivers(SIGNAL(activeProfileChanged()));
  if (profileId.isEmpty())
    return;
  if (m_activeProfileId == profileId && !m_currentSessionId.isEmpty())
    return;

  if (profileId != QStringLiteral("default") &&
      !loadProfilesMeta().contains(profileId)) {
    qWarning() << "switchProfile: unknown profile" << profileId;
    return;
  }

  if (!m_currentSessionId.isEmpty()) {
    emit activeProfileAboutToChange();
    endSession();
    m_database.close();
    m_currentSessionId.clear();
  }

  m_activeProfileId = profileId;
  m_profilePath = profilePathFor(profileId);

  QDir().mkpath(m_profilePath + QStringLiteral("/webengine/cache"));

  const QString dbPath = m_profilePath + QStringLiteral("/browser.db");
  if (m_database.initialize(dbPath)) {
    saveLastProfileId(profileId);
  } else {
    qWarning() << "switchProfile: failed to open database" << dbPath;
  }

    qWarning() << "switchProfile: failed to open database" << dbPath;
qInfo() << "[profile] switched to" << profileId << "db:" << dbPath;

  ensureStateTable();
  startSession();
  emit activeProfileChanged();
}

void ProfileManager::startSession() {
  m_currentSessionId = QUuid::createUuid().toString(QUuid::WithoutBraces);
  qint64 now = QDateTime::currentSecsSinceEpoch();

  m_database.execute(
      QStringLiteral(
          "INSERT INTO sessions (id, started_at) VALUES (:id, :started_at);"),
      {{QStringLiteral(":id"), m_currentSessionId},
       {QStringLiteral(":started_at"), now}});
}

void ProfileManager::endSession() {
  if (m_currentSessionId.isEmpty()) {
    return;
  }

  qint64 now = QDateTime::currentSecsSinceEpoch();
  m_database.execute(
      QStringLiteral(
          "UPDATE sessions SET ended_at = :ended_at WHERE id = :id;"),
      {{QStringLiteral(":id"), m_currentSessionId},
       {QStringLiteral(":ended_at"), now}});
}

void ProfileManager::recordVisit(const QString &url, const QString &title,
                                 const QString &workspaceId) {
  if (url.isEmpty() || url == QStringLiteral("about:blank") ||
      url == QStringLiteral("ciel://history") ||
      url == QStringLiteral("about:history")) {
    return;
  }

  qint64 now = QDateTime::currentSecsSinceEpoch();
  m_database.execute(
      QStringLiteral(
          "INSERT INTO history (workspace_id, session_id, url, title, "
          "timestamp) "
          "VALUES (:workspace_id, :session_id, :url, :title, :timestamp);"),
      {{QStringLiteral(":workspace_id"),
        workspaceId.isEmpty() ? QVariant() : workspaceId},
       {QStringLiteral(":session_id"), m_currentSessionId},
       {QStringLiteral(":url"), url},
       {QStringLiteral(":title"), title.isEmpty() ? url : title},
       {QStringLiteral(":timestamp"), now}});

  emit visitRecorded(url, title, workspaceId);
}

QVariantList ProfileManager::fetchHistory(const QString &search) {
  QString queryStr = QStringLiteral(
      "SELECT h.id, h.workspace_id, h.session_id, h.url, h.title, h.timestamp, "
      "w.name AS workspace_name, w.color AS workspace_color "
      "FROM history h "
      "LEFT JOIN workspaces w ON h.workspace_id = w.id ");

  QVariantMap bindings;
  if (!search.trimmed().isEmpty()) {
    queryStr += QStringLiteral("WHERE h.title LIKE :s OR h.url LIKE :s ");
    bindings.insert(QStringLiteral(":s"), QStringLiteral("%") +
                                              search.trimmed() +
                                              QStringLiteral("%"));
  }

  queryStr += QStringLiteral("ORDER BY h.timestamp DESC LIMIT 300;");

  return m_database.query(queryStr, bindings);
}

void ProfileManager::deleteHistoryItem(qint64 id) {
  m_database.execute(QStringLiteral("DELETE FROM history WHERE id = :id;"),
                     {{QStringLiteral(":id"), id}});
}

void ProfileManager::clearHistory(int rangeIndex) {
  qint64 now = QDateTime::currentSecsSinceEpoch();
  qint64 cutoff = 0;

  switch (rangeIndex) {
  case 0:
    cutoff = now - 3600;
    break;
  case 1:
    cutoff = now - 86400;
    break;
  case 2:
    cutoff = now - (7 * 86400);
    break;
  case 3:
  default:
    cutoff = 0;
    break;
  }

  if (cutoff == 0) {
    m_database.execute(QStringLiteral("DELETE FROM history;"));
  } else {
    m_database.execute(
        QStringLiteral("DELETE FROM history WHERE timestamp >= :cutoff;"),
        {{QStringLiteral(":cutoff"), cutoff}});
  }
}

bool ProfileManager::isBookmarked(const QString &url) {
  if (url.isEmpty() || url == QStringLiteral("about:blank") ||
      url == QStringLiteral("ciel://history")) {
    return false;
  }

  QVariantList rows = m_database.query(
      QStringLiteral("SELECT id FROM bookmarks WHERE url = :url LIMIT 1;"),
      {{QStringLiteral(":url"), url}});

  return !rows.isEmpty();
}

bool ProfileManager::toggleBookmark(const QString &url, const QString &title) {
  if (url.isEmpty() || url == QStringLiteral("about:blank") ||
      url == QStringLiteral("ciel://history")) {
    return false;
  }

  if (isBookmarked(url)) {
    m_database.execute(
        QStringLiteral("DELETE FROM bookmarks WHERE url = :url;"),
        {{QStringLiteral(":url"), url}});
    emit bookmarksChanged();
    return false;
  }

  m_database.execute(QStringLiteral(
      "INSERT OR IGNORE INTO collections (id, name, icon, color, sort_order) "
      "VALUES ('default', 'Bookmarks', 'bookmark', '#3B82F6', 0);"));

  QString id = QUuid::createUuid().toString(QUuid::WithoutBraces);
  qint64 now = QDateTime::currentSecsSinceEpoch();

  m_database.execute(
      QStringLiteral("INSERT INTO bookmarks (id, collection_id, title, url, "
                     "timestamp, sort_order) "
                     "VALUES (:id, 'default', :title, :url, :timestamp, 0);"),
      {{QStringLiteral(":id"), id},
       {QStringLiteral(":title"), title.isEmpty() ? url : title},
       {QStringLiteral(":url"), url},
       {QStringLiteral(":timestamp"), now}});

  emit bookmarksChanged();
  return true;
}

void ProfileManager::ensureStateTable() {
  m_database.execute(QStringLiteral(
      "CREATE TABLE IF NOT EXISTS profile_state ("
      "state_key TEXT PRIMARY KEY, state_value TEXT NOT NULL);"));
}

QString ProfileManager::stateValue(const QString &key, const QString &fallback) {
  QVariantList rows = m_database.query(
      QStringLiteral("SELECT state_value FROM profile_state "
                     "WHERE state_key = :k LIMIT 1;"),
      {{QStringLiteral(":k"), key}});
  if (rows.isEmpty())
    return fallback;
  return rows.first().toMap().value(QStringLiteral("state_value")).toString();
}

void ProfileManager::setStateValue(const QString &key, const QString &value) {
  m_database.execute(
      QStringLiteral("INSERT OR REPLACE INTO profile_state "
                     "(state_key, state_value) VALUES (:k, :v);"),
      {{QStringLiteral(":k"), key}, {QStringLiteral(":v"), value}});
}

void ProfileManager::removeStateValue(const QString &key) {
  m_database.execute(
      QStringLiteral("DELETE FROM profile_state WHERE state_key = :k;"),
      {{QStringLiteral(":k"), key}});
}
