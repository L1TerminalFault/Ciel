#include "FileModel.hpp"
#include "TabManager.hpp"
#include "files/DirCache.hpp"
#include "files/DirLoader.hpp"
#include <algorithm>
#include <qabstractitemmodel.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <sys/stat.h>

#if defined(Q_OS_LINUX)
inline int fastCompareName(const QByteArray &a, const QByteArray &b) {
  return strverscmp(a.constData(), b.constData());
}
#else
inline int fastCompareName(const QByteArray &a, const QByteArray &b) {
  return qstricmp(a.constData(), b.constData());
}
#endif

inline std::string_view getExtension(const QByteArray &name) {
  int dot = name.lastIndexOf('.');
  if (dot <= 0)
    return {};
  return std::string_view(name.constData() + dot + 1, name.size() - dot - 1);
}

inline bool compareEntries(const ItemEntery &a, const ItemEntery &b,
                           const FileViewSettings &s) {
  if (s.foldersFirstSorting && (a.isDir != b.isDir)) {
    return a.isDir;
  }

  int cmp = 0;
  switch (s.sortBy) {
  case FileViewSettings::SortBy::DateModified:
    if (a.modified != b.modified)
      return s.ascending ? (a.modified < b.modified)
                         : (a.modified > b.modified);
    break;

  case FileViewSettings::SortBy::Size:
    if (a.size != b.size)
      return s.ascending ? (a.size < b.size) : (a.size > b.size);
    break;

  case FileViewSettings::SortBy::Type: {
    if (!a.isDir && !b.isDir) {
      auto extA = getExtension(a.name);
      auto extB = getExtension(b.name);
      cmp = extA.compare(extB);
      if (cmp != 0) {
        return s.ascending ? (cmp < 0) : (cmp > 0);
      }
    }
    break;
  }

  case FileViewSettings::SortBy::Name:
  default:
    break;
  }

  cmp = fastCompareName(a.name, b.name);
  return s.ascending ? (cmp < 0) : (cmp > 0);
}

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
    m_rawEntries = cached->entries;
    applySortAndFilter();
    QMetaObject::invokeMethod(m_loader,
                              &DirectoryLoader::revalidateInBackground,
                              Qt::QueuedConnection, path, cached->dirMtime);
  } else {
    m_rawEntries.clear();
    beginResetModel();
    m_entries.clear();
    endResetModel();

    QMetaObject::invokeMethod(m_loader, &DirectoryLoader::loadDirectory,
                              Qt::QueuedConnection, path, m_batchLoading);
  }
}

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

void FileListModel::onLoadStarted(const QString &path) {
  if (path == m_currentPath) {
    m_rawEntries.clear();
    beginResetModel();
    m_entries.clear();
    endResetModel();
  }
}

void FileListModel::onEntriesReady(const QVector<ItemEntery> &batch) {
  if (batch.isEmpty())
    return;

  m_rawEntries.append(batch);
}

void FileListModel::onLoadFinished(const QString &path, qint64 modifiedTime) {
  if (path == m_currentPath) {
    applySortAndFilter();
    m_cache.insert(path, new CachedListing{m_rawEntries, modifiedTime});
  }
}

void FileListModel::onLoadError(const QString &path, int errorCode,
                                const QString &errorMessage) {
  if (path == m_currentPath) {
    m_rawEntries.clear();
    beginResetModel();
    m_entries.clear();
    endResetModel();
    emit loadErrorNotify(errorMessage);
  }
}

bool FileListModel::createFolder(const QString &dirName) {
  if (dirName.isEmpty())
    return false;

  QDir dir(m_currentPath);
  if (!dir.mkdir(dirName))
    return false;

  qint64 newDirMtime = 0;
  struct stat dirStat;
  QByteArray encodedPath = QFile::encodeName(m_currentPath);
  if (stat(encodedPath.constData(), &dirStat) == 0) {
    newDirMtime = dirStat.st_mtime;
  }

  ItemEntery newFolder;
  newFolder.name = QFile::encodeName(dirName);
  newFolder.isDir = true;
  newFolder.isSymLink = false;
  newFolder.size = 0;
  newFolder.modified = newDirMtime;

  m_rawEntries.append(newFolder);
  applySortAndFilter();

  if (CachedListing *cached = m_cache.object(m_currentPath)) {
    cached->entries = m_rawEntries;
    cached->dirMtime = newDirMtime;
  } else {
    m_cache.insert(m_currentPath, new CachedListing{m_rawEntries, newDirMtime});
  }

  return true;
}

void FileListModel::setSettings(const FileViewSettings &settings) {
  if (m_settings.sortBy == settings.sortBy &&
      m_settings.ascending == settings.ascending &&
      m_settings.foldersFirstSorting == settings.foldersFirstSorting &&
      m_settings.showHiddenFiles == settings.showHiddenFiles &&
      m_settings.showSymlinks == settings.showSymlinks) {
    m_settings = settings;
    return;
  }

  m_settings = settings;
  applySortAndFilter();
}

void FileListModel::applySortAndFilter() {
  beginResetModel();
  m_entries.clear();
  m_entries.reserve(m_rawEntries.size());

  for (const auto &entry : m_rawEntries) {
    if (!m_settings.showHiddenFiles && !entry.name.isEmpty() &&
        entry.name.at(0) == '.') {
      continue;
    }
    if (!m_settings.showSymlinks && entry.isSymLink) {
      continue;
    }
    m_entries.append(entry);
  }

  const FileViewSettings &s = m_settings;
  std::sort(m_entries.begin(), m_entries.end(),
            [&s](const ItemEntery &a, const ItemEntery &b) {
              return compareEntries(a, b, s);
            });

  endResetModel();
}
