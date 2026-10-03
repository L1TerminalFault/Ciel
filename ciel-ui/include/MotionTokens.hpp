#pragma once

#include <QObject>
#include <QtQml/qqmlregistration.h>

class CielMotionTokens : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

  // Snappy, interactive (e.g. tracking gestures, dragging)
  Q_PROPERTY(qreal responsiveDamping READ responsiveDamping CONSTANT)
  Q_PROPERTY(qreal responsiveEpsilon READ responsiveEpsilon CONSTANT)
  Q_PROPERTY(qreal responsiveMass READ responsiveMass CONSTANT)

  // Standard spatial transitions (e.g. window spawn, sheets)
  Q_PROPERTY(qreal spatialDamping READ spatialDamping CONSTANT)
  Q_PROPERTY(qreal spatialEpsilon READ spatialEpsilon CONSTANT)
  Q_PROPERTY(qreal spatialMass READ spatialMass CONSTANT)

public:
  explicit CielMotionTokens(QObject *parent = nullptr) : QObject(parent) {}

  qreal responsiveDamping() const { return 0.75; }
  qreal responsiveEpsilon() const { return 0.25; }
  qreal responsiveMass() const { return 0.8; }

  qreal spatialDamping() const { return 0.85; }
  qreal spatialEpsilon() const { return 0.15; }
  qreal spatialMass() const { return 1.2; }
};
