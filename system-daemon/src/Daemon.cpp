#include "Daemon.hpp"
#include "CielNotificationsAdaptor.h"
#include "DownloadsAdaptor.h"
#include "NotificationsAdaptor.h"
#include "SystemStatsAdaptor.h"
#include "ThemeAdaptor.h"
#include <QDebug>
#include <QtDBus/QDBusConnection>

Daemon::Daemon(QObject *parent) : QObject(parent) {}

bool Daemon::init() {
  auto connection = QDBusConnection::sessionBus();
  if (!connection.isConnected()) {
    qCritical() << "[Daemon] Cannot connect to D-Bus session bus.";
    return false;
  }

  connection.registerService("org.ciel.Theme");
  m_themeService = std::make_unique<ThemeService>(this);
  new ThemeAdaptor(m_themeService.get());
  connection.registerObject("/org/ciel/Theme", m_themeService.get());

  connection.registerService("org.freedesktop.Notifications");
  connection.registerService("org.ciel.Notifications");
  m_notificationService = std::make_unique<NotificationService>(this);
  new NotificationsAdaptor(m_notificationService.get());
  connection.registerObject("/org/freedesktop/Notifications",
                            m_notificationService.get());
  new CielNotificationsAdaptor(m_notificationService.get());
  connection.registerObject("/org/ciel/Notifications",
                            m_notificationService.get());

  connection.registerService("org.ciel.Downloads");
  m_downloadService =
      std::make_unique<DownloadService>(m_notificationService.get(), this);
  new DownloadsAdaptor(m_downloadService.get());
  connection.registerObject("/org/ciel/Downloads", m_downloadService.get());

  connection.registerService("org.ciel.SystemStats");
  m_systemStatsService = std::make_unique<SystemStatsService>(this);
  new SystemStatsAdaptor(m_systemStatsService.get());
  connection.registerObject("/org/ciel/SystemStats",
                            m_systemStatsService.get());

  qInfo() << "[Daemon] Services registered successfully:"
          << "org.ciel.Theme, org.freedesktop.Notifications, "
             "org.ciel.Notifications, org.ciel.Downloads, org.ciel.SystemStats";
  return true;
}
