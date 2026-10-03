#pragma once

#include <QAbstractListModel>
#include <QString>
#include <QVector>
#include <QtQml/qqmlregistration.h>

class OrgCielNotificationsInterface;

struct NotificationItem {
  uint id;
  QString appName;
  QString summary;
  QString body;
  QString appIcon;
  int urgency;
};

class NotificationsModel : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

  Q_PROPERTY(int unreadCount READ unreadCount NOTIFY unreadCountChanged)
  Q_PROPERTY(bool doNotDisturb READ doNotDisturb WRITE setDoNotDisturb NOTIFY
                 doNotDisturbChanged)

public:
  enum NotificationRoles {
    IdRole = Qt::UserRole + 1,
    AppNameRole,
    SummaryRole,
    BodyRole,
    AppIconRole,
    UrgencyRole
  };

  explicit NotificationsModel(QObject *parent = nullptr);
  ~NotificationsModel() override = default;

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  int unreadCount() const { return m_items.size(); }
  bool doNotDisturb() const { return m_dnd; }

  Q_INVOKABLE void dismiss(uint id);
  Q_INVOKABLE void clearAll();
  Q_INVOKABLE void setDoNotDisturb(bool enabled);

signals:
  void unreadCountChanged();
  void doNotDisturbChanged();
  // Signal to trigger floating toast popups in the UI
  void popupRequested(uint id, const QString &appName, const QString &summary,
                      const QString &body);

private slots:
  void onNotificationAdded(uint id, const QString &appName,
                           const QString &summary, const QString &body,
                           const QString &appIcon, uchar urgency);
  void onNotificationRemoved(uint id);

private:
  OrgCielNotificationsInterface *m_iface = nullptr;
  QVector<NotificationItem> m_items;
  bool m_dnd = false;
};
