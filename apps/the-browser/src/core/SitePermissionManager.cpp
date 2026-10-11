#include "SitePermissionManager.hpp"
#include "ProfileManager.hpp"

#include <QCoreApplication>
#include <QDateTime>

namespace {
bool knownFeature(const QString &f) {
  return f == QLatin1String("microphone") || f == QLatin1String("camera") ||
         f == QLatin1String("location") || f == QLatin1String("notifications");
}
bool knownState(const QString &s) {
  return s == QLatin1String("granted") || s == QLatin1String("denied") ||
         s == QLatin1String("ask");
}

Database *db() { return ProfileManager::instance()->database(); }
} // namespace

SitePermissionManager *SitePermissionManager::instance() {
  static SitePermissionManager *s =
      new SitePermissionManager(QCoreApplication::instance());
  return s;
}

SitePermissionManager *SitePermissionManager::create(QQmlEngine *, QJSEngine *) {
  SitePermissionManager *m = instance();
  QJSEngine::setObjectOwnership(m, QJSEngine::CppOwnership);
  return m;
}

SitePermissionManager::SitePermissionManager(QObject *parent) : QObject(parent) {
  connect(ProfileManager::instance(), &ProfileManager::activeProfileChanged,
          this, &SitePermissionManager::permissionsChanged);
}

QString SitePermissionManager::originOf(const QUrl &url) const {
  const QString scheme = url.scheme().toLower();
  if (scheme.isEmpty() || url.host().isEmpty())
    return {};
  QString host = url.host();
  if (host.contains(QLatin1Char(':')))
    host = QLatin1Char('[') + host + QLatin1Char(']');
  QString out = scheme + QStringLiteral("://") + host;
  if (url.port() != -1)
    out += QLatin1Char(':') + QString::number(url.port());
  return out;
}

QString SitePermissionManager::permissionState(const QString &origin,
                                               const QString &feature) {
  if (origin.isEmpty() || !knownFeature(feature))
    return {};
  const QVariantList rows = db()->query(
      QStringLiteral("SELECT state FROM site_permissions "
                     "WHERE origin = :o AND feature = :f LIMIT 1;"),
      {{QStringLiteral(":o"), origin}, {QStringLiteral(":f"), feature}});
  return rows.isEmpty()
             ? QString()
             : rows.first().toMap().value(QStringLiteral("state")).toString();
}

void SitePermissionManager::setPermission(const QString &origin,
                                          const QString &feature,
                                          const QString &state) {
  if (origin.isEmpty() || !knownFeature(feature) || !knownState(state))
    return;
  const qint64 now = QDateTime::currentSecsSinceEpoch();
  db()->execute(
      QStringLiteral(
          "INSERT INTO site_permissions (origin, feature, state, created_at, "
          "updated_at) VALUES (:o, :f, :s, :t1, :t2) "
          "ON CONFLICT(origin, feature) DO UPDATE SET "
          "state = excluded.state, updated_at = excluded.updated_at;"),
      {{QStringLiteral(":o"), origin},
       {QStringLiteral(":f"), feature},
       {QStringLiteral(":s"), state},
       {QStringLiteral(":t1"), now},
       {QStringLiteral(":t2"), now}});
  emit permissionsChanged();
}

void SitePermissionManager::removePermission(const QString &origin,
                                             const QString &feature) {
  db()->execute(QStringLiteral("DELETE FROM site_permissions "
                               "WHERE origin = :o AND feature = :f;"),
                {{QStringLiteral(":o"), origin}, {QStringLiteral(":f"), feature}});
  emit permissionsChanged();
}

void SitePermissionManager::clearAll() {
  db()->execute(QStringLiteral("DELETE FROM site_permissions;"));
  emit permissionsChanged();
}

QVariantList SitePermissionManager::list(const QString &search,
                                         const QString &stateFilter) {
  QString sql = QStringLiteral(
      "SELECT origin, feature, state AS decision, updated_at "
      "FROM site_permissions WHERE 1 = 1 ");
  QVariantMap b;
  if (!search.trimmed().isEmpty()) {
    sql += QStringLiteral("AND origin LIKE :s ");
    b.insert(QStringLiteral(":s"),
             QStringLiteral("%") + search.trimmed() + QStringLiteral("%"));
  }
  if (knownState(stateFilter)) {
    sql += QStringLiteral("AND state = :st ");
    b.insert(QStringLiteral(":st"), stateFilter);
  }
  sql += QStringLiteral(
      "ORDER BY origin COLLATE NOCASE ASC, feature ASC;");
  return db()->query(sql, b);
}
