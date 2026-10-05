#include "ProfileManager.hpp"

#include <QDir>
#include <QStandardPaths>
#include <QUuid>

ProfileManager *ProfileManager::s_instance = nullptr;

ProfileManager *ProfileManager::create(QQmlEngine *, QJSEngine *) {
  return instance();
}

ProfileManager *ProfileManager::instance() {
  if (!s_instance) {
    s_instance = new ProfileManager();
  }
  return s_instance;
}

ProfileManager::ProfileManager(QObject *parent) : QObject(parent) {
  s_instance = this;
  switchProfile(QStringLiteral("default"));
}

ProfileManager::~ProfileManager() {
  endSession();
  m_database.close();
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

void ProfileManager::switchProfile(const QString &profileId) {
  if (m_activeProfileId == profileId && !m_currentSessionId.isEmpty()) {
    return;
  }

  if (!m_currentSessionId.isEmpty()) {
    endSession();
    m_database.close();
  }

    m_activeProfileId = profileId;
    m_profilePath = profilePathFor(profileId);

    QDir().mkpath(m_profilePath + QStringLiteral("/webengine"));
    QDir().mkpath(m_profilePath + QStringLiteral("/webengine/cache"));

  m_activeProfileId = profileId;
  m_profilePath = profilePathFor(profileId);
  // m_profilePath =
  //     QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) +
  //     QStringLiteral("/ciel/browser/profiles/") + profileId;

  QDir().mkpath(m_profilePath + QStringLiteral("/webengine"));

  QString dbPath = m_profilePath + QStringLiteral("/browser.db");
  m_database.initialize(dbPath);

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
