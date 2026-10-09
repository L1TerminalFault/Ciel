#pragma once

#include "ThemeMetrics.hpp"
#include <QColor>
#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <qcolor.h>

class OrgCielThemeInterface;

class Theme : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

  Q_PROPERTY(ThemeMetrics *metrics READ metrics CONSTANT)

  // Semantic tokens
  Q_PROPERTY(QColor background READ background NOTIFY themeChanged)
  Q_PROPERTY(QColor transparent READ transparent NOTIFY themeChanged)
  Q_PROPERTY(QColor surface READ surface NOTIFY themeChanged)
  Q_PROPERTY(QColor surfaceHover READ surfaceHover NOTIFY themeChanged)
  Q_PROPERTY(QColor surfacePressed READ surfacePressed NOTIFY themeChanged)
  Q_PROPERTY(QColor textPrimary READ textPrimary NOTIFY themeChanged)
  Q_PROPERTY(QColor textSecondary READ textSecondary NOTIFY themeChanged)
  Q_PROPERTY(QColor accent READ accent NOTIFY themeChanged)
  Q_PROPERTY(QColor accentHover READ accentHover NOTIFY themeChanged)
  Q_PROPERTY(QColor accentPressed READ accentPressed NOTIFY themeChanged)
  Q_PROPERTY(QColor border READ border NOTIFY themeChanged)

  Q_PROPERTY(bool isDark READ isDark NOTIFY themeChanged)
  Q_PROPERTY(int transitionMs READ transitionMs NOTIFY themeChanged)
public:
  explicit Theme(QObject *parent = nullptr);
  ~Theme() override = default;
  enum class IconSize {
    XXSMALL = 8,
    XSMALL = 12,
    SMALL = 16,
    MEDIUM = 20,
    LARGE = 24,
    XLARGE = 32
  };
  Q_ENUM(IconSize)

  ThemeMetrics *metrics() { return &m_metrics; }

  QColor background() const { return m_background; }
  QColor transparent() const { return m_transparent; }
  QColor surface() const { return m_surface; }
  QColor surfaceHover() const { return m_surfaceHover; }
  QColor surfacePressed() const { return m_surfacePressed; }
  QColor textPrimary() const { return m_textPrimary; }
  QColor textSecondary() const { return m_textSecondary; }
  QColor accent() const { return m_accent; }
  QColor accentHover() const { return m_accentHover; }
  QColor accentPressed() const { return m_accentPressed; }
  QColor border() const { return m_border; }

  bool isDark() const { return m_isDark; }
  int transitionMs() const { return m_reducedMotion ? 0 : 300; }

  Q_INVOKABLE void toggleMode();
  Q_INVOKABLE void setAccent(const QString &hexColor);

signals:
  void themeChanged();

private slots:
  void onThemeChangedSignal(bool darkMode, const QString &accentColor,
                            bool reducedMotion);

private:
  void resolveSemanticTokens();

  ThemeMetrics m_metrics;
  OrgCielThemeInterface *m_iface = nullptr;

  bool m_isDark = true;
  QString m_accentHex = "#64c5fa";
  bool m_reducedMotion = false;

  // Resolved cache
  QColor m_background;
  QColor m_transparent;
  QColor m_surface;
  QColor m_surfaceHover;
  QColor m_surfacePressed;
  QColor m_textPrimary;
  QColor m_textSecondary;
  QColor m_accent;
  QColor m_accentHover;
  QColor m_accentPressed;
  QColor m_border;
};
