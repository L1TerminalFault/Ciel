#pragma once

#include <QColor>
#include <QElapsedTimer>
#include <QList>
#include <QPropertyAnimation>
#include <QQuickItem>
#include <QtQml/qqmlregistration.h>

class CielGraphView : public QQuickItem {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(
      QList<qreal> values READ values WRITE setValues NOTIFY valuesChanged)
  Q_PROPERTY(
      qreal minValue READ minValue WRITE setMinValue NOTIFY minValueChanged)
  Q_PROPERTY(
      qreal maxValue READ maxValue WRITE setMaxValue NOTIFY maxValueChanged)
  Q_PROPERTY(QColor strokeColor READ strokeColor WRITE setStrokeColor NOTIFY
                 strokeColorChanged)
  Q_PROPERTY(qreal strokeWidth READ strokeWidth WRITE setStrokeWidth NOTIFY
                 strokeWidthChanged)
  Q_PROPERTY(qreal fillOpacityTop READ fillOpacityTop WRITE setFillOpacityTop
                 NOTIFY fillOpacityTopChanged)
  Q_PROPERTY(bool smoothScroll READ smoothScroll WRITE setSmoothScroll NOTIFY
                 smoothScrollChanged)
  Q_PROPERTY(qreal scrollProgress READ scrollProgress WRITE setScrollProgress
                 NOTIFY scrollProgressChanged)

public:
  explicit CielGraphView(QQuickItem *parent = nullptr);

  QList<qreal> values() const { return m_values; }
  void setValues(const QList<qreal> &values);

  qreal minValue() const { return m_minValue; }
  void setMinValue(qreal val);

  qreal maxValue() const { return m_maxValue; }
  void setMaxValue(qreal val);

  QColor strokeColor() const { return m_strokeColor; }
  void setStrokeColor(const QColor &c);

  qreal strokeWidth() const { return m_strokeWidth; }
  void setStrokeWidth(qreal w);

  qreal fillOpacityTop() const { return m_fillOpacityTop; }
  void setFillOpacityTop(qreal o);

  bool smoothScroll() const { return m_smoothScroll; }
  void setSmoothScroll(bool s);

  qreal scrollProgress() const { return m_scrollProgress; }
  void setScrollProgress(qreal p);

signals:
  void valuesChanged();
  void minValueChanged();
  void maxValueChanged();
  void strokeColorChanged();
  void strokeWidthChanged();
  void fillOpacityTopChanged();
  void smoothScrollChanged();
  void scrollProgressChanged();

protected:
  QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;

private:
  QList<qreal> m_values;
  QList<qreal> m_previousValues;
  qreal m_droppedHead = 0.0;
  bool m_hasDroppedHead = false;

  qreal m_minValue = 0.0;
  qreal m_maxValue = 1.0;
  QColor m_strokeColor = QColor(0, 122, 255);
  qreal m_strokeWidth = 2.0;
  qreal m_fillOpacityTop = 0.25;

  bool m_smoothScroll = true;
  qreal m_scrollProgress = 1.0;
  QPropertyAnimation *m_scrollAnim = nullptr;
  QElapsedTimer m_elapsedTimer;
  qint64 m_lastTimestamp = 0;
};
