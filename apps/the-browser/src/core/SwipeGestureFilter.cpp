#include "SwipeGestureFilter.hpp"

#include <QCoreApplication>
#include <algorithm>
#include <cmath>

SwipeGestureFilter::SwipeGestureFilter(QQuickItem *parent)
    : QQuickItem(parent) {
  m_inactivityTimer.setSingleShot(true);
  m_inactivityTimer.setInterval(280);
  connect(&m_inactivityTimer, &QTimer::timeout, this,
          &SwipeGestureFilter::commitGesture);

  if (qApp) {
    qApp->installEventFilter(this);
  }
}

SwipeGestureFilter::~SwipeGestureFilter() {
  if (qApp) {
    qApp->removeEventFilter(this);
  }
}

double SwipeGestureFilter::backProgress() const { return m_backProgress; }

double SwipeGestureFilter::forwardProgress() const { return m_forwardProgress; }

bool SwipeGestureFilter::active() const { return m_active; }

bool SwipeGestureFilter::canGoBack() const { return m_canGoBack; }

void SwipeGestureFilter::setCanGoBack(bool can) {
  if (m_canGoBack != can) {
    m_canGoBack = can;
    emit canGoBackChanged();
  }
}

bool SwipeGestureFilter::canGoForward() const { return m_canGoForward; }

void SwipeGestureFilter::setCanGoForward(bool can) {
  if (m_canGoForward != can) {
    m_canGoForward = can;
    emit canGoForwardChanged();
  }
}

double SwipeGestureFilter::threshold() const { return m_threshold; }

void SwipeGestureFilter::setThreshold(double val) {
  if (m_threshold != val && val > 0.0) {
    m_threshold = val;
    emit thresholdChanged();
  }
}

// SwipeGestureFilter::~SwipeGestureFilter() = default;

bool SwipeGestureFilter::eventFilter(QObject *watched, QEvent *event) {
  if (event->type() != QEvent::Wheel)
    return QQuickItem::eventFilter(watched, event);

  auto *we = static_cast<QWheelEvent *>(event);

  QPointF localPos = mapFromScene(we->position());

  if (!contains(localPos)) {
    return QQuickItem::eventFilter(watched, event);
  }

  // New gesture starts.
  if (we->phase() == Qt::ScrollBegin) {
    m_hasPhase = true;
    m_gestureRejected = false;
    m_inactivityTimer.stop();
  }

  // If this gesture started vertically, completely ignore it.
  if (m_gestureRejected) {
    if (we->phase() == Qt::ScrollEnd ||
        we->phase() == Qt::NoScrollPhase) {
      m_gestureRejected = false;
      m_hasPhase = false;
    }

    return QQuickItem::eventFilter(watched, event);
  }

  if (we->phase() == Qt::ScrollMomentum) {
    return m_active ? true : QQuickItem::eventFilter(watched, event);
  }

  if (we->phase() == Qt::ScrollEnd) {
    if (m_active) {
      commitGesture();
      return true;
    }

    m_hasPhase = false;
    return QQuickItem::eventFilter(watched, event);
  }

  QPoint pixelDelta = we->pixelDelta();
  QPoint angleDelta = we->angleDelta();

  double dx = pixelDelta.x() != 0
                  ? pixelDelta.x()
                  : (angleDelta.x() / 4.0);

  double dy = pixelDelta.y() != 0
                  ? pixelDelta.y()
                  : (angleDelta.y() / 4.0);

  // Don't classify zero movement.
  if (dx == 0.0 && dy == 0.0)
    return QQuickItem::eventFilter(watched, event);

  // ---------------------------------------------------------
  // AXIS LOCK
  //
  // The first meaningful movement decides the gesture axis.
  // If it starts vertically, abandon the gesture entirely.
  // ---------------------------------------------------------
  if (!m_active) {

    if (std::abs(dy) >= std::abs(dx)) {
      m_gestureRejected = true;
      return QQuickItem::eventFilter(watched, event);
    }

    // Horizontal gesture.
    if (std::abs(dx) < 2.0)
      return QQuickItem::eventFilter(watched, event);

    if (dx > 0 && !m_canGoBack)
      return QQuickItem::eventFilter(watched, event);

    if (dx < 0 && !m_canGoForward)
      return QQuickItem::eventFilter(watched, event);

    m_active = true;
    emit activeChanged();
  }

  m_accumulatedX += dx;

  if (m_accumulatedX > 0 && m_canGoBack) {
    m_backProgress =
        std::min(1.25, m_accumulatedX / m_threshold);
    m_forwardProgress = 0.0;

  } else if (m_accumulatedX < 0 && m_canGoForward) {
    m_forwardProgress =
        std::min(1.25, -m_accumulatedX / m_threshold);
    m_backProgress = 0.0;

  } else {
    m_backProgress = 0.0;
    m_forwardProgress = 0.0;
  }

  emit gestureUpdated();

  if (we->phase() == Qt::ScrollBegin ||
      we->phase() == Qt::ScrollUpdate) {

    m_hasPhase = true;
    m_inactivityTimer.stop();

  } else if (we->phase() == Qt::NoScrollPhase && !m_hasPhase) {
    m_inactivityTimer.start();
  }

  if (std::abs(m_accumulatedX) > 10.0)
    return true;

  return QQuickItem::eventFilter(watched, event);
}

void SwipeGestureFilter::commitGesture() {
  m_inactivityTimer.stop();

  if (m_accumulatedX >= m_threshold && m_canGoBack) {
    emit backTriggered();
  } else if (m_accumulatedX <= -m_threshold && m_canGoForward) {
    emit forwardTriggered();
  }

  reset();
}

void SwipeGestureFilter::reset() {
  m_inactivityTimer.stop();
  m_accumulatedX = 0.0;

  if (m_active) {
    m_active = false;
    emit activeChanged();
  }

  m_backProgress = 0.0;
  m_forwardProgress = 0.0;
  emit gestureUpdated();
}
