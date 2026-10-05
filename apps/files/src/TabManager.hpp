#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QString>
#include <QUuid>
#include <qhashfunctions.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

struct TabItem {
  QUuid id = QUuid::createUuid();
  QString title;
  QString path;
  QString icon;
  int scrollPosition = 0;
};
class QJSEngine;
class QQmlEngine;
class TabManager : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON
  Q_PROPERTY(QString currentTabId READ currentTabId WRITE setCurrentTabId NOTIFY
                 currentTabIdChanged)
  Q_PROPERTY(int currentIndex READ currentIndex NOTIFY currentIndexChanged)
  Q_PROPERTY(QString currentPath READ currentPath WRITE setCurrentPath NOTIFY
                 currentPathChanged)

public:
  enum TabRoles { IdRole = Qt::UserRole + 1, TitleRole, PathRole, IconRole };
  Q_ENUM(TabRoles)
  TabManager *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine);
  explicit TabManager(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  // QML Invokables
  Q_INVOKABLE void addTab(const QString &path = QString());
  Q_INVOKABLE void closeTab(const QString &id);

  // Getters & Setters
  QString currentTabId() const;
  void setCurrentTabId(const QString &id);

  int currentIndex() const;
  QString currentPath() const;
  Q_INVOKABLE void setCurrentPath(const QString &path);
  Q_INVOKABLE void goUp();
  Q_INVOKABLE void openFolder(const QString &folderName);

signals:
  void currentTabIdChanged();
  void currentIndexChanged();
  void currentPathChanged();

private:
  int indexOf(const QUuid &id) const;

  QList<TabItem> m_tabs;
  QList<QUuid> m_history;
  QUuid m_activeTabId;
};
