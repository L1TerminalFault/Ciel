#include "FileModel.hpp"
#include "files/DirCache.hpp"
#include "files/DirLoader.hpp"
#include <qabstractitemmodel.h>
#include <qdir.h>
#include <qfileinfo.h>

FileListModel::FileListModel(QObject *parent) : QAbstractListModel(parent) {
  m_cache.setMaxCost(25);
  qRegisterMetaType<QVector<ItemEntery>>("QVector<ItemEntery>");
  m_loader = new DirectoryLoader;
  m_loader->moveToThread(&m_workerThread);
  connect(m_loader, &DirectoryLoader::loadStarted, this,
          &FileListModel::onLoadStarted);
  connect(m_loader, &DirectoryLoader::entriesReady, this,
          &FileListModel::onEntriesReady);
  connect(m_loader, &DirectoryLoader::loadFinished, this,
          &FileListModel::onLoadFinished);
  connect(m_loader, &DirectoryLoader::loadError, this,
          &FileListModel::onLoadError);

  m_workerThread.start();
}

FileListModel::~FileListModel() {
  if (m_loader) {
    m_loader->cancel();
  }
  m_workerThread.quit();
  m_workerThread.wait();
  delete m_loader;
}

void FileListModel::setPath(const QString &path) {
  if (path.isEmpty() || path == m_currentPath) {
    return;
  }
  m_loader->cancel();
  m_currentPath = path;
  if (CachedListing *cached = m_cache.object(path)) {
    beginResetModel();
    m_entries = cached->entries;
    endResetModel();
    QMetaObject::invokeMethod(m_loader,
                              &DirectoryLoader::revalidateInBackground,
                              Qt::QueuedConnection, path, cached->dirMtime);
  } else {
    beginResetModel();
    m_entries.clear();
    endResetModel();

    // Queue worker thread to start streaming files
    QMetaObject::invokeMethod(m_loader, &DirectoryLoader::loadDirectory,
                              Qt::QueuedConnection, path, m_batchLoading);
  }
};

int FileListModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;
  return m_entries.size();
}

QHash<int, QByteArray> FileListModel::roleNames() const {
  return {{NameRole, "name"},
          {IsDirRole, "isDir"},
          {SizeRole, "size"},
          {ModifiedRole, "modified"},
          {IconRole, "icon"}};
}

QVariant FileListModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() < 0 || index.row() >= m_entries.size())
    return QVariant();

  const ItemEntery &entry = m_entries.at(index.row());

  switch (role) {
  case NameRole:
  case Qt::DisplayRole:
    return QFile::decodeName(entry.name);

  case IsDirRole:
    return entry.isDir;

  case SizeRole:
    return entry.size;

  case ModifiedRole:
    return QDateTime::fromSecsSinceEpoch(entry.modified)
        .toString(QStringLiteral("MMM d, yyyy"));

  case IconRole:
    return entry.isDir ? QStringLiteral("folder") : QStringLiteral("file");

  default:
    return QVariant();
  }
}

void FileListModel::onEntriesReady(const QVector<ItemEntery> &batch) {
  if (batch.isEmpty())
    return;

  int first = m_entries.size();
  int last = first + batch.size() - 1;

  beginInsertRows(QModelIndex(), first, last);
  m_entries.append(batch);
  endInsertRows();
}

void FileListModel::onLoadFinished(const QString &path, qint64 modifiedTime) {
  // Only cache if the user is still looking at this path
  if (path == m_currentPath) {
    m_cache.insert(path, new CachedListing{m_entries, modifiedTime});
  }
}

void FileListModel::onLoadError(const QString &path, int errorCode,
                                const QString &errorMessage) {
  beginResetModel();
  m_entries.clear();
  endResetModel();

  emit loadErrorNotify(errorMessage);
}
void FileListModel::onLoadStarted(const QString &path) {
  if (path == m_currentPath) {
    beginResetModel();
    m_entries.clear();
    endResetModel();
  }
}

bool FileListModel::createFolder(const QString &dirName) {
  if (dirName.isEmpty())
    return false;

  QDir dir(m_currentPath);
  if (!dir.mkdir(dirName))
    return false;

  QMetaObject::invokeMethod(m_loader, &DirectoryLoader::loadDirectory,
                            Qt::QueuedConnection, m_currentPath,
                            m_batchLoading);
  return true;
}
