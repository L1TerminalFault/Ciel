#include "Database.hpp"

#include <QDir>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QUuid>

Database::Database(QObject *parent) : QObject(parent) {
  m_connectionName = QStringLiteral("ciel_browser_") +
                     QUuid::createUuid().toString(QUuid::WithoutBraces);
}

Database::~Database() { close(); }

bool Database::initialize(const QString &dbPath) {
  QFileInfo fi(dbPath);
  QDir().mkpath(fi.absolutePath());

  QSqlDatabase db =
      QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), m_connectionName);
  m_open = true;
  db.setDatabaseName(dbPath);

  if (!db.open()) {
    return false;
  }

  return runMigrations();
}

void Database::close() {
  if (!m_open) {
    return;
  }
  m_open = false;

  if (QSqlDatabase::contains(m_connectionName)) {
    {
      QSqlDatabase db = QSqlDatabase::database(m_connectionName);
      if (db.isOpen()) {
        db.close();
      }
    }
    QSqlDatabase::removeDatabase(m_connectionName);
  }
}

bool Database::execute(const QString &queryStr, const QVariantMap &bindings) {
  QSqlDatabase db = QSqlDatabase::database(m_connectionName);
  QSqlQuery q(db);
  q.prepare(queryStr);
  for (auto it = bindings.cbegin(); it != bindings.cend(); ++it) {
    q.bindValue(it.key(), it.value());
  }
  return q.exec();
}

QVariantList Database::query(const QString &queryStr,
                             const QVariantMap &bindings) {
  QVariantList results;
  QSqlDatabase db = QSqlDatabase::database(m_connectionName);
  QSqlQuery q(db);
  q.prepare(queryStr);
  for (auto it = bindings.cbegin(); it != bindings.cend(); ++it) {
    q.bindValue(it.key(), it.value());
  }

  if (!q.exec()) {
    return results;
  }

  while (q.next()) {
    QVariantMap row;
    for (int i = 0; i < q.record().count(); ++i) {
      row.insert(q.record().fieldName(i), q.value(i));
    }
    results.append(row);
  }
  return results;
}

bool Database::runMigrations() {
  QSqlDatabase db = QSqlDatabase::database(m_connectionName);
  QSqlQuery q(db);

  const QStringList queries = {
      QStringLiteral("PRAGMA journal_mode = WAL;"),
      QStringLiteral("PRAGMA foreign_keys = ON;"),
      QStringLiteral("CREATE TABLE IF NOT EXISTS sessions ("
                     "  id TEXT PRIMARY KEY,"
                     "  started_at INTEGER NOT NULL,"
                     "  ended_at INTEGER"
                     ");"),
      QStringLiteral("CREATE TABLE IF NOT EXISTS workspaces ("
                     "  id TEXT PRIMARY KEY,"
                     "  name TEXT NOT NULL,"
                     "  color TEXT NOT NULL,"
                     "  icon TEXT NOT NULL,"
                     "  preset_id TEXT,"
                     "  sort_order INTEGER NOT NULL"
                     ");"),
      QStringLiteral("CREATE TABLE IF NOT EXISTS tabs ("
                     "  id TEXT PRIMARY KEY,"
                     "  workspace_id TEXT NOT NULL,"
                     "  url TEXT NOT NULL,"
                     "  title TEXT NOT NULL,"
                     "  is_pinned INTEGER NOT NULL DEFAULT 0,"
                     "  sort_order INTEGER NOT NULL,"
                     "  FOREIGN KEY(workspace_id) REFERENCES workspaces(id) ON "
                     "DELETE CASCADE"
                     ");"),
      QStringLiteral(
          "CREATE TABLE IF NOT EXISTS history ("
          "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
          "  workspace_id TEXT,"
          "  session_id TEXT NOT NULL,"
          "  url TEXT NOT NULL,"
          "  title TEXT NOT NULL,"
          "  timestamp INTEGER NOT NULL,"
          "  FOREIGN KEY(session_id) REFERENCES sessions(id) ON DELETE CASCADE"
          ");"),
      QStringLiteral("CREATE TABLE IF NOT EXISTS collections ("
                     "  id TEXT PRIMARY KEY,"
                     "  name TEXT NOT NULL,"
                     "  icon TEXT NOT NULL,"
                     "  color TEXT NOT NULL,"
                     "  sort_order INTEGER NOT NULL"
                     ");"),
      QStringLiteral("CREATE TABLE IF NOT EXISTS bookmarks ("
                     "  id TEXT PRIMARY KEY,"
                     "  collection_id TEXT NOT NULL,"
                     "  title TEXT NOT NULL,"
                     "  url TEXT NOT NULL,"
                     "  timestamp INTEGER NOT NULL,"
                     "  sort_order INTEGER NOT NULL,"
                     "  FOREIGN KEY(collection_id) REFERENCES collections(id) "
                     "ON DELETE CASCADE"
                     ");"),
      QStringLiteral("CREATE TABLE IF NOT EXISTS presets ("
                     "  id TEXT PRIMARY KEY,"
                     "  name TEXT NOT NULL,"
                     "  color TEXT NOT NULL,"
                     "  icon TEXT NOT NULL,"
                     "  data_json TEXT NOT NULL"
                     ");")};

  for (const auto &queryText : queries) {
    if (!q.exec(queryText)) {
      return false;
    }
  }
  return true;
}
