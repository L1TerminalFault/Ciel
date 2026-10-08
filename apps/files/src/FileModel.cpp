#include "FileModel.hpp"
#include "TabManager.hpp"
#include "files/DirCache.hpp"
#include "files/DirLoader.hpp"
#include <algorithm>
#include <qdir.h>
#include <qfileinfo.h>
#include <sys/stat.h>

static FileListModel *s_fileListModelInstance = nullptr;

FileListModel *FileListModel::instance() { return s_fileListModelInstance; }

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

  ensureTabManagerConnected();

  m_workerThread.start();
  s_fileListModelInstance = this;
}

FileListModel::~FileListModel() {
  if (m_loader) {
    m_loader->cancel();
  }
  m_workerThread.quit();
  m_workerThread.wait();
  delete m_loader;
}

void FileListModel::ensureTabManagerConnected() {
  if (m_tabManagerConnected)
    return;
  if (auto *tm = TabManager::instance()) {
    connect(tm, &TabManager::currentTabIdChanged, this,
            &FileListModel::onCurrentTabChanged);
    m_tabManagerConnected = true;
  }
}

void FileListModel::syncWithTab() {
  ensureTabManagerConnected();
  onCurrentTabChanged();
}

void FileListModel::onCurrentTabChanged() {
  auto *tm = TabManager::instance();
  if (!tm)
    return;

  int idx = tm->currentIndex();
  if (idx < 0)
    return;

  setSettings(tm->currentSettings());

  QString path = tm->currentPath();
  if (path != m_currentPath) {
    setPath(path);
  } else {
    if (!m_visibleIndices.isEmpty()) {
      emit dataChanged(index(0), index(m_visibleIndices.size() - 1),
                       {SelectedRole});
    }
  }
}

void FileListModel::setPath(const QString &path) {
  if (path.isEmpty() || path == m_currentPath) {
    return;
  }
  ensureTabManagerConnected();
  m_loader->cancel();
  m_currentPath = path;
  m_focusedRow = -1;
  emit focusedRowChanged();

  if (CachedListing *cached = m_cache.object(path)) {
    m_items = cached->entries;
    applySortAndFilter();
    QMetaObject::invokeMethod(m_loader,
                              &DirectoryLoader::revalidateInBackground,
                              Qt::QueuedConnection, path, cached->dirMtime);
  } else {
    m_items.clear();
    beginResetModel();
    m_visibleIndices.clear();
    endResetModel();

    QMetaObject::invokeMethod(m_loader, &DirectoryLoader::loadDirectory,
                              Qt::QueuedConnection, path, m_batchLoading);
  }
}

int FileListModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;
  return m_visibleIndices.size();
}

QHash<int, QByteArray> FileListModel::roleNames() const {
  return {{NameRole, "name"}, {IsDirRole, "isDir"},
          {SizeRole, "size"}, {ModifiedRole, "modified"},
          {IconRole, "icon"}, {SelectedRole, "selected"}};
}

QVariant FileListModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() < 0 ||
      index.row() >= m_visibleIndices.size())
    return QVariant();

  const ItemEntery &entry = m_items.at(m_visibleIndices.at(index.row()));

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

  case SelectedRole:
    if (auto *tm = TabManager::instance()) {
      return tm->isRowSelected(index.row());
    }
    return false;

  default:
    return QVariant();
  }
}

void FileListModel::onLoadStarted(const QString &path) {
  if (path == m_currentPath) {
    m_focusedRow = -1;
    emit focusedRowChanged();
    m_items.clear();
    beginResetModel();
    m_visibleIndices.clear();
    endResetModel();
  }
}

void FileListModel::onEntriesReady(const QVector<ItemEntery> &batch) {
  if (batch.isEmpty())
    return;

  m_items.append(batch);
}

void FileListModel::onLoadFinished(const QString &path, qint64 modifiedTime) {
  if (path == m_currentPath) {
    applySortAndFilter();
    m_cache.insert(path, new CachedListing{m_items, modifiedTime});
  }
}

void FileListModel::onLoadError(const QString &path, int errorCode,
                                const QString &errorMessage) {
  if (path == m_currentPath) {
    m_focusedRow = -1;
    emit focusedRowChanged();
    m_items.clear();
    beginResetModel();
    m_visibleIndices.clear();
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

  m_items.append(newFolder);
  applySortAndFilter();

  if (CachedListing *cached = m_cache.object(m_currentPath)) {
    cached->entries = m_items;
    cached->dirMtime = newDirMtime;
  } else {
    m_cache.insert(m_currentPath, new CachedListing{m_items, newDirMtime});
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
  m_visibleIndices.clear();
  m_visibleIndices.reserve(m_items.size());

  for (int i = 0; i < m_items.size(); ++i) {
    const auto &entry = m_items.at(i);
    if (!m_settings.showHiddenFiles && !entry.name.isEmpty() &&
        entry.name.at(0) == '.') {
      continue;
    }
    if (!m_settings.showSymlinks && entry.isSymLink) {
      continue;
    }
    m_visibleIndices.append(i);
  }

  const FileViewSettings &s = m_settings;
  const auto &items = m_items;
  std::sort(m_visibleIndices.begin(), m_visibleIndices.end(),
            [&items, &s](int a, int b) {
              return compareEntries(items.at(a), items.at(b), s);
            });

  endResetModel();
}

void FileListModel::handleSelection(int row, Qt::KeyboardModifiers modifiers) {
  if (row < 0 || row >= m_visibleIndices.size())
    return;

  auto *tm = TabManager::instance();
  if (!tm)
    return;

  m_focusedRow = row;
  emit focusedRowChanged();

  if (modifiers & Qt::ControlModifier) {
    tm->toggleSelection(row);
    QModelIndex idx = index(row);
    emit dataChanged(idx, idx, {SelectedRole});
  } else if ((modifiers & Qt::ShiftModifier) && tm->currentTabAnchor() != -1) {
    int anchor = tm->currentTabAnchor();
    const auto prevSelection = tm->currentTabSelection();
    int start = std::min(anchor, row);
    int end = std::max(anchor, row);

    tm->selectRange(start, end);

    int minRow = start;
    int maxRow = end;
    for (int prev : prevSelection) {
      minRow = std::min(minRow, prev);
      maxRow = std::max(maxRow, prev);
    }
    emit dataChanged(index(minRow), index(maxRow), {SelectedRole});
  } else {
    const auto prevSelection = tm->currentTabSelection();
    tm->selectSingle(row);

    for (int prevRow : prevSelection) {
      if (prevRow != row && prevRow < m_visibleIndices.size()) {
        QModelIndex idx = index(prevRow);
        emit dataChanged(idx, idx, {SelectedRole});
      }
    }
    QModelIndex idx = index(row);
    emit dataChanged(idx, idx, {SelectedRole});
  }
}

void FileListModel::clearSelection() {
  auto *tm = TabManager::instance();
  if (!tm)
    return;

  const auto prevSelection = tm->currentTabSelection();
  if (prevSelection.isEmpty())
    return;

  tm->clearSelection();
  for (int row : prevSelection) {
    if (row < m_visibleIndices.size()) {
      QModelIndex idx = index(row);
      emit dataChanged(idx, idx, {SelectedRole});
    }
  }
}

void FileListModel::selectAll() {
  if (m_visibleIndices.isEmpty())
    return;

  auto *tm = TabManager::instance();
  if (!tm)
    return;

  tm->selectAll(m_visibleIndices.size());
  emit dataChanged(index(0), index(m_visibleIndices.size() - 1),
                   {SelectedRole});
}

void FileListModel::navigate(int targetRow, int modifiers) {
  if (m_visibleIndices.isEmpty())
    return;

  if (m_focusedRow == -1) {
    targetRow = 0;
  }

  if (targetRow < 0 || targetRow >= m_visibleIndices.size())
    return;

  auto *tm = TabManager::instance();
  if (!tm)
    return;

  m_focusedRow = targetRow;
  emit focusedRowChanged();

  auto mods = Qt::KeyboardModifiers(modifiers);
  if (mods & Qt::ControlModifier) {
    return;
  }

  if (mods & Qt::ShiftModifier) {
    int anchor = tm->currentTabAnchor();
    if (anchor == -1)
      anchor = 0;
    int start = std::min(anchor, targetRow);
    int end = std::max(anchor, targetRow);
    tm->selectRange(start, end);
    emit dataChanged(index(start), index(end), {SelectedRole});
  } else {
    const auto prevSelection = tm->currentTabSelection();
    tm->selectSingle(targetRow);

    for (int prevRow : prevSelection) {
      if (prevRow != targetRow && prevRow < m_visibleIndices.size()) {
        QModelIndex idx = index(prevRow);
        emit dataChanged(idx, idx, {SelectedRole});
      }
    }
    QModelIndex idx = index(targetRow);
    emit dataChanged(idx, idx, {SelectedRole});
  }
  tm->selectSingle(targetRow);
  QModelIndex idx = index(targetRow);
  emit dataChanged(idx, idx, {SelectedRole});
}

int FileListModel::findNextByPrefix(const QString &prefix) {
  if (prefix.isEmpty() || m_visibleIndices.isEmpty())
    return -1;

  QByteArray p = prefix.toUtf8();
  int n = m_visibleIndices.size();

  for (int i = 1; i <= n; ++i) {
    int idx = (m_focusedRow + i) % n;
    int rawIdx = m_visibleIndices.at(idx);
    if (m_items.at(rawIdx).name.startsWith(p)) {
      return idx;
    }
  }

  return -1;
}

QStringList FileListModel::selectedPaths() const {
  QStringList paths;
  auto *tm = TabManager::instance();
  if (!tm)
    return paths;

  const auto &selection = tm->currentTabSelection();
  paths.reserve(selection.size());
  QDir dir(m_currentPath);

  for (int row : selection) {
    if (row >= 0 && row < m_visibleIndices.size()) {
      int rawIdx = m_visibleIndices.at(row);
      QString name = QFile::decodeName(m_items.at(rawIdx).name);
      paths.append(dir.filePath(name));
    }
  }

  return paths;
}

QStringList FileListModel::selectedNames() const {
  QStringList names;
  auto *tm = TabManager::instance();
  if (!tm)
    return names;

  const auto &selection = tm->currentTabSelection();
  names.reserve(selection.size());

  for (int row : selection) {
    if (row >= 0 && row < m_visibleIndices.size()) {
      int rawIdx = m_visibleIndices.at(row);
      names.append(QFile::decodeName(m_items.at(rawIdx).name));
    }
  }

  return names;
}
void FileListModel::refresh() {
  m_cache.remove(m_currentPath);
  m_items.clear();
  beginResetModel();
  m_visibleIndices.clear();
  endResetModel();

  QMetaObject::invokeMethod(m_loader, &DirectoryLoader::loadDirectory,
                            Qt::QueuedConnection, m_currentPath,
                            m_batchLoading);
}
