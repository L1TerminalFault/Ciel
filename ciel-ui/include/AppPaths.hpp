#pragma once

#include <QObject>
#include <QString>
#include <QtQml/qqmlregistration.h>

class AppPaths : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

public:
  explicit AppPaths(QObject *parent = nullptr);

  // Q_INVOKABLE methods callable directly from QML
  Q_INVOKABLE QString configDir(const QString &appId) const;
  Q_INVOKABLE QString dataDir(const QString &appId) const;
  Q_INVOKABLE QString cacheDir(const QString &appId) const;

  // Helper ensuring directory structure exists prior to returning path
  Q_INVOKABLE QString ensureDir(const QString &path) const;
};
