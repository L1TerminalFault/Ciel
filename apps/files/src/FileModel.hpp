#pragma once

#include "TabManager.hpp"
#include "files/DirCache.hpp"
#include "files/type.hpp"
#include <QAbstractListModel>
#include <QCache>
#include <QString>
#include <QThread>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

class DirectoryLoader;

class FileListModel : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

  Q_PROPERTY(int focusedRow READ focusedRow NOTIFY focusedRowChanged)
public:
  enum Roles {
    NameRole = Qt::UserRole + 1,
    IsDirRole,
    SizeRole,
    ModifiedRole,
    IconRole,
    SelectedRole
  };
  Q_ENUM(Roles)

  explicit FileListModel(QObject *parent = nullptr);
  ~FileListModel() override;

  Q_INVOKABLE void setPath(const QString &path);
  Q_INVOKABLE bool createFolder(const QString &dirName);
  Q_INVOKABLE void setSettings(const FileViewSettings &settings);
  int focusedRow() const { return m_focusedRow; }
  Q_INVOKABLE void selectAll();
  Q_INVOKABLE void navigate(int targetRow, int modifiers = 0);
  Q_INVOKABLE int findNextByPrefix(const QString &prefix);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QHash<int, QByteArray> roleNames() const override;
  Q_INVOKABLE void handleSelection(int row, Qt::KeyboardModifiers modifiers);
  Q_INVOKABLE void clearSelection();
private slots:
  void onLoadStarted(const QString &path);
  void onEntriesReady(const QVector<ItemEntery> &batch);
  void onLoadFinished(const QString &path, qint64 modifiedTime);
  void onLoadError(const QString &path, int errorCode,
                   const QString &errorMessage);
signals:
  void loadErrorNotify(const QString err);
  void focusedRowChanged();

private:
  int m_focusedRow = 0;
  QVector<int> m_selectedIndices;
  int m_anchorIndex = -1;
  void applySortAndFilter();
  FileViewSettings m_settings;
  QVector<ItemEntery> m_rawEntries;
  QVector<ItemEntery> m_entries;
  QString m_currentPath;
  QCache<QString, CachedListing> m_cache;

  DirectoryLoader *m_loader = nullptr;
  QThread m_workerThread;
  bool m_batchLoading = true;
};
