#pragma once

#include <QMutex>
#include <QObject>
#include <QWaitCondition>
#include <atomic>
#include <qcontainerfwd.h>
#include <qobject.h>
#include <qtmetamacros.h>

struct FsResult {
  enum class Code {
    Success,
    Skipped,
    Cancelled,
    NotFound,
    PermissionDenied,
    Exists,
    NoSpace,
    Io
  } code;
  QString detail;
};

class FileOperationWorker : public QObject {
  Q_OBJECT
public:
  enum ConflictResponse { Skip = 0, Overwrite, Cancel };
  Q_ENUM(ConflictResponse)

  explicit FileOperationWorker(const QStringList &sources,
                               const QString &destination, bool isCut,
                               QObject *parent = nullptr);
  void resolveConflict(ConflictResponse action);
  void cancel();

public slots:
  void start();

signals:
  void progressChanged(qint64 bytesCopied, qint64 totalBytes,
                       const QString &currentFile);
  void conflictFound(const QString &fileName);
  void operationFailed(const QString &errorMsg);
  void finished();

private:
  QStringList m_sources;
  QString m_destination;
  bool m_isCut;

  qint64 m_totalBytes = 0;
  qint64 m_bytesCopied = 0;

  QMutex m_mutex;
  QWaitCondition m_waitCondition;
  ConflictResponse m_ConflictResponse = Skip;
  std::atomic_bool m_cancelRequested = false;

  void calculateTotalBytes(const QString &path);
  void processPath(const QString &srcPath, const QString &dstPath);
  [[nodiscard]] FsResult copyFileChunked(const QString &srcPath,
                                         const QString &dstPath);
  [[nodiscard]] FsResult copyDirectoryRecursive(const QString &srcDir,
                                                const QString &dstDir);
  bool askUserConflict(const QString &fileName);
};
