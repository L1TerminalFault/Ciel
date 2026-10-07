#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QString>
#include <QUuid>
#include <cstdint>
#include <qhashfunctions.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

struct FileViewSettings {
  Q_GADGET
  QML_VALUE_TYPE(fileViewSettings)
  QML_UNCREATABLE("FileViewSettings cannot be created directly")

  Q_PROPERTY(bool listViewMode MEMBER listViewMode)
  Q_PROPERTY(SortBy sortBy MEMBER sortBy)
  Q_PROPERTY(bool ascending MEMBER ascending)
  Q_PROPERTY(bool showHiddenFiles MEMBER showHiddenFiles)
  Q_PROPERTY(bool showSymlinks MEMBER showSymlinks)
  Q_PROPERTY(bool foldersFirstSorting MEMBER foldersFirstSorting)

public:
  enum class SortBy : uint8_t { DateModified = 0, Type, Name, Size };
  Q_ENUM(SortBy)
  bool listViewMode = false;
  SortBy sortBy = SortBy::DateModified;
  bool foldersFirstSorting = true;
  bool ascending = true;
  bool showHiddenFiles = false;
  bool showSymlinks = false;
};
Q_DECLARE_METATYPE(FileViewSettings)

struct TabItem {
  QUuid id = QUuid::createUuid();
  QString title;
  QString path;
  QString icon;
  int scrollPosition = 0;
  QSet<QString> selectedFiles;
  QString selectionAnchor;
  FileViewSettings settings;
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

  Q_PROPERTY(FileViewSettings currentSettings READ currentSettings NOTIFY
                 currentSettingsChanged)

public:
  enum TabRoles { IdRole = Qt::UserRole + 1, TitleRole, PathRole, IconRole };
  Q_ENUM(TabRoles)
  TabManager *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine);
  explicit TabManager(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;
  FileViewSettings currentSettings() const;
  // QML Invokables
  Q_INVOKABLE void addTab(const QString &path = QString());
  Q_INVOKABLE void closeTab(const QString &id);
  Q_INVOKABLE void toggleViewMode();
  Q_INVOKABLE void toggleHiddenFiles();
  Q_INVOKABLE void setSortBy(FileViewSettings::SortBy criteria);
  Q_INVOKABLE void toggleAscending();
  Q_INVOKABLE void toggleFoldersFirst();
  Q_INVOKABLE void toggleSymlinks();

  // Getters & Setters
  QString currentTabId() const;
  void setCurrentTabId(const QString &id);

  int currentIndex() const;
  QString currentPath() const;
  Q_INVOKABLE bool setCurrentPath(const QString &path);
  Q_INVOKABLE void goUp();
  Q_INVOKABLE void openFolder(const QString &folderName);

signals:
  void currentSettingsChanged();
  void currentTabIdChanged();
  void currentIndexChanged();
  void currentPathChanged();

private:
  int indexOf(const QUuid &id) const;

  QList<TabItem> m_tabs;
  QList<QUuid> m_history;
  QUuid m_activeTabId;
};
