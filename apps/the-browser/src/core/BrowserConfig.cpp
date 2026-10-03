#include "BrowserConfig.hpp"

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusMetaType>
#include <QDBusReply>
#include <QDesktopServices>
#include <QFileInfo>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QUrlQuery>

DownloadsModel::DownloadsModel(QObject *parent) : QAbstractListModel(parent) {}

int DownloadsModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;
  return m_records.size();
}

QVariant DownloadsModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() < 0 || index.row() >= m_records.size())
    return {};

  const auto &rec = m_records.at(index.row());
  switch (role) {
  case IdRole:
    return rec.id;
  case FilenameRole:
    return rec.filename;
  case DestinationRole:
    return rec.destination;
  case CategoryRole:
    return rec.category;
  case StateRole:
    return rec.state;
  case BytesReceivedRole:
    return rec.bytesReceived;
  case TotalBytesRole:
    return rec.totalBytes;
  case SpeedRole:
    return rec.speed;
  case SegmentsRole: {
    QVariantList list;
    for (double s : rec.segments)
      list.append(s);
    return list;
  }
  case ErrorMessageRole:
    return rec.errorMessage;
  default:
    return {};
  }
}

QHash<int, QByteArray> DownloadsModel::roleNames() const {
  return {{IdRole, "id"},
          {FilenameRole, "filename"},
          {DestinationRole, "destination"},
          {CategoryRole, "category"},
          {StateRole, "state"},
          {BytesReceivedRole, "bytesReceived"},
          {TotalBytesRole, "totalBytes"},
          {SpeedRole, "speed"},
          {SegmentsRole, "segments"},
          {ErrorMessageRole, "errorMessage"}};
}

int DownloadsModel::count() const { return m_records.size(); }

int DownloadsModel::findIndex(const QString &id) const {
  for (int i = 0; i < m_records.size(); ++i) {
    if (m_records.at(i).id == id)
      return i;
  }
  return -1;
}

void DownloadsModel::addOrUpdateInitial(const QString &id,
                                        const QVariantMap &data) {
  int idx = findIndex(id);
  if (idx >= 0)
    return;

  beginInsertRows(QModelIndex(), 0, 0);
  DownloadItemRecord rec;
  rec.id = id;
  rec.filename = data.value(QStringLiteral("filename")).toString();
  rec.destination = data.value(QStringLiteral("destination")).toString();
  rec.category = data.value(QStringLiteral("category")).toString();
  rec.bytesReceived = data.value(QStringLiteral("bytesReceived")).toLongLong();
  rec.totalBytes = data.value(QStringLiteral("totalBytes")).toLongLong();
  rec.speed = data.value(QStringLiteral("speed")).toDouble();
  rec.state = QStringLiteral("downloading");
  m_records.prepend(rec);
  endInsertRows();

  emit countChanged();
}

void DownloadsModel::onStarted(const QString &id, const QString &filename,
                               const QString &destination, qint64 totalBytes,
                               const QString &category) {
  int idx = findIndex(id);
  if (idx >= 0) {
    auto &rec = m_records[idx];
    rec.filename = filename;
    rec.destination = destination;
    rec.totalBytes = totalBytes;
    rec.category = category;
    rec.state = QStringLiteral("downloading");
    emit dataChanged(index(idx), index(idx));
    return;
  }

  beginInsertRows(QModelIndex(), 0, 0);
  DownloadItemRecord rec;
  rec.id = id;
  rec.filename = filename;
  rec.destination = destination;
  rec.totalBytes = totalBytes;
  rec.category = category;
  rec.state = QStringLiteral("downloading");
  m_records.prepend(rec);
  endInsertRows();

  emit countChanged();
}

void DownloadsModel::onProgress(const QString &id, qint64 bytesReceived,
                                qint64 totalBytes, double speed,
                                const QList<double> &segments) {
  int idx = findIndex(id);
  if (idx < 0) {
    onStarted(id, id, QString(), totalBytes, QStringLiteral("Other"));
    idx = 0;
  }

  auto &rec = m_records[idx];
  rec.bytesReceived = bytesReceived;
  if (totalBytes > 0)
    rec.totalBytes = totalBytes;
  rec.speed = speed;
  rec.segments = segments;

  emit dataChanged(
      index(idx), index(idx),
      {BytesReceivedRole, TotalBytesRole, SpeedRole, SegmentsRole});
}

void DownloadsModel::onFinished(const QString &id, const QString &destination,
                                const QString &category) {
  int idx = findIndex(id);
  if (idx < 0)
    return;

  auto &rec = m_records[idx];
  rec.destination = destination;
  rec.category = category;
  rec.state = QStringLiteral("finished");
  rec.speed = 0.0;

  emit dataChanged(index(idx), index(idx),
                   {StateRole, DestinationRole, CategoryRole, SpeedRole});
}

void DownloadsModel::onFailed(const QString &id, const QString &error) {
  int idx = findIndex(id);
  if (idx < 0)
    return;

  auto &rec = m_records[idx];
  rec.state = QStringLiteral("failed");
  rec.errorMessage = error;
  rec.speed = 0.0;

  emit dataChanged(index(idx), index(idx),
                   {StateRole, ErrorMessageRole, SpeedRole});
}

void DownloadsModel::setState(const QString &id, const QString &state) {
  int idx = findIndex(id);
  if (idx < 0)
    return;

  m_records[idx].state = state;
  if (state == QStringLiteral("paused"))
    m_records[idx].speed = 0.0;

  emit dataChanged(index(idx), index(idx), {StateRole, SpeedRole});
}

void DownloadsModel::removeById(const QString &id) {
  int idx = findIndex(id);
  if (idx < 0)
    return;

  beginRemoveRows(QModelIndex(), idx, idx);
  m_records.removeAt(idx);
  endRemoveRows();

  emit countChanged();
}

void DownloadsModel::clearCompleted() {
  for (int i = m_records.size() - 1; i >= 0; --i) {
    if (m_records.at(i).state == QStringLiteral("finished") ||
        m_records.at(i).state == QStringLiteral("failed")) {
      beginRemoveRows(QModelIndex(), i, i);
      m_records.removeAt(i);
      endRemoveRows();
    }
  }
  emit countChanged();
}

BrowserConfig::BrowserConfig(QObject *parent)
    : QObject(parent), m_model(new DownloadsModel(this)) {
  qRegisterMetaType<QList<double>>("QList<double>");
  qDBusRegisterMetaType<QList<double>>();

  QDBusConnection::sessionBus().connect(
      QStringLiteral("org.ciel.Downloads"),
      QStringLiteral("/org/ciel/Downloads"),
      QStringLiteral("org.ciel.Downloads"), QStringLiteral("DownloadStarted"),
      this,
      SLOT(onDownloadStarted(QString, QString, QString, qint64, QString)));

  QDBusConnection::sessionBus().connect(
      QStringLiteral("org.ciel.Downloads"),
      QStringLiteral("/org/ciel/Downloads"),
      QStringLiteral("org.ciel.Downloads"), QStringLiteral("DownloadProgress"),
      this,
      SLOT(onDownloadProgress(QString, qint64, qint64, double, QList<double>)));

  QDBusConnection::sessionBus().connect(
      QStringLiteral("org.ciel.Downloads"),
      QStringLiteral("/org/ciel/Downloads"),
      QStringLiteral("org.ciel.Downloads"), QStringLiteral("DownloadFinished"),
      this, SLOT(onDownloadFinished(QString, QString, QString)));

  QDBusConnection::sessionBus().connect(
      QStringLiteral("org.ciel.Downloads"),
      QStringLiteral("/org/ciel/Downloads"),
      QStringLiteral("org.ciel.Downloads"), QStringLiteral("DownloadFailed"),
      this, SLOT(onDownloadFailed(QString, QString)));

  fetchInitialDownloads();
}

QUrl BrowserConfig::newTabUrl() const { return m_newTabUrl; }

void BrowserConfig::setNewTabUrl(const QUrl &url) {
  if (m_newTabUrl != url) {
    m_newTabUrl = url;
    emit newTabUrlChanged();
  }
}

QUrl BrowserConfig::homeUrl() const { return m_homeUrl; }

void BrowserConfig::setHomeUrl(const QUrl &url) {
  if (m_homeUrl != url) {
    m_homeUrl = url;
    emit homeUrlChanged();
  }
}

QString BrowserConfig::searchTemplate() const { return m_searchTemplate; }

void BrowserConfig::setSearchTemplate(const QString &tmpl) {
  if (m_searchTemplate != tmpl) {
    m_searchTemplate = tmpl;
    emit searchTemplateChanged();
  }
}

DownloadsModel *BrowserConfig::downloadsModel() const { return m_model; }

QUrl BrowserConfig::resolveQueryOrUrl(const QString &input) const {
  const QString trimmed = input.trimmed();
  if (trimmed.isEmpty())
    return m_newTabUrl;

  static const QRegularExpression schemeRegex(
      QStringLiteral("^[a-zA-Z][a-zA-Z0-9+-.]*://"));
  if (schemeRegex.match(trimmed).hasMatch()) {
    return QUrl(trimmed);
  }

  static const QRegularExpression hostRegex(
      QStringLiteral("^([a-zA-Z0-9-]+\\.)+[a-zA-Z]{2,}(:\\d+)?(/.*)?$"));
  if (hostRegex.match(trimmed).hasMatch() ||
      trimmed.startsWith(QLatin1String("localhost"))) {
    return QUrl(QStringLiteral("https://") + trimmed);
  }

  QString queryUrl = m_searchTemplate;
  const QString encoded = QString::fromUtf8(QUrl::toPercentEncoding(trimmed));
  queryUrl.replace(QStringLiteral("%s"), encoded);
  return QUrl(queryUrl);
}

void BrowserConfig::fetchInitialDownloads() {
  QDBusInterface iface(QStringLiteral("org.ciel.Downloads"),
                       QStringLiteral("/org/ciel/Downloads"),
                       QStringLiteral("org.ciel.Downloads"),
                       QDBusConnection::sessionBus());
  if (!iface.isValid())
    return;

  QDBusReply<QVariantMap> reply =
      iface.call(QStringLiteral("GetActiveDownloads"));
  if (!reply.isValid())
    return;

  QVariantMap map = reply.value();
  for (auto it = map.constBegin(); it != map.constEnd(); ++it) {
    m_model->addOrUpdateInitial(it.key(), it.value().toMap());
  }
}

void BrowserConfig::startDownload(const QUrl &url, const QString &filename,
                                  const QString &mimeType) {
  QDBusInterface iface(QStringLiteral("org.ciel.Downloads"),
                       QStringLiteral("/org/ciel/Downloads"),
                       QStringLiteral("org.ciel.Downloads"),
                       QDBusConnection::sessionBus());
  if (!iface.isValid())
    return;

  QString customDest;
  if (!filename.isEmpty()) {
    QDBusReply<QString> destReply =
        iface.call(QStringLiteral("ResolveDestination"), filename, mimeType);
    if (destReply.isValid()) {
      customDest = destReply.value();
    }
  }

  iface.call(QStringLiteral("StartDownload"), url.toString(), customDest);
}

void BrowserConfig::pauseDownload(const QString &id) {
  QDBusInterface iface(QStringLiteral("org.ciel.Downloads"),
                       QStringLiteral("/org/ciel/Downloads"),
                       QStringLiteral("org.ciel.Downloads"),
                       QDBusConnection::sessionBus());
  if (iface.isValid())
    iface.call(QStringLiteral("PauseDownload"), id);

  m_model->setState(id, QStringLiteral("paused"));
}

void BrowserConfig::resumeDownload(const QString &id) {
  QDBusInterface iface(QStringLiteral("org.ciel.Downloads"),
                       QStringLiteral("/org/ciel/Downloads"),
                       QStringLiteral("org.ciel.Downloads"),
                       QDBusConnection::sessionBus());
  if (iface.isValid())
    iface.call(QStringLiteral("ResumeDownload"), id);

  m_model->setState(id, QStringLiteral("downloading"));
}

void BrowserConfig::cancelDownload(const QString &id) {
  QDBusInterface iface(QStringLiteral("org.ciel.Downloads"),
                       QStringLiteral("/org/ciel/Downloads"),
                       QStringLiteral("org.ciel.Downloads"),
                       QDBusConnection::sessionBus());
  if (iface.isValid())
    iface.call(QStringLiteral("CancelDownload"), id);

  m_model->removeById(id);
}

void BrowserConfig::retryDownload(const QString &id) {
  QModelIndexList matches = m_model->match(
      m_model->index(0, 0), DownloadsModel::IdRole, id, 1, Qt::MatchExactly);
  if (!matches.isEmpty()) {
    QModelIndex idx = matches.first();
    startDownload(
        QUrl(m_model->data(idx, DownloadsModel::DestinationRole).toString()),
        m_model->data(idx, DownloadsModel::FilenameRole).toString());
  }
}

void BrowserConfig::openFolder(const QString &path) {
  QString targetPath = path;
  if (targetPath.isEmpty()) {
    targetPath =
        QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    if (targetPath.isEmpty())
      targetPath = QDir::homePath() + QStringLiteral("/Downloads");
  } else {
    QFileInfo fi(targetPath);
    if (fi.isFile())
      targetPath = fi.absolutePath();
  }
  QDesktopServices::openUrl(QUrl::fromLocalFile(targetPath));
}

void BrowserConfig::clearFinished() { m_model->clearCompleted(); }

void BrowserConfig::onDownloadStarted(const QString &id,
                                      const QString &filename,
                                      const QString &destination,
                                      qint64 total_bytes,
                                      const QString &category) {
  m_model->onStarted(id, filename, destination, total_bytes, category);
}

void BrowserConfig::onDownloadProgress(const QString &id, qint64 bytes_received,
                                       qint64 total_bytes,
                                       double speed_bytes_sec,
                                       const QList<double> &segments) {
  m_model->onProgress(id, bytes_received, total_bytes, speed_bytes_sec,
                      segments);
}

void BrowserConfig::onDownloadFinished(const QString &id,
                                       const QString &destination,
                                       const QString &category) {
  m_model->onFinished(id, destination, category);
}

void BrowserConfig::onDownloadFailed(const QString &id,
                                     const QString &error_message) {
  m_model->onFailed(id, error_message);
}
