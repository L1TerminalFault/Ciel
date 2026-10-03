#include "NotificationService.hpp"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

NotificationService::NotificationService(QObject *parent) : QObject(parent) {
  loadHistory();
}

QString NotificationService::historyFilePath() const {
  const QString dir =
      QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) +
      "/ciel";
  QDir().mkpath(dir);
  return dir + "/notifications.json";
}

void NotificationService::loadHistory() {
  QFile file(historyFilePath());
  if (!file.open(QIODevice::ReadOnly))
    return;

  const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
  const QJsonArray array = doc.array();

  m_history.clear();
  for (const auto &val : array) {
    QJsonObject obj = val.toObject();
    NotificationEntry entry;
    entry.id = obj["id"].toInt();
    entry.appName = obj["appName"].toString();
    entry.appIcon = obj["appIcon"].toString();
    entry.summary = obj["summary"].toString();
    entry.body = obj["body"].toString();
    entry.urgency = static_cast<uchar>(obj["urgency"].toInt());
    entry.timestamp =
        QDateTime::fromString(obj["timestamp"].toString(), Qt::ISODate);
    entry.dismissed = obj["dismissed"].toBool();

    if (entry.id >= m_nextId) {
      m_nextId = entry.id + 1;
    }
    m_history.push_back(entry);
  }
  updateUnreadCount();
}

void NotificationService::saveHistory() {
  QFile file(historyFilePath());
  if (!file.open(QIODevice::WriteOnly))
    return;

  QJsonArray array;
  // Persist up to the latest 100 entries
  int start = std::max(0, static_cast<int>(m_history.size()) - 100);
  for (size_t i = start; i < m_history.size(); ++i) {
    const auto &entry = m_history[i];
    QJsonObject obj;
    obj["id"] = static_cast<int>(entry.id);
    obj["appName"] = entry.appName;
    obj["appIcon"] = entry.appIcon;
    obj["summary"] = entry.summary;
    obj["body"] = entry.body;
    obj["urgency"] = entry.urgency;
    obj["timestamp"] = entry.timestamp.toString(Qt::ISODate);
    obj["dismissed"] = entry.dismissed;
    array.append(obj);
  }

  file.write(QJsonDocument(array).toJson(QJsonDocument::Compact));
}

void NotificationService::updateUnreadCount() {
  uint count = 0;
  for (const auto &entry : m_history) {
    if (!entry.dismissed)
      count++;
  }
  if (m_unreadCount != count) {
    m_unreadCount = count;
    emit unreadCountChanged(m_unreadCount);
    emit UnreadCountChanged(m_unreadCount);
  }
}

uint NotificationService::Notify(const QString &app_name, uint replaces_id,
                                 const QString &app_icon,
                                 const QString &summary, const QString &body,
                                 const QStringList &actions,
                                 const QVariantMap &hints, int expire_timeout) {
  Q_UNUSED(actions);
  Q_UNUSED(expire_timeout);

  uint id = (replaces_id > 0) ? replaces_id : m_nextId++;

  uchar urgency = 1; // Default normal
  if (hints.contains("urgency")) {
    urgency = static_cast<uchar>(hints["urgency"].toUInt());
  }

  NotificationEntry entry;
  entry.id = id;
  entry.appName = app_name.isEmpty() ? "System" : app_name;
  entry.appIcon = app_icon;
  entry.summary = summary;
  entry.body = body;
  entry.urgency = urgency;
  entry.timestamp = QDateTime::currentDateTime();
  entry.dismissed = false;

  m_history.push_back(entry);
  saveHistory();
  updateUnreadCount();

  qInfo() << "[Notifications] Incoming:" << entry.appName << "-"
          << entry.summary;

  if (!m_dnd || urgency == 2) { // Critical ignores DND
    emit NotificationAdded(id, entry.appName, entry.summary, entry.body,
                           entry.appIcon, urgency);
  }

  return id;
}

void NotificationService::CloseNotification(uint id) {
  Dismiss(id);
  emit NotificationClosed(id, 2); // 2 = dismissed by user/client
}

void NotificationService::Dismiss(uint id) {
  for (auto &entry : m_history) {
    if (entry.id == id) {
      entry.dismissed = true;
      emit NotificationRemoved(id);
      break;
    }
  }
  saveHistory();
  updateUnreadCount();
}

void NotificationService::ClearAll() {
  for (auto &entry : m_history) {
    if (!entry.dismissed) {
      entry.dismissed = true;
      emit NotificationRemoved(entry.id);
    }
  }
  saveHistory();
  updateUnreadCount();
}

void NotificationService::SetDoNotDisturb(bool enabled) {
  if (m_dnd != enabled) {
    m_dnd = enabled;
    emit doNotDisturbChanged(m_dnd);
    qInfo() << "[Notifications] Do Not Disturb set to:" << m_dnd;
  }
}

QStringList NotificationService::GetCapabilities() {
  return {"body", "actions", "icon-static", "persistence"};
}

QString NotificationService::GetServerInformation(QString &vendor,
                                                  QString &version,
                                                  QString &spec_version) {
  vendor = "Ciel Desktop";
  version = "0.1.0";
  spec_version = "1.2";
  return "cield";
}
