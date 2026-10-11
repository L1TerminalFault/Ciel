#pragma once

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QString>
#include <QVariantList>
#include <QtQml/qqmlregistration.h>

class ExtensionManager : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

  Q_PROPERTY(QVariantList scripts READ scripts NOTIFY scriptsChanged)

public:
  static ExtensionManager *create(QQmlEngine *, QJSEngine *);
  static ExtensionManager *instance();

  QVariantList scripts() const { return m_scripts; }

  Q_INVOKABLE QVariantList list();
  Q_INVOKABLE QString installFromFolder(const QString &folder);
  Q_INVOKABLE void setEnabled(const QString &id, bool enabled);
  Q_INVOKABLE void remove(const QString &id);

signals:
  void extensionsChanged();
  void scriptsChanged();

private:
  explicit ExtensionManager(QObject *parent = nullptr);
  QString rootDir() const;
  void rebuild();

  QVariantList m_scripts;
};
