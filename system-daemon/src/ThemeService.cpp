#include "ThemeService.hpp"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

ThemeService::ThemeService(QObject *parent) : QObject(parent) { loadConfig(); }

QString ThemeService::configFilePath() const {
  const QString dir =
      QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) +
      "/ciel";
  QDir().mkpath(dir);
  return dir + "/theme.json";
}

void ThemeService::loadConfig() {
  QFile file(configFilePath());
  if (!file.open(QIODevice::ReadOnly)) {
    qInfo()
        << "[ThemeService] No existing config found. Writing default values.";
    saveConfig();
    return;
  }

  const QJsonObject json = QJsonDocument::fromJson(file.readAll()).object();
  m_darkMode = json.value("darkMode").toBool(true);
  m_accentColor = json.value("accentColor").toString("#0A84FF");
  m_reducedMotion = json.value("reducedMotion").toBool(false);

  qInfo() << "[ThemeService] Loaded configuration from disk:"
          << "Dark:" << m_darkMode << "Accent:" << m_accentColor
          << "ReducedMotion:" << m_reducedMotion;
}

void ThemeService::saveConfig() {
  QFile file(configFilePath());
  if (!file.open(QIODevice::WriteOnly)) {
    qWarning() << "[ThemeService] Failed to write config:"
               << file.errorString();
    return;
  }

  QJsonObject json;
  json["darkMode"] = m_darkMode;
  json["accentColor"] = m_accentColor;
  json["reducedMotion"] = m_reducedMotion;

  file.write(QJsonDocument(json).toJson(QJsonDocument::Indented));
}

void ThemeService::SetDarkMode(bool enabled) {
  if (m_darkMode == enabled)
    return;
  m_darkMode = enabled;
  saveConfig();
  emit ThemeChanged(m_darkMode, m_accentColor, m_reducedMotion);
  qInfo() << "[ThemeService] DarkMode updated to:" << m_darkMode;
}

void ThemeService::SetAccentColor(const QString &hexColor) {
  if (m_accentColor == hexColor)
    return;
  m_accentColor = hexColor;
  saveConfig();
  emit ThemeChanged(m_darkMode, m_accentColor, m_reducedMotion);
  qInfo() << "[ThemeService] AccentColor updated to:" << m_accentColor;
}

void ThemeService::SetReducedMotion(bool enabled) {
  if (m_reducedMotion == enabled)
    return;
  m_reducedMotion = enabled;
  saveConfig();
  emit ThemeChanged(m_darkMode, m_accentColor, m_reducedMotion);
  qInfo() << "[ThemeService] ReducedMotion updated to:" << m_reducedMotion;
}
