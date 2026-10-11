#pragma once

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QString>
#include <QUrl>
#include <QVariantList>
#include <QtQml/qqmlregistration.h>

class SitePermissionManager : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

public:
  static SitePermissionManager *create(QQmlEngine *, QJSEngine *);
  static SitePermissionManager *instance();

  Q_INVOKABLE QString originOf(const QUrl &url) const;
  Q_INVOKABLE QString permissionState(const QString &origin, const QString &feature);
  Q_INVOKABLE void setPermission(const QString &origin, const QString &feature,
                                 const QString &state);
  Q_INVOKABLE void removePermission(const QString &origin, const QString &feature);
  Q_INVOKABLE void clearAll();
  Q_INVOKABLE QVariantList list(const QString &search = QString(),
                                const QString &stateFilter = QString());

signals:
  void permissionsChanged();

private:
  explicit SitePermissionManager(QObject *parent = nullptr);
};
