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

public:
  enum Roles {
    NameRole = Qt::UserRole + 1,
    IsDirRole,
    SizeRole,
    ModifiedRole,
    IconRole
  };
  Q_ENUM(Roles)

  explicit FileListModel(QObject *parent = nullptr);
  ~FileListModel() override;

  Q_INVOKABLE void setPath(const QString &path);
  Q_INVOKABLE bool createFolder(const QString &dirName);
  Q_INVOKABLE void setSettings(const FileViewSettings &settings);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QHash<int, QByteArray> roleNames() const override;

private slots:
  void onLoadStarted(const QString &path);
  void onEntriesReady(const QVector<ItemEntery> &batch);
  void onLoadFinished(const QString &path, qint64 modifiedTime);
  void onLoadError(const QString &path, int errorCode,
                   const QString &errorMessage);
signals:
  void loadErrorNotify(const QString err);

private:
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
