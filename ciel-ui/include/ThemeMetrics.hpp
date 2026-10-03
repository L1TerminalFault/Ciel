#pragma once

#include <QObject>
#include <QtQml/qqmlregistration.h>

class ThemeMetrics : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("ThemeMetrics is accessed via Theme.metrics")

  Q_PROPERTY(int spacingXs READ spacingXs CONSTANT)
  Q_PROPERTY(int spacingSm READ spacingSm CONSTANT)
  Q_PROPERTY(int spacingMd READ spacingMd CONSTANT)
  Q_PROPERTY(int spacingLg READ spacingLg CONSTANT)
  Q_PROPERTY(int spacingXl READ spacingXl CONSTANT)

  Q_PROPERTY(qreal radiusSm READ radiusSm CONSTANT)
  Q_PROPERTY(qreal radiusMd READ radiusMd CONSTANT)
  Q_PROPERTY(qreal radiusLg READ radiusLg CONSTANT)

  Q_PROPERTY(qreal squircleExponent READ squircleExponent CONSTANT)

public:
  explicit ThemeMetrics(QObject *parent = nullptr) : QObject(parent) {}

  int spacingXs() const { return 4; }
  int spacingSm() const { return 8; }
  int spacingMd() const { return 14; }
  int spacingLg() const { return 20; }
  int spacingXl() const { return 32; }

  qreal radiusSm() const { return 8.0; }
  qreal radiusMd() const { return 14.0; }
  qreal radiusLg() const { return 22.0; }

  qreal squircleExponent() const { return 3.8; }
};
