#pragma once

#include <QAbstractListModel>
#include <QDir>
#include <QObject>
#include <QString>
#include <QUrl>
#include <QVariantList>
#include <QVariantMap>
#include <QtQml/qqmlregistration.h>

struct DownloadItemRecord {
  QString id;
  QString filename;
  QString destination;
  QString category{QStringLiteral("Other")};
  QString state{QStringLiteral("downloading")};
  qint64 bytesReceived{0};
  qint64 totalBytes{0};
  double speed{0.0};
  QList<double> segments;
  QString errorMessage;
};

class DownloadsModel : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT
  Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
  enum Roles {
    IdRole = Qt::UserRole + 1,
    FilenameRole,
    DestinationRole,
    CategoryRole,
    StateRole,
    BytesReceivedRole,
    TotalBytesRole,
    SpeedRole,
    SegmentsRole,
    ErrorMessageRole
  };

  explicit DownloadsModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  int count() const;

  void addOrUpdateInitial(const QString &id, const QVariantMap &data);
  void onStarted(const QString &id, const QString &filename,
                 const QString &destination, qint64 totalBytes,
                 const QString &category);
  void onProgress(const QString &id, qint64 bytesReceived, qint64 totalBytes,
                  double speed, const QList<double> &segments);
  void onFinished(const QString &id, const QString &destination,
                  const QString &category);
  void onFailed(const QString &id, const QString &error);
  void setState(const QString &id, const QString &state);
  void removeById(const QString &id);
  void clearCompleted();

signals:
  void countChanged();

private:
  int findIndex(const QString &id) const;
  QList<DownloadItemRecord> m_records;
};

class BrowserConfig : public QObject {
  Q_OBJECT
  QML_ELEMENT
  Q_PROPERTY(
      QUrl newTabUrl READ newTabUrl WRITE setNewTabUrl NOTIFY newTabUrlChanged)
  Q_PROPERTY(QUrl homeUrl READ homeUrl WRITE setHomeUrl NOTIFY homeUrlChanged)
  Q_PROPERTY(QString searchTemplate READ searchTemplate WRITE setSearchTemplate
                 NOTIFY searchTemplateChanged)
  Q_PROPERTY(DownloadsModel *downloadsModel READ downloadsModel CONSTANT)

public:
  explicit BrowserConfig(QObject *parent = nullptr);

  QUrl newTabUrl() const;
  void setNewTabUrl(const QUrl &url);

  QUrl homeUrl() const;
  void setHomeUrl(const QUrl &url);

  QString searchTemplate() const;
  void setSearchTemplate(const QString &tmpl);

  DownloadsModel *downloadsModel() const;

  Q_INVOKABLE QUrl resolveQueryOrUrl(const QString &input) const;

  Q_INVOKABLE void startDownload(const QUrl &url,
                                 const QString &filename = QString(),
                                 const QString &mimeType = QString());
  Q_INVOKABLE void pauseDownload(const QString &id);
  Q_INVOKABLE void resumeDownload(const QString &id);
  Q_INVOKABLE void cancelDownload(const QString &id);
  Q_INVOKABLE void retryDownload(const QString &id);
  Q_INVOKABLE void openFolder(const QString &path);
  Q_INVOKABLE void clearFinished();

signals:
  void newTabUrlChanged();
  void homeUrlChanged();
  void searchTemplateChanged();

private slots:
  void onDownloadStarted(const QString &id, const QString &filename,
                         const QString &destination, qint64 total_bytes,
                         const QString &category);
  void onDownloadProgress(const QString &id, qint64 bytes_received,
                          qint64 total_bytes, double speed_bytes_sec,
                          const QList<double> &segments);
  void onDownloadFinished(const QString &id, const QString &destination,
                          const QString &category);
  void onDownloadFailed(const QString &id, const QString &error_message);

private:
  void fetchInitialDownloads();

  QUrl m_newTabUrl{QStringLiteral("about:blank")};
  QUrl m_homeUrl{QStringLiteral("about:blank")};
  QString m_searchTemplate{
      QStringLiteral("https://www.google.com/search?q=%s")};
  DownloadsModel *m_model{nullptr};
};
