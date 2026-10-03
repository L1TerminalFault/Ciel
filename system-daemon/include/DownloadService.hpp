#pragma once

#include <QElapsedTimer>
#include <QFile>
#include <QList>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QUrl>
#include <QVariantMap>
#include <memory>

class NotificationService;

class SegmentWorker : public QObject {
  Q_OBJECT
public:
  explicit SegmentWorker(const QUrl &url, const QString &filePath,
                         qint64 startByte, qint64 endByte, int segmentIndex,
                         QNetworkAccessManager *nam, QObject *parent = nullptr);
  ~SegmentWorker() override;

  void start();
  void abort();

  qint64 bytesDownloaded() const;
  qint64 totalSegmentBytes() const;
  double progress() const;
  bool isFinished() const;

signals:
  void progressUpdated();
  void finished(int index);
  void failed(int index, const QString &error);

private:
  void attemptRequest();
  void closeFile();

  QUrl m_url;
  QString m_filePath;
  qint64 m_startByte;
  qint64 m_endByte;
  qint64 m_received{0};
  int m_index;
  int m_retries{0};
  bool m_finished{false};

  QNetworkAccessManager *m_nam;
  QNetworkReply *m_reply{nullptr};
  std::unique_ptr<QFile> m_file;
};

class DownloadTask : public QObject {
  Q_OBJECT
public:
  enum class State { Queued, Downloading, Paused, Finished, Failed };

  explicit DownloadTask(const QString &id, const QUrl &url,
                        const QString &destination, QNetworkAccessManager *nam,
                        QObject *parent = nullptr);

  void start();
  void pause();
  void resume();
  void cancel();

  QString id() const;
  QString filename() const;
  QString destination() const;
  State state() const;
  QString category() const;
  void setCategory(const QString &category);

  qint64 bytesReceived() const;
  qint64 totalBytes() const;
  double speed() const;
  void updateSpeed();
  QList<double> segmentProgresses() const;

signals:
  void taskFinished(const QString &id);
  void taskFailed(const QString &id, const QString &error);

private:
  bool preallocateFile(qint64 size);
  void startSegmentedDownload();
  void startSingleStreamDownload();

  QString m_id;
  QUrl m_url;
  QString m_destination;
  QString m_filename;
  QString m_category;
  State m_state{State::Queued};

  qint64 m_totalBytes{0};
  qint64 m_bytesReceived{0};
  qint64 m_lastBytesReceived{0};
  double m_speed{0.0};
  int m_segmentCount{6};

  QElapsedTimer m_speedTimer;
  QNetworkAccessManager *m_nam;
  QNetworkReply *m_singleStreamReply{nullptr};
  std::unique_ptr<QFile> m_singleFile;
  QList<SegmentWorker *> m_workers;
};

struct DownloadCategory {
  QString name;
  QString icon;
  QString folderName;
  QStringList mimePatterns;
  QStringList extensions;
};

class DownloadService : public QObject {
  Q_OBJECT
public:
  explicit DownloadService(NotificationService *notifService = nullptr,
                           QObject *parent = nullptr);

  Q_INVOKABLE QString ResolveDestination(const QString &filename,
                                         const QString &mime_type);
  Q_INVOKABLE QVariantMap GetCategoryInfo(const QString &filename,
                                          const QString &mime_type);
  Q_INVOKABLE QString StartDownload(const QString &url,
                                    const QString &custom_destination);
  Q_INVOKABLE void PauseDownload(const QString &download_id);
  Q_INVOKABLE void ResumeDownload(const QString &download_id);
  Q_INVOKABLE void CancelDownload(const QString &download_id);
  Q_INVOKABLE QVariantMap GetActiveDownloads();

signals:
  void DownloadStarted(const QString &download_id, const QString &filename,
                       const QString &destination, qint64 total_bytes,
                       const QString &category);
  void DownloadProgress(const QString &download_id, qint64 bytes_received,
                        qint64 total_bytes, double speed_bytes_sec,
                        const QList<double> &segments);
  void DownloadFinished(const QString &download_id, const QString &destination,
                        const QString &category);
  void DownloadFailed(const QString &download_id, const QString &error_message);

private slots:
  void onTick();
  void onTaskFinished(const QString &id);
  void onTaskFailed(const QString &id, const QString &error);

private:
  void initDefaultCategories();
  QString matchCategory(const QString &filename, const QString &mimeType) const;
  QString deduplicatePath(const QString &targetDir,
                          const QString &filename) const;

  QString m_downloadsRoot;
  QList<DownloadCategory> m_categories;
  QMap<QString, DownloadTask *> m_tasks;
  QNetworkAccessManager m_nam;
  QTimer m_tickTimer;
  NotificationService *m_notifService{nullptr};
  qint64 m_nextTaskId{1};
};
