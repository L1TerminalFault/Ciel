#include "Theme.hpp"
#include "ThemeInterface.h"
#include <QDBusConnection>
#include <QDebug>
#include <qcolor.h>

Theme::Theme(QObject *parent) : QObject(parent) {
  m_iface = new OrgCielThemeInterface("org.ciel.Theme", "/org/ciel/Theme",
                                      QDBusConnection::sessionBus(), this);

  if (m_iface->isValid()) {
    m_isDark = m_iface->darkMode();
    m_accentHex = m_iface->accentColor();
    m_reducedMotion = m_iface->reducedMotion();

    connect(m_iface, &OrgCielThemeInterface::ThemeChanged, this,
            &Theme::onThemeChangedSignal);

    qInfo() << "[Theme Client] Connected to system-daemon over D-Bus."
            << "Dark:" << m_isDark << "Accent:" << m_accentHex;
  } else {
    qWarning()
        << "[Theme Client] system-daemon not detected. Running fallback theme.";
  }

  resolveSemanticTokens();
}

void Theme::onThemeChangedSignal(bool darkMode, const QString &accentColor,
                                 bool reducedMotion) {
  qInfo() << "[Theme Client] Received ThemeChanged signal from D-Bus!"
          << "Dark:" << darkMode << "Accent:" << accentColor;

  m_isDark = darkMode;
  m_accentHex = accentColor;
  m_reducedMotion = reducedMotion;

  resolveSemanticTokens();
  emit themeChanged();
}

void Theme::toggleMode() {
  if (m_iface && m_iface->isValid()) {
    m_iface->SetDarkMode(!m_isDark);
  } else {
    m_isDark = !m_isDark;
    resolveSemanticTokens();
    emit themeChanged();
  }
}

void Theme::setAccent(const QString &hexColor) {
  if (m_iface && m_iface->isValid()) {
    m_iface->SetAccentColor(hexColor);
  }
}

void Theme::resolveSemanticTokens() {
  if (m_isDark) {
    m_background = QColor("#0D0D0E");
    m_surface = QColor("#1C1C1E");
    m_surfaceHover = QColor("#2C2C2E");
    m_surfacePressed = QColor("#3A3A3C");
    m_textPrimary = QColor("#FFFFFF");
    m_textSecondary = QColor("#8E8E93");
    m_border = QColor("#282828");
  } else {
    m_background = QColor("#F2F2F7");
    m_surface = QColor("#FFFFFF");
    m_surfaceHover = QColor("#E5E5EA");
    m_surfacePressed = QColor("#D1D1D6");
    m_textPrimary = QColor("#000000");
    m_textSecondary = QColor("#6C6C70");
    m_border = QColor("#d9d9d9");
  }

  m_transparent = QColor("#00FFFFFF");
  m_accent = QColor(m_accentHex);
  m_accentHover = m_accent.lighter(115);
  m_accentPressed = m_accent.darker(115);
}
