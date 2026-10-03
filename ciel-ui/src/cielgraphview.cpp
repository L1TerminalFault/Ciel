#include "cielgraphview.hpp"
#include <QPointF>
#include <QSGGeometry>
#include <QSGGeometryNode>
#include <QSGVertexColorMaterial>
#include <QVector>
#include <algorithm>
#include <cmath>

CielGraphView::CielGraphView(QQuickItem *parent) : QQuickItem(parent) {
  setFlag(ItemHasContents, true);
  setClip(true);
  setAntialiasing(true);

  m_scrollAnim = new QPropertyAnimation(this, "scrollProgress", this);
  m_scrollAnim->setEasingCurve(QEasingCurve::Linear);
  m_elapsedTimer.start();
}

void CielGraphView::setValues(const QList<qreal> &values) {
  if (values.isEmpty()) {
    m_values.clear();
    m_previousValues.clear();
    m_hasDroppedHead = false;
    emit valuesChanged();
    update();
    return;
  }

  const qint64 now = m_elapsedTimer.elapsed();
  const qint64 elapsed = (m_lastTimestamp > 0) ? (now - m_lastTimestamp) : 1000;
  m_lastTimestamp = now;

  if (m_smoothScroll && !m_previousValues.isEmpty() &&
      m_previousValues.size() == values.size()) {
    m_droppedHead = m_previousValues.first();
    m_hasDroppedHead = true;
    m_previousValues = values;
    m_values = values;

    const int duration = std::clamp(static_cast<int>(elapsed), 80, 2000);
    m_scrollAnim->stop();
    m_scrollAnim->setDuration(duration);
    m_scrollAnim->setStartValue(0.0);
    m_scrollAnim->setEndValue(1.0);
    m_scrollAnim->start();

    emit valuesChanged();
    return;
  }

  m_previousValues = values;
  m_values = values;
  m_droppedHead = values.first();
  m_hasDroppedHead = false;
  m_scrollProgress = 1.0;
  m_scrollAnim->stop();

  emit valuesChanged();
  update();
}

void CielGraphView::setMinValue(qreal val) {
  if (qFuzzyCompare(m_minValue, val))
    return;
  m_minValue = val;
  emit minValueChanged();
  update();
}

void CielGraphView::setMaxValue(qreal val) {
  if (qFuzzyCompare(m_maxValue, val))
    return;
  m_maxValue = val;
  emit maxValueChanged();
  update();
}

void CielGraphView::setStrokeColor(const QColor &c) {
  if (m_strokeColor == c)
    return;
  m_strokeColor = c;
  emit strokeColorChanged();
  update();
}

void CielGraphView::setStrokeWidth(qreal w) {
  if (qFuzzyCompare(m_strokeWidth, w))
    return;
  m_strokeWidth = w;
  emit strokeWidthChanged();
  update();
}

void CielGraphView::setFillOpacityTop(qreal o) {
  if (qFuzzyCompare(m_fillOpacityTop, o))
    return;
  m_fillOpacityTop = o;
  emit fillOpacityTopChanged();
  update();
}

void CielGraphView::setSmoothScroll(bool s) {
  if (m_smoothScroll == s)
    return;
  m_smoothScroll = s;
  if (!m_smoothScroll) {
    m_scrollAnim->stop();
    m_scrollProgress = 1.0;
  }
  emit smoothScrollChanged();
  update();
}

void CielGraphView::setScrollProgress(qreal p) {
  if (qFuzzyCompare(m_scrollProgress, p))
    return;
  m_scrollProgress = p;
  emit scrollProgressChanged();
  update();
}

QSGNode *CielGraphView::updatePaintNode(QSGNode *oldNode,
                                        UpdatePaintNodeData *) {
  auto *rootNode = static_cast<QSGNode *>(oldNode);
  if (!rootNode) {
    rootNode = new QSGNode();
  }

  const int rawCount = m_values.size();
  const qreal w = width();
  const qreal h = height();

  if (rawCount < 2 || w <= 0.0 || h <= 0.0) {
    delete rootNode;
    return nullptr;
  }

  const qreal feather = 1.0;
  const qreal rIn = std::max(0.0, (m_strokeWidth * 0.5) - (feather * 0.5));
  const qreal rOut = (m_strokeWidth * 0.5) + (feather * 0.5);
  const qreal padding = std::ceil(rOut) + 1.5;

  const qreal minY = padding;
  const qreal maxY = std::max(minY + 1.0, h - padding);
  const qreal usableH = maxY - minY;

  const bool useScroll = m_smoothScroll && m_hasDroppedHead;
  const int count = useScroll ? (rawCount + 1) : rawCount;
  const qreal p = useScroll ? m_scrollProgress : 1.0;

  const qreal stepX = w / static_cast<qreal>(rawCount - 1);
  const qreal range =
      qFuzzyCompare(m_maxValue, m_minValue) ? 1.0 : (m_maxValue - m_minValue);

  QVector<QPointF> basePoints;
  basePoints.reserve(count);

  for (int i = 0; i < count; ++i) {
    qreal x = 0.0;
    qreal val = 0.0;

    if (useScroll) {
      x = (static_cast<qreal>(i) - p) * stepX;
      val = (i == 0) ? m_droppedHead : m_values.at(i - 1);
    } else {
      x = static_cast<qreal>(i) * stepX;
      val = m_values.at(i);
    }

    const qreal clampedVal = std::clamp(val, m_minValue, m_maxValue);
    const qreal norm = (clampedVal - m_minValue) / range;
    const qreal y = maxY - (norm * usableH);

    basePoints.append(QPointF(x, y));
  }

  const int subdivisions = 12;
  QVector<QPointF> curvePoints;
  QVector<QPointF> curveNormals;
  curvePoints.reserve((count - 1) * subdivisions + 1);
  curveNormals.reserve((count - 1) * subdivisions + 1);

  for (int i = 0; i < count - 1; ++i) {
    const QPointF p0 = (i > 0) ? basePoints.at(i - 1) : basePoints.at(i);
    const QPointF p1 = basePoints.at(i);
    const QPointF p2 = basePoints.at(i + 1);
    const QPointF p3 = (i + 2 < count) ? basePoints.at(i + 2) : p2;

    QPointF cp1 = p1 + (p2 - p0) / 6.0;
    QPointF cp2 = p2 - (p3 - p1) / 6.0;

    cp1.setY(std::clamp(cp1.y(), minY, maxY));
    cp2.setY(std::clamp(cp2.y(), minY, maxY));

    for (int s = 0; s < subdivisions; ++s) {
      const qreal t = static_cast<qreal>(s) / static_cast<qreal>(subdivisions);
      const qreal u = 1.0 - t;
      const qreal tt = t * t;
      const qreal uu = u * u;

      const QPointF pt = (uu * u * p1) + (3.0 * uu * t * cp1) +
                         (3.0 * u * tt * cp2) + (tt * t * p2);

      const QPointF tangent = (3.0 * uu * (cp1 - p1)) +
                              (6.0 * u * t * (cp2 - cp1)) +
                              (3.0 * tt * (p2 - cp2));

      const qreal len = std::hypot(tangent.x(), tangent.y());
      QPointF norm(0.0, 1.0);
      if (len > 1e-6) {
        norm = QPointF(-tangent.y() / len, tangent.x() / len);
      }

      curvePoints.append(QPointF(pt.x(), std::clamp(pt.y(), minY, maxY)));
      curveNormals.append(norm);
    }
  }

  const QPointF lastPt = basePoints.last();
  QPointF lastTangent = lastPt - basePoints.at(count - 2);
  const qreal lastLen = std::hypot(lastTangent.x(), lastTangent.y());
  QPointF lastNorm(0.0, 1.0);
  if (lastLen > 1e-6) {
    lastNorm = QPointF(-lastTangent.y() / lastLen, lastTangent.x() / lastLen);
  }
  curvePoints.append(QPointF(lastPt.x(), std::clamp(lastPt.y(), minY, maxY)));
  curveNormals.append(lastNorm);

  const int totalVerts = curvePoints.size();
  if (totalVerts < 2) {
    delete rootNode;
    return nullptr;
  }

  QSGGeometryNode *fillNode = nullptr;
  QSGGeometryNode *strokeNode = nullptr;

  if (rootNode->childCount() == 2) {
    fillNode = static_cast<QSGGeometryNode *>(rootNode->childAtIndex(0));
    strokeNode = static_cast<QSGGeometryNode *>(rootNode->childAtIndex(1));
  } else {
    while (rootNode->childCount() > 0) {
      delete rootNode->childAtIndex(0);
    }

    fillNode = new QSGGeometryNode();
    auto *fillGeom = new QSGGeometry(
        QSGGeometry::defaultAttributes_ColoredPoint2D(), totalVerts * 2);
    fillGeom->setDrawingMode(QSGGeometry::DrawTriangleStrip);
    fillNode->setGeometry(fillGeom);
    fillNode->setFlag(QSGNode::OwnsGeometry);

    auto *fillMat = new QSGVertexColorMaterial();
    fillNode->setMaterial(fillMat);
    fillNode->setFlag(QSGNode::OwnsMaterial);
    rootNode->appendChildNode(fillNode);

    strokeNode = new QSGGeometryNode();
    auto *strokeGeom =
        new QSGGeometry(QSGGeometry::defaultAttributes_ColoredPoint2D(),
                        totalVerts * 4, (totalVerts - 1) * 18);
    strokeGeom->setDrawingMode(QSGGeometry::DrawTriangles);
    strokeNode->setGeometry(strokeGeom);
    strokeNode->setFlag(QSGNode::OwnsGeometry);

    auto *strokeMat = new QSGVertexColorMaterial();
    strokeNode->setMaterial(strokeMat);
    strokeNode->setFlag(QSGNode::OwnsMaterial);
    rootNode->appendChildNode(strokeNode);
  }

  auto *fillGeom = fillNode->geometry();
  if (fillGeom->vertexCount() != totalVerts * 2) {
    fillGeom->allocate(totalVerts * 2);
  }
  auto *fillVerts = fillGeom->vertexDataAsColoredPoint2D();

  const qreal alphaFactor = std::clamp(m_fillOpacityTop, 0.0, 1.0);
  const uchar aTop = static_cast<uchar>(std::round(alphaFactor * 255.0));
  const uchar rTop =
      static_cast<uchar>(std::round(m_strokeColor.red() * alphaFactor));
  const uchar gTop =
      static_cast<uchar>(std::round(m_strokeColor.green() * alphaFactor));
  const uchar bTop =
      static_cast<uchar>(std::round(m_strokeColor.blue() * alphaFactor));

  for (int i = 0; i < totalVerts; ++i) {
    const QPointF pt = curvePoints.at(i);
    fillVerts[i * 2].set(pt.x(), pt.y(), rTop, gTop, bTop, aTop);
    fillVerts[i * 2 + 1].set(pt.x(), h, 0, 0, 0, 0);
  }
  fillNode->markDirty(QSGNode::DirtyGeometry);

  auto *strokeGeom = strokeNode->geometry();
  const int reqVerts = totalVerts * 4;
  const int reqIndices = (totalVerts - 1) * 18;
  if (strokeGeom->vertexCount() != reqVerts ||
      strokeGeom->indexCount() != reqIndices) {
    strokeGeom->allocate(reqVerts, reqIndices);
  }

  auto *strokeVerts = strokeGeom->vertexDataAsColoredPoint2D();
  auto *indices = strokeGeom->indexDataAsUShort();

  const qreal strokeAlpha = m_strokeColor.alphaF();
  const uchar sA = static_cast<uchar>(std::round(strokeAlpha * 255.0));
  const uchar sR =
      static_cast<uchar>(std::round(m_strokeColor.red() * strokeAlpha));
  const uchar sG =
      static_cast<uchar>(std::round(m_strokeColor.green() * strokeAlpha));
  const uchar sB =
      static_cast<uchar>(std::round(m_strokeColor.blue() * strokeAlpha));

  for (int i = 0; i < totalVerts; ++i) {
    const QPointF curr = curvePoints.at(i);
    const QPointF normal = curveNormals.at(i);

    const QPointF p0 = curr + (normal * rOut);
    const QPointF p1 = curr + (normal * rIn);
    const QPointF p2 = curr - (normal * rIn);
    const QPointF p3 = curr - (normal * rOut);

    const int base = i * 4;
    strokeVerts[base + 0].set(p0.x(), p0.y(), 0, 0, 0, 0);
    strokeVerts[base + 1].set(p1.x(), p1.y(), sR, sG, sB, sA);
    strokeVerts[base + 2].set(p2.x(), p2.y(), sR, sG, sB, sA);
    strokeVerts[base + 3].set(p3.x(), p3.y(), 0, 0, 0, 0);
  }

  int idx = 0;
  for (int i = 0; i < totalVerts - 1; ++i) {
    const quint16 b = static_cast<quint16>(i * 4);
    const quint16 nxt = static_cast<quint16>((i + 1) * 4);

    indices[idx++] = b + 0;
    indices[idx++] = b + 1;
    indices[idx++] = nxt + 1;
    indices[idx++] = b + 0;
    indices[idx++] = nxt + 1;
    indices[idx++] = nxt + 0;

    indices[idx++] = b + 1;
    indices[idx++] = b + 2;
    indices[idx++] = nxt + 2;
    indices[idx++] = b + 1;
    indices[idx++] = nxt + 2;
    indices[idx++] = nxt + 1;

    indices[idx++] = b + 2;
    indices[idx++] = b + 3;
    indices[idx++] = nxt + 3;
    indices[idx++] = b + 2;
    indices[idx++] = nxt + 3;
    indices[idx++] = nxt + 2;
  }

  strokeNode->markDirty(QSGNode::DirtyGeometry);

  return rootNode;
}
