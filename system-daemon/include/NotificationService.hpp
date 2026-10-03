#pragma once

#include <QDateTime>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariantMap>
#include <vector>

struct NotificationEntry {
  uint id;
  QString appName;
  QString appIcon;
  QString summary;
  QString body;
  uchar urgency; // 0 = low, 1 = normal, 2 = critical
  QDateTime timestamp;
  bool dismissed = false;
};

class NotificationService : public QObject {
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "org.freedesktop.Notifications")

  // Properties for org.ciel.Notifications
  Q_PROPERTY(bool DoNotDisturb READ isDoNotDisturb WRITE SetDoNotDisturb NOTIFY
                 doNotDisturbChanged)
  Q_PROPERTY(uint UnreadCount READ unreadCount NOTIFY unreadCountChanged)

public:
  explicit NotificationService(QObject *parent = nullptr);

  bool isDoNotDisturb() const { return m_dnd; }
  uint unreadCount() const { return m_unreadCount; }

public slots:
  // freedesktop.Notifications methods (Exact case matches XML spec)
  uint Notify(const QString &app_name, uint replaces_id,
              const QString &app_icon, const QString &summary,
              const QString &body, const QStringList &actions,
              const QVariantMap &hints, int expire_timeout);
  void CloseNotification(uint id);
  QStringList GetCapabilities();
  QString GetServerInformation(QString &vendor, QString &version,
                               QString &spec_version);

  // org.ciel.Notifications methods
  void SetDoNotDisturb(bool enabled);
  void ClearAll();
  void Dismiss(uint id);

signals:
  // freedesktop.Notifications signals
  void NotificationClosed(uint id, uint reason);
  void ActionInvoked(uint id, const QString &action_key);

  // org.ciel.Notifications signals
  void NotificationAdded(uint id, const QString &appName,
                         const QString &summary, const QString &body,
                         const QString &appIcon, uchar urgency);
  void NotificationRemoved(uint id);
  void UnreadCountChanged(uint count);
  void doNotDisturbChanged(bool enabled);
  void unreadCountChanged(uint count);

private:
  void updateUnreadCount();
  void saveHistory();
  void loadHistory();
  QString historyFilePath() const;

  uint m_nextId = 1;
  bool m_dnd = false;
  uint m_unreadCount = 0;
  std::vector<NotificationEntry> m_history;
};
