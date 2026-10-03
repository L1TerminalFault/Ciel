#include "AppPaths.hpp"
#include <QDebug>
#include <QDir>
#include <QStandardPaths>

AppPaths::AppPaths(QObject *parent) : QObject(parent) {}

QString AppPaths::configDir(const QString &appId) const {
  const QString base =
      QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
  return ensureDir(base + "/ciel/" + appId);
}

QString AppPaths::dataDir(const QString &appId) const {
  const QString base =
      QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);

  // If the target is the global system name, keep it flat
  if (appId == "ciel" || appId == "Ciel") {
    return ensureDir(base + "/ciel");
  }

  return ensureDir(base + "/ciel/" + appId);
}

QString AppPaths::cacheDir(const QString &appId) const {
  const QString base =
      QStandardPaths::writableLocation(QStandardPaths::GenericCacheLocation);
  return ensureDir(base + "/ciel/" + appId);
}

QString AppPaths::ensureDir(const QString &path) const {
  QDir dir(path);
  if (!dir.exists()) {
    if (!dir.mkpath(".")) {
      qWarning() << "[AppPaths] Failed to construct path:" << path;
    }
  }
  return path;
}
