#include "FileOperation.hpp"
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <qdir.h>
#include <qlogging.h>

FileOperationWorker::FileOperationWorker(const QStringList &sources,
                                         const QString &destination, bool isCut,
                                         QObject *parent)
    : QObject(parent), m_sources(sources), m_destination(destination),
      m_isCut(isCut) {}

void FileOperationWorker::resolveConflict(ConflictResponse action) {
  QMutexLocker locker(&m_mutex);
  m_ConflictResponse = action;
  m_waitCondition.wakeOne();
}

void FileOperationWorker::cancel() {
  QMutexLocker locker(&m_mutex);
  m_cancelRequested.store(true, std::memory_order_relaxed);
  m_waitCondition.wakeOne();
}

void FileOperationWorker::calculateTotalBytes(const QString &path) {
  QFileInfo info(path);
  if (!info.exists()) {
    return;
  }

  if (info.isDir()) {
    QDirIterator it(path, QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot,
                    QDirIterator::Subdirectories);
    while (it.hasNext()) {
      it.next();
      m_totalBytes += it.fileInfo().size();
    }
  } else {
    m_totalBytes += info.size();
  }
}

void FileOperationWorker::start() {
  for (const QString &src : m_sources) {
    calculateTotalBytes(src);
  }

  for (const QString &src : m_sources) {
    if (m_cancelRequested.load(std::memory_order_relaxed))
      break;

    QFileInfo info(src);
    QString dstPath = QDir(m_destination).filePath(info.fileName());

    processPath(src, dstPath);
  }

  emit finished();
}

void FileOperationWorker::processPath(const QString &srcPath,
                                      const QString &dstPath) {
  QFileInfo srcInfo(srcPath);
  if (!srcInfo.exists()) {
    emit operationFailed("Source does not exist: " + srcPath);
    return;
  }

  if (srcInfo.isDir()) {
    QString canonicalSrc = QDir(srcPath).canonicalPath();
    QString canonicalDst = QDir(dstPath).canonicalPath();
    if (!canonicalDst.isEmpty() && canonicalDst.startsWith(canonicalSrc)) {
      emit operationFailed("Cannot copy directory into itself: " + srcPath);
      return;
    }

    if (m_isCut && !QFile::exists(dstPath)) {
      if (QDir().rename(srcPath, dstPath)) {
        m_bytesCopied += srcInfo.size();
        emit progressChanged(m_bytesCopied, m_totalBytes, srcInfo.fileName());
        return;
      }
    }

    auto result = copyDirectoryRecursive(srcPath, dstPath);
    if (result.code != FsResult::Code::Success) {
      emit operationFailed(result.detail);
      return;
    }

    if (m_isCut && !m_cancelRequested.load(std::memory_order_relaxed)) {
      QDir().rmdir(srcPath);
    }
  } else {
    if (m_isCut && !QFile::exists(dstPath)) {
      if (QFile::rename(srcPath, dstPath)) {
        m_bytesCopied += srcInfo.size();
        emit progressChanged(m_bytesCopied, m_totalBytes, srcInfo.fileName());
        return;
      }
    }
    qDebug() << m_isCut << !QFile::exists(dstPath);

    auto result = copyFileChunked(srcPath, dstPath);
    if (result.code == FsResult::Code::Success) {
      if (m_isCut && !m_cancelRequested.load(std::memory_order_relaxed)) {
        QFile::remove(srcPath);
      }
    } else if (result.code == FsResult::Code::Skipped) {
      return;
    } else {
      emit operationFailed(result.detail);
      return;
    }
  }
}

bool FileOperationWorker::askUserConflict(const QString &fileName) {
  if (m_cancelRequested.load(std::memory_order_relaxed)) {
    return false;
  }

  emit conflictFound(fileName);

  m_mutex.lock();
  m_waitCondition.wait(&m_mutex);
  ConflictResponse response = m_ConflictResponse;
  m_mutex.unlock();

  if (m_cancelRequested.load(std::memory_order_relaxed) ||
      response == ConflictResponse::Cancel) {
    m_cancelRequested.store(true, std::memory_order_relaxed);
    return false;
  }

  return (response == ConflictResponse::Overwrite);
}

FsResult FileOperationWorker::copyFileChunked(const QString &srcPath,
                                              const QString &dstPath) {
  QFileInfo srcInfo(srcPath);
  if (!srcInfo.exists()) {
    return FsResult{FsResult::Code::NotFound,
                    "Source file not found: " + srcPath};
  }

  bool fileCollisionExists = QFile::exists(dstPath);
  if (fileCollisionExists) {
    bool shouldOverwrite = askUserConflict(srcInfo.fileName());
    if (!shouldOverwrite) {
      if (!m_cancelRequested.load(std::memory_order_relaxed)) {
        m_bytesCopied += srcInfo.size();
        emit progressChanged(m_bytesCopied, m_totalBytes, srcInfo.fileName());
      }
      return FsResult{FsResult::Code::Skipped};
    }
  }

  QFile srcFile(srcPath);
  if (!srcFile.open(QIODevice::ReadOnly)) {
    return FsResult{FsResult::Code::Io,
                    "Unable to open source file: " + srcPath};
  }

  QString tmpDstPath = dstPath + ".part";
  QFile dstFile(tmpDstPath);
  if (!dstFile.open(QIODevice::WriteOnly)) {
    return FsResult{FsResult::Code::Io,
                    "Unable to open temporary destination file: " + tmpDstPath};
  }

  constexpr qint64 chunkSize = 512 * 1024;
  QByteArray buffer;
  buffer.resize(chunkSize);

  while (!srcFile.atEnd() &&
         !m_cancelRequested.load(std::memory_order_relaxed)) {
    qint64 bytesRead = srcFile.read(buffer.data(), chunkSize);
    if (bytesRead <= 0)
      break;

    qint64 bytesWritten = dstFile.write(buffer.constData(), bytesRead);
    if (bytesWritten != bytesRead) {
      srcFile.close();
      dstFile.close();
      QFile::remove(tmpDstPath);
      return FsResult{FsResult::Code::Io,
                      "Disk write error or insufficient space at: " +
                          tmpDstPath};
    }

    m_bytesCopied += bytesRead;
    emit progressChanged(m_bytesCopied, m_totalBytes, srcInfo.fileName());
  }

  srcFile.close();
  dstFile.close();

  if (m_cancelRequested.load(std::memory_order_relaxed)) {
    QFile::remove(tmpDstPath);
    return FsResult{FsResult::Code::Cancelled};
  }

  if (fileCollisionExists) {
    if (!QFile::remove(dstPath)) {
      QFile::remove(tmpDstPath);
      return FsResult{FsResult::Code::Io,
                      "Unable to clear conflicting file path target: " +
                          dstPath};
    }
  }

  if (!dstFile.rename(dstPath)) {
    QFile::remove(tmpDstPath);
    return FsResult{FsResult::Code::Io,
                    "Failed to finalize atomic file renaming transition: " +
                        dstPath};
  }

  QFile::setPermissions(dstPath, QFile::permissions(srcPath));
  return FsResult{FsResult::Code::Success};
}

FsResult FileOperationWorker::copyDirectoryRecursive(const QString &srcDir,
                                                     const QString &dstDir) {
  if (!QDir().mkpath(dstDir)) {
    return FsResult{FsResult::Code::Io,
                    "Failed to create directory at: " + dstDir};
  }

  QDir sourceDirectory(srcDir);
  QFileInfoList entries = sourceDirectory.entryInfoList(
      QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden);

  for (const QFileInfo &entry : entries) {
    if (m_cancelRequested.load(std::memory_order_relaxed)) {
      return FsResult{FsResult::Code::Cancelled};
    }

    QString newSrc = entry.filePath();
    QString newDst = QDir(dstDir).filePath(entry.fileName());

    if (entry.isDir()) {
      auto result = copyDirectoryRecursive(newSrc, newDst);
      if (result.code != FsResult::Code::Success &&
          result.code != FsResult::Code::Skipped) {
        return result;
      }
      if (m_isCut && result.code == FsResult::Code::Success &&
          !m_cancelRequested.load(std::memory_order_relaxed)) {
        QDir().rmdir(newSrc);
      }
    } else {
      auto result = copyFileChunked(newSrc, newDst);
      if (result.code == FsResult::Code::Success) {
        if (m_isCut && !m_cancelRequested.load(std::memory_order_relaxed)) {
          QFile::remove(newSrc);
        }
      } else if (result.code == FsResult::Code::Skipped) {
        continue;
      } else {
        return result;
      }
    }
  }

  return FsResult{FsResult::Code::Success};
}
