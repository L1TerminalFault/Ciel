#pragma once

#include "files/FileOperation.hpp"
#include <QAbstractListModel>
#include <QList>
#include <QString>
#include <QThread>
#include <QUuid>
#include <QVector>
#include <cstdint>
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
  QVector<int> selectedFiles;
  int selectionAnchor = -1;
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
  Q_PROPERTY(bool hasClipboard READ hasClipboard NOTIFY clipboardChanged)

public:
  enum TabRoles { IdRole = Qt::UserRole + 1, TitleRole, PathRole, IconRole };
  Q_ENUM(TabRoles)

  static TabManager *instance();
  static TabManager *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine);
  explicit TabManager(QObject *parent = nullptr);
  ~TabManager() override;

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;
  FileViewSettings currentSettings() const;

  bool isRowSelected(int row) const;
  const QVector<int> &currentTabSelection() const;
  int currentTabAnchor() const;
  void setSelection(const QVector<int> &selection, int anchor);
  void toggleSelection(int row);
  void selectRange(int start, int end);
  void selectSingle(int row);
  void selectAll(int count);
  void clearSelection();

  bool hasClipboard() const;

  Q_INVOKABLE void addTab(const QString &path = QString());
  Q_INVOKABLE void closeTab(const QString &id);
  Q_INVOKABLE void toggleViewMode();
  Q_INVOKABLE void toggleHiddenFiles();
  Q_INVOKABLE void setSortBy(FileViewSettings::SortBy criteria);
  Q_INVOKABLE void toggleAscending();
  Q_INVOKABLE void toggleFoldersFirst();
  Q_INVOKABLE void toggleSymlinks();
  Q_INVOKABLE void deleteSelected(bool permanent);
  Q_INVOKABLE void createFile(const QString &fileName);

  Q_INVOKABLE void setClipboardText(const QString &text);
  Q_INVOKABLE QString getClipboardText();

  QString currentTabId() const;
  void setCurrentTabId(const QString &id);

  int currentIndex() const;
  QString currentPath() const;
  Q_INVOKABLE bool setCurrentPath(const QString &path);
  Q_INVOKABLE void goUp();
  Q_INVOKABLE void openFolder(const QString &folderName);

  Q_INVOKABLE void addSelectedToClipboard();
  Q_INVOKABLE void setCutMode(bool isCutMode);
  Q_INVOKABLE void paste();
  Q_INVOKABLE void resolveConflict(int action);
  Q_INVOKABLE void cancelOperation();

signals:
  void currentSettingsChanged();
  void currentTabIdChanged();
  void currentIndexChanged();
  void currentPathChanged();
  void clipboardChanged();

  void copyProgress(qint64 bytesCopied, qint64 totalBytes,
                    const QString &currentFile);
  void conflictDetected(const QString &fileName);
  void operationFailed(const QString &errorMsg);
  void copyFinished();

private:
  int indexOf(const QUuid &id) const;

  QList<TabItem> m_tabs;
  QList<QUuid> m_history;
  QUuid m_activeTabId;

  QVector<QString> m_clipboardPaths;
  bool m_cutToTarget = false;

  FileOperationWorker *m_activeWorker = nullptr;
  QThread *m_workerThread = nullptr;
};
