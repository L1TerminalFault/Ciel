#include "NotificationsModel.hpp"
#include "CielNotificationsInterface.h"
#include <QDBusConnection>
#include <QDebug>

NotificationsModel::NotificationsModel(QObject *parent)
    : QAbstractListModel(parent) {
  m_iface = new OrgCielNotificationsInterface(
      "org.ciel.Notifications", "/org/ciel/Notifications",
      QDBusConnection::sessionBus(), this);

  if (m_iface->isValid()) {
    m_dnd = m_iface->doNotDisturb();

    connect(m_iface, &OrgCielNotificationsInterface::NotificationAdded, this,
            &NotificationsModel::onNotificationAdded);
    connect(m_iface, &OrgCielNotificationsInterface::NotificationRemoved, this,
            &NotificationsModel::onNotificationRemoved);

    qInfo() << "[Notifications Client] Connected to daemon.";
  } else {
    qWarning() << "[Notifications Client] org.ciel.Notifications unavailable "
                  "on D-Bus.";
  }
}

int NotificationsModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;
  return m_items.size();
}

QVariant NotificationsModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() < 0 || index.row() >= m_items.size())
    return QVariant();

  const auto &item = m_items.at(index.row());
  switch (role) {
  case IdRole:
    return item.id;
  case AppNameRole:
    return item.appName;
  case SummaryRole:
    return item.summary;
  case BodyRole:
    return item.body;
  case AppIconRole:
    return item.appIcon;
  case UrgencyRole:
    return item.urgency;
  default:
    return QVariant();
  }
}

QHash<int, QByteArray> NotificationsModel::roleNames() const {
  return {{IdRole, "id"},           {AppNameRole, "appName"},
          {SummaryRole, "summary"}, {BodyRole, "body"},
          {AppIconRole, "appIcon"}, {UrgencyRole, "urgency"}};
}

void NotificationsModel::onNotificationAdded(uint id, const QString &appName,
                                             const QString &summary,
                                             const QString &body,
                                             const QString &appIcon,
                                             uchar urgency) {
  beginInsertRows(QModelIndex(), 0, 0);
  m_items.prepend(
      {id, appName, summary, body, appIcon, static_cast<int>(urgency)});
  endInsertRows();

  emit unreadCountChanged();
  emit popupRequested(id, appName, summary, body);
}

void NotificationsModel::onNotificationRemoved(uint id) {
  for (int i = 0; i < m_items.size(); ++i) {
    if (m_items[i].id == id) {
      beginRemoveRows(QModelIndex(), i, i);
      m_items.removeAt(i);
      endRemoveRows();
      emit unreadCountChanged();
      break;
    }
  }
}

void NotificationsModel::dismiss(uint id) {
  if (m_iface && m_iface->isValid()) {
    m_iface->Dismiss(id);
  }
}

void NotificationsModel::clearAll() {
  if (m_iface && m_iface->isValid()) {
    m_iface->ClearAll();
  }
}

void NotificationsModel::setDoNotDisturb(bool enabled) {
  if (m_iface && m_iface->isValid()) {
    m_iface->SetDoNotDisturb(enabled);
    m_dnd = enabled;
    emit doNotDisturbChanged();
  }
}
