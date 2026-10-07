#include "DirLoader.hpp"
#include "files/type.hpp"
#include <atomic>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <qfileinfo.h>
#include <qhashfunctions.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qstringview.h>
#include <qtimezone.h>
#include <qtypes.h>
#include <qvariant.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

DirectoryLoader::DirectoryLoader(QObject *parent) : QObject(parent) {}

void DirectoryLoader::loadDirectory(const QString &path, bool batchLoading) {
  m_cancelRequested.store(false, std::memory_order_relaxed);
  emit loadStarted(path);
  QVector<ItemEntery> batch;
  batch.reserve(128);

  const QByteArray localPath = QFile::encodeName(path);
  DIR *dir = opendir(localPath.constData());
  if (dir == NULL) {
    emit loadError(path, errno, QString::fromLocal8Bit(strerror(errno)));
    return;
  }
  int dfd = dirfd(dir);
  if (dfd == -1) {
    emit loadError(path, errno, QString::fromLocal8Bit(strerror(errno)));
    closedir(dir);
    return;
  }
  qint64 directoryModified = 0;
  struct stat dirStat;
  if (fstat(dfd, &dirStat) == 0) {
    directoryModified = dirStat.st_mtime;
  }
  bool readError = false;
  int readErrno = 0;
  auto readNext = [&]() -> struct dirent * {
    errno = 0;
    struct dirent *e = readdir(dir);
    if (!e && errno != 0) {
      readError = true;
      readErrno = errno;
    }
    return e;
  };

  while (struct dirent *entry = readNext()) {
    if (m_cancelRequested.load(std::memory_order_relaxed))
      break;
    // skipping the . and .. dirs
    if (entry->d_name[0] == '.' &&
        (entry->d_name[1] == '\0' ||
         (entry->d_name[1] == '.' && entry->d_name[2] == '\0'))) {
      continue;
    }

    ItemEntery file_info;
    file_info.name = QByteArray(entry->d_name);

    struct stat st;
    if (fstatat(dfd, entry->d_name, &st, AT_SYMLINK_NOFOLLOW) == 0) {
      file_info.isDir = S_ISDIR(st.st_mode);
      file_info.isSymLink = S_ISLNK(st.st_mode);
      if (file_info.isDir || file_info.isSymLink) {
        file_info.size = 0;
      } else {
        file_info.size = st.st_size;
      }
      file_info.modified = st.st_mtime;
    } else {
      if (errno == ENOENT) {
        continue;
      }
      file_info.isInaccessible = true;
      file_info.metadataError = errno;
      file_info.isDir = (entry->d_type == DT_DIR);
    }
    batch.append(std::move(file_info));
    if (batchLoading && batch.size() >= 100) {
      emit entriesReady(batch);
      batch.clear();
    }
  }
  if (readError) {
    emit loadError(path, readErrno,
                   QString::fromLocal8Bit(strerror(readErrno)));
  }
  closedir(dir);
  if (m_cancelRequested.load(std::memory_order_relaxed)) {
    return;
  }
  if (!batch.isEmpty()) {
    emit entriesReady(batch);
    batch.clear();
  }
  emit loadFinished(path, directoryModified);
  return;
}

void DirectoryLoader::revalidateInBackground(const QString &path,
                                             qint64 cachedMtime) {
  m_cancelRequested.store(false, std::memory_order_relaxed);

  const QByteArray localPath = QFile::encodeName(path);
  struct stat st;
  if (stat(localPath.constData(), &st) != 0) {
    loadDirectory(path);
    return;
  }
  if (st.st_mtime == cachedMtime) {
    return;
  }
  // on changed
  loadDirectory(path);
}
void DirectoryLoader::cancel() {
  m_cancelRequested.store(true, std::memory_order_relaxed);
}
