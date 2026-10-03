#include "DownloadService.hpp"
#include "NotificationService.hpp"

#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QNetworkRequest>
#include <QStandardPaths>
#include <algorithm>
#include <fcntl.h>
#include <unistd.h>

static const auto kBrowserUserAgent = QStringLiteral(
    "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) "
    "Chrome/130.0.0.0 Safari/537.36");

SegmentWorker::SegmentWorker(const QUrl &url, const QString &filePath,
                             qint64 startByte, qint64 endByte, int segmentIndex,
                             QNetworkAccessManager *nam, QObject *parent)
    : QObject(parent), m_url(url), m_filePath(filePath), m_startByte(startByte),
      m_endByte(endByte), m_index(segmentIndex), m_nam(nam) {}

SegmentWorker::~SegmentWorker() {
  abort();
  closeFile();
}

void SegmentWorker::closeFile() {
  if (m_file) {
    if (m_file->isOpen()) {
      m_file->flush();
      m_file->close();
    }
    m_file.reset();
  }
}

void SegmentWorker::start() { attemptRequest(); }

void SegmentWorker::attemptRequest() {
  if (m_reply) {
    m_reply->deleteLater();
    m_reply = nullptr;
  }

  qint64 currentOffset = m_startByte + m_received;
  if (currentOffset > m_endByte) {
    m_finished = true;
    closeFile();
    emit finished(m_index);
    return;
  }

  if (!m_file) {
    m_file = std::make_unique<QFile>(m_filePath);
    if (!m_file->open(QIODevice::ReadWrite | QIODevice::Unbuffered)) {
      emit failed(m_index,
                  QStringLiteral("Cannot open target file for segment"));
      return;
    }
  }

  QNetworkRequest request(m_url);
  request.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                       QNetworkRequest::NoLessSafeRedirectPolicy);
  request.setAttribute(QNetworkRequest::Http2AllowedAttribute, false);
  request.setHeader(QNetworkRequest::UserAgentHeader, kBrowserUserAgent);
  request.setRawHeader("Accept", "*/*");
  request.setRawHeader("Connection", "keep-alive");

  const QString rangeHeader =
      QStringLiteral("bytes=%1-%2").arg(currentOffset).arg(m_endByte);
  request.setRawHeader("Range", rangeHeader.toUtf8());

  m_reply = m_nam->get(request);
  m_reply->setReadBufferSize(256 * 1024);

  connect(m_reply, &QNetworkReply::readyRead, this, [this]() {
    if (!m_file || !m_reply)
      return;

    const QByteArray data = m_reply->readAll();
    if (data.isEmpty())
      return;

    qint64 writePos = m_startByte + m_received;
    if (m_file->seek(writePos)) {
      qint64 bytesWritten = m_file->write(data);
      if (bytesWritten > 0) {
        m_received += bytesWritten;
        emit progressUpdated();
      }
    }
  });

  connect(m_reply, &QNetworkReply::finished, this, [this]() {
    if (!m_reply)
      return;

    if (m_reply->error() == QNetworkReply::NoError) {
      m_finished = true;
      closeFile();
      emit finished(m_index);
    } else if (m_retries < 3) {
      ++m_retries;
      QTimer::singleShot(800 * m_retries, this, &SegmentWorker::attemptRequest);
    } else {
      closeFile();
      emit failed(m_index, m_reply->errorString());
    }
  });
}

void SegmentWorker::abort() {
  if (m_reply) {
    m_reply->abort();
    m_reply->deleteLater();
    m_reply = nullptr;
  }
  closeFile();
}

qint64 SegmentWorker::bytesDownloaded() const { return m_received; }

qint64 SegmentWorker::totalSegmentBytes() const {
  return (m_endByte - m_startByte) + 1;
}

double SegmentWorker::progress() const {
  qint64 total = totalSegmentBytes();
  return total > 0
             ? std::clamp(static_cast<double>(m_received) / total, 0.0, 1.0)
             : 0.0;
}

bool SegmentWorker::isFinished() const { return m_finished; }

DownloadTask::DownloadTask(const QString &id, const QUrl &url,
                           const QString &destination,
                           QNetworkAccessManager *nam, QObject *parent)
    : QObject(parent), m_id(id), m_url(url), m_destination(destination),
      m_nam(nam) {
  m_filename = QFileInfo(m_destination).fileName();
}

void DownloadTask::start() {
  m_state = State::Downloading;
  m_speedTimer.restart();
  m_lastBytesReceived = 0;
  startSingleStreamDownload();
}

bool DownloadTask::preallocateFile(qint64 size) {
  QDir().mkpath(QFileInfo(m_destination).absolutePath());
  int fd = ::open(m_destination.toUtf8().constData(), O_CREAT | O_RDWR, 0644);
  if (fd == -1)
    return false;

  int res = ::posix_fallocate(fd, 0, size);
  ::close(fd);
  return res == 0;
}

void DownloadTask::startSegmentedDownload() {
  m_state = State::Downloading;
  m_speedTimer.restart();
  m_lastBytesReceived = 0;
  qint64 segmentSize = m_totalBytes / m_segmentCount;

  for (int i = 0; i < m_segmentCount; ++i) {
    qint64 start = i * segmentSize;
    qint64 end = (i == m_segmentCount - 1) ? (m_totalBytes - 1)
                                           : (start + segmentSize - 1);

    auto *worker =
        new SegmentWorker(m_url, m_destination, start, end, i, m_nam, this);
    connect(worker, &SegmentWorker::finished, this, [this](int) {
      bool allDone = true;
      for (const auto *w : m_workers) {
        if (!w->isFinished()) {
          allDone = false;
          break;
        }
      }
      if (allDone) {
        m_state = State::Finished;
        emit taskFinished(m_id);
      }
    });
    connect(worker, &SegmentWorker::failed, this,
            [this](int, const QString &err) {
              m_state = State::Failed;
              emit taskFailed(m_id, err);
            });

    worker->start();
    m_workers.append(worker);
  }
}

void DownloadTask::startSingleStreamDownload() {
  m_state = State::Downloading;
  QDir().mkpath(QFileInfo(m_destination).absolutePath());
  m_singleFile = std::make_unique<QFile>(m_destination);
  if (!m_singleFile->open(QIODevice::WriteOnly)) {
    emit taskFailed(m_id, QStringLiteral("Cannot write target file"));
    return;
  }

  QNetworkRequest request(m_url);
  request.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                       QNetworkRequest::NoLessSafeRedirectPolicy);
  request.setHeader(QNetworkRequest::UserAgentHeader, kBrowserUserAgent);
  request.setRawHeader("Accept", "*/*");
  request.setRawHeader("Connection", "keep-alive");

  m_singleStreamReply = m_nam->get(request);
  m_singleStreamReply->setReadBufferSize(512 * 1024);

  connect(m_singleStreamReply, &QNetworkReply::metaDataChanged, this, [this]() {
    if (!m_singleStreamReply)
      return;

    qint64 len =
        m_singleStreamReply->header(QNetworkRequest::ContentLengthHeader)
            .toLongLong();
    if (len > 0)
      m_totalBytes = len;

    const QByteArray acceptRanges =
        m_singleStreamReply->rawHeader("Accept-Ranges");
    bool supportsRanges = acceptRanges.contains("bytes");

    if (supportsRanges && m_totalBytes > (8 * 1024 * 1024) &&
        m_workers.isEmpty()) {
      m_singleStreamReply->abort();
      m_singleStreamReply->deleteLater();
      m_singleStreamReply = nullptr;

      if (m_singleFile) {
        m_singleFile->close();
        m_singleFile.reset();
      }
      m_bytesReceived = 0;

      if (preallocateFile(m_totalBytes)) {
        startSegmentedDownload();
      } else {
        startSingleStreamDownload();
      }
    }
  });

  connect(m_singleStreamReply, &QNetworkReply::readyRead, this, [this]() {
    if (m_singleFile && m_singleStreamReply) {
      const QByteArray data = m_singleStreamReply->readAll();
      m_singleFile->write(data);
      m_bytesReceived += data.size();
    }
  });

  connect(m_singleStreamReply, &QNetworkReply::finished, this, [this]() {
    if (m_state != State::Downloading || !m_workers.isEmpty())
      return;

    if (m_singleFile) {
      m_singleFile->flush();
      m_singleFile->close();
    }
    if (m_singleStreamReply &&
        m_singleStreamReply->error() == QNetworkReply::NoError) {
      m_state = State::Finished;
      emit taskFinished(m_id);
    } else if (m_singleStreamReply &&
               m_singleStreamReply->error() !=
                   QNetworkReply::OperationCanceledError) {
      m_state = State::Failed;
      emit taskFailed(m_id, m_singleStreamReply->errorString());
    }
  });
}

void DownloadTask::pause() {
  m_state = State::Paused;
  for (auto *w : m_workers)
    w->abort();
  if (m_singleStreamReply)
    m_singleStreamReply->abort();
}

void DownloadTask::resume() {
  if (m_state == State::Paused) {
    if (!m_workers.isEmpty()) {
      m_state = State::Downloading;
      m_speedTimer.restart();
      for (auto *w : m_workers) {
        if (!w->isFinished())
          w->start();
      }
    } else {
      start();
    }
  }
}

void DownloadTask::cancel() {
  pause();
  QFile::remove(m_destination);
}

QString DownloadTask::id() const { return m_id; }
QString DownloadTask::filename() const { return m_filename; }
QString DownloadTask::destination() const { return m_destination; }
DownloadTask::State DownloadTask::state() const { return m_state; }
QString DownloadTask::category() const { return m_category; }
void DownloadTask::setCategory(const QString &category) {
  m_category = category;
}

qint64 DownloadTask::bytesReceived() const {
  if (!m_workers.isEmpty()) {
    qint64 total = 0;
    for (const auto *w : m_workers)
      total += w->bytesDownloaded();
    return total;
  }
  return m_bytesReceived;
}

qint64 DownloadTask::totalBytes() const { return m_totalBytes; }

double DownloadTask::speed() const { return m_speed; }

void DownloadTask::updateSpeed() {
  qint64 elapsedMs = m_speedTimer.restart();
  if (elapsedMs <= 0)
    return;

  qint64 current = bytesReceived();
  qint64 delta = current - m_lastBytesReceived;
  m_lastBytesReceived = current;

  double instantSpeed =
      std::max(0.0, (static_cast<double>(delta) * 1000.0) / elapsedMs);

  if (m_speed <= 0.0) {
    m_speed = instantSpeed;
  } else {
    m_speed = (m_speed * 0.90) + (instantSpeed * 0.10);
  }

  if (m_speed < 1.0)
    m_speed = 0.0;
}

QList<double> DownloadTask::segmentProgresses() const {
  QList<double> result;
  for (const auto *w : m_workers)
    result.push_back(w->progress());
  return result;
}

DownloadService::DownloadService(NotificationService *notifService,
                                 QObject *parent)
    : QObject(parent), m_notifService(notifService) {
  m_downloadsRoot =
      QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
  if (m_downloadsRoot.isEmpty())
    m_downloadsRoot = QDir::homePath() + QStringLiteral("/Downloads");

  initDefaultCategories();

  connect(&m_tickTimer, &QTimer::timeout, this, &DownloadService::onTick);
  m_tickTimer.start(100);
}

void DownloadService::initDefaultCategories() {
  m_categories = {
      {QStringLiteral("Images"),
       QStringLiteral("image"),
       QStringLiteral("Images"),
       {QStringLiteral("image/*")},
       {QStringLiteral("png"), QStringLiteral("jpg"), QStringLiteral("jpeg"),
        QStringLiteral("webp"), QStringLiteral("svg"), QStringLiteral("gif")}},
      {QStringLiteral("Documents"),
       QStringLiteral("file-text"),
       QStringLiteral("Documents"),
       {QStringLiteral("application/pdf"), QStringLiteral("text/plain"),
        QStringLiteral("text/markdown")},
       {QStringLiteral("pdf"), QStringLiteral("docx"), QStringLiteral("xlsx"),
        QStringLiteral("txt"), QStringLiteral("md")}},
      {QStringLiteral("Archives"),
       QStringLiteral("file-zip"),
       QStringLiteral("Archives"),
       {QStringLiteral("application/zip"), QStringLiteral("application/x-tar"),
        QStringLiteral("application/x-gzip"),
        QStringLiteral("application/zstd")},
       {QStringLiteral("zip"), QStringLiteral("tar"), QStringLiteral("gz"),
        QStringLiteral("7z"), QStringLiteral("rar"), QStringLiteral("zst")}},
      {QStringLiteral("Software"),
       QStringLiteral("package"),
       QStringLiteral("Software"),
       {QStringLiteral("application/x-executable"),
        QStringLiteral("application/vnd.debian.binary-package"),
        QStringLiteral("application/x-appimage")},
       {QStringLiteral("appimage"), QStringLiteral("deb"),
        QStringLiteral("rpm"), QStringLiteral("pkg.tar.zst"),
        QStringLiteral("iso")}},
      {QStringLiteral("Audio"),
       QStringLiteral("speaker-high"),
       QStringLiteral("Audio"),
       {QStringLiteral("audio/*")},
       {QStringLiteral("mp3"), QStringLiteral("flac"), QStringLiteral("wav"),
        QStringLiteral("ogg"), QStringLiteral("m4a")}},
      {QStringLiteral("Video"),
       QStringLiteral("video"),
       QStringLiteral("Video"),
       {QStringLiteral("video/*")},
       {QStringLiteral("mp4"), QStringLiteral("mkv"), QStringLiteral("webm"),
        QStringLiteral("mov")}},
      {QStringLiteral("Code"),
       QStringLiteral("code"),
       QStringLiteral("Code"),
       {QStringLiteral("text/x-c"), QStringLiteral("text/x-python"),
        QStringLiteral("application/json")},
       {QStringLiteral("json"), QStringLiteral("py"), QStringLiteral("js"),
        QStringLiteral("ts"), QStringLiteral("cpp"), QStringLiteral("rs"),
        QStringLiteral("qml")}}};
}

QString DownloadService::matchCategory(const QString &filename,
                                       const QString &mimeType) const {
  const QString lowerMime = mimeType.toLower().trimmed();
  if (!lowerMime.isEmpty() &&
      lowerMime != QStringLiteral("application/octet-stream")) {
    for (const auto &cat : m_categories) {
      for (const auto &pattern : cat.mimePatterns) {
        if (pattern.endsWith(QLatin1Char('*'))) {
          if (lowerMime.startsWith(pattern.left(pattern.length() - 1)))
            return cat.name;
        } else if (lowerMime == pattern) {
          return cat.name;
        }
      }
    }
  }

  const QString cleanName = filename.toLower().trimmed();
  for (const auto &cat : m_categories) {
    for (const auto &ext : cat.extensions) {
      if (cleanName.endsWith(QStringLiteral(".") + ext))
        return cat.name;
    }
  }
  return QStringLiteral("Other");
}

QString DownloadService::deduplicatePath(const QString &targetDir,
                                         const QString &filename) const {
  const QFileInfo initialInfo(targetDir + QLatin1Char('/') + filename);
  if (!initialInfo.exists())
    return initialInfo.absoluteFilePath();

  QString baseName = initialInfo.completeBaseName();
  QString suffix = initialInfo.suffix();

  int counter = 1;
  while (true) {
    QString candidate =
        suffix.isEmpty() ? QStringLiteral("%1 (%2)").arg(baseName).arg(counter)
                         : QStringLiteral("%1 (%2).%3")
                               .arg(baseName)
                               .arg(counter)
                               .arg(suffix);

    QFileInfo check(targetDir + QLatin1Char('/') + candidate);
    if (!check.exists())
      return check.absoluteFilePath();
    ++counter;
  }
}

QString DownloadService::ResolveDestination(const QString &filename,
                                            const QString &mime_type) {
  const QString safeName = QFileInfo(filename).fileName();
  if (safeName.isEmpty())
    return m_downloadsRoot;

  const QString category = matchCategory(safeName, mime_type);
  const QString targetDir = m_downloadsRoot + QLatin1Char('/') + category;
  QDir().mkpath(targetDir);

  return deduplicatePath(targetDir, safeName);
}

QVariantMap DownloadService::GetCategoryInfo(const QString &filename,
                                             const QString &mime_type) {
  const QString category = matchCategory(filename, mime_type);
  QString icon = QStringLiteral("file");
  for (const auto &cat : m_categories) {
    if (cat.name == category) {
      icon = cat.icon;
      break;
    }
  }

  QVariantMap res;
  res.insert(QStringLiteral("category"), category);
  res.insert(QStringLiteral("icon"), icon);
  res.insert(QStringLiteral("folderPath"),
             m_downloadsRoot + QLatin1Char('/') + category);
  return res;
}

QString DownloadService::StartDownload(const QString &url,
                                       const QString &custom_destination) {
  QUrl parsedUrl(url);
  if (!parsedUrl.isValid())
    return QString();

  QString destination = custom_destination;
  if (destination.isEmpty()) {
    QString rawFileName = parsedUrl.fileName();
    if (rawFileName.isEmpty())
      rawFileName = QStringLiteral("download_%1")
                        .arg(QDateTime::currentMSecsSinceEpoch());
    destination = ResolveDestination(rawFileName, QString());
  }

  QString id = QStringLiteral("dl_%1").arg(m_nextTaskId++);
  auto *task = new DownloadTask(id, parsedUrl, destination, &m_nam, this);
  task->setCategory(matchCategory(task->filename(), QString()));

  connect(task, &DownloadTask::taskFinished, this,
          &DownloadService::onTaskFinished);
  connect(task, &DownloadTask::taskFailed, this,
          &DownloadService::onTaskFailed);

  QString filename = task->filename();
  QString category = task->category();

  task->start();
  m_tasks.insert(id, task);

  emit DownloadStarted(id, filename, destination, 0, category);
  return id;
}

void DownloadService::PauseDownload(const QString &download_id) {
  if (m_tasks.contains(download_id))
    m_tasks[download_id]->pause();
}

void DownloadService::ResumeDownload(const QString &download_id) {
  if (m_tasks.contains(download_id))
    m_tasks[download_id]->resume();
}

void DownloadService::CancelDownload(const QString &download_id) {
  if (m_tasks.contains(download_id)) {
    auto *task = m_tasks.take(download_id);
    task->cancel();
    delete task;
  }
}

QVariantMap DownloadService::GetActiveDownloads() {
  QVariantMap map;
  for (auto it = m_tasks.constBegin(); it != m_tasks.constEnd(); ++it) {
    const auto *task = it.value();
    QVariantMap entry;
    entry.insert(QStringLiteral("filename"), task->filename());
    entry.insert(QStringLiteral("destination"), task->destination());
    entry.insert(QStringLiteral("bytesReceived"), task->bytesReceived());
    entry.insert(QStringLiteral("totalBytes"), task->totalBytes());
    entry.insert(QStringLiteral("speed"), task->speed());
    entry.insert(QStringLiteral("category"), task->category());

    QVariantList segs;
    for (double p : task->segmentProgresses())
      segs.append(p);
    entry.insert(QStringLiteral("segments"), segs);

    map.insert(it.key(), entry);
  }
  return map;
}

void DownloadService::onTick() {
  for (auto it = m_tasks.constBegin(); it != m_tasks.constEnd(); ++it) {
    auto *task = it.value();
    if (task->state() == DownloadTask::State::Downloading) {
      task->updateSpeed();
      emit DownloadProgress(task->id(), task->bytesReceived(),
                            task->totalBytes(), task->speed(),
                            task->segmentProgresses());
    }
  }
}

void DownloadService::onTaskFinished(const QString &id) {
  if (!m_tasks.contains(id))
    return;

  auto *task = m_tasks.take(id);
  QString dest = task->destination();
  QString cat = task->category();

  emit DownloadFinished(id, dest, cat);

  if (m_notifService) {
    m_notifService->Notify(QStringLiteral("Downloads"), 0,
                           QStringLiteral("download-simple"),
                           QStringLiteral("Download Complete"),
                           QFileInfo(dest).fileName(), {}, {}, 4000);
  }

  delete task;
}

void DownloadService::onTaskFailed(const QString &id, const QString &error) {
  if (!m_tasks.contains(id))
    return;

  auto *task = m_tasks.take(id);
  emit DownloadFailed(id, error);

  if (m_notifService) {
    m_notifService->Notify(
        QStringLiteral("Downloads"), 0, QStringLiteral("download-simple"),
        QStringLiteral("Download Failed"), error, {}, {}, 5000);
  }

  delete task;
}
