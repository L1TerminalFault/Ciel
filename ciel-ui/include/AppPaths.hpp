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

  Q_INVOKABLE QString home() const;
  Q_INVOKABLE QString desktop() const;
  Q_INVOKABLE QString documents() const;
  Q_INVOKABLE QString downloads() const;
  Q_INVOKABLE QString pictures() const;
  Q_INVOKABLE QString music() const;
  Q_INVOKABLE QString videos() const;
  Q_INVOKABLE QString publicShare() const;
  Q_INVOKABLE QString templates() const;
  Q_INVOKABLE QString trash() const;

  // Helper ensuring directory structure exists prior to returning path
  Q_INVOKABLE QString ensureDir(const QString &path) const;
};
