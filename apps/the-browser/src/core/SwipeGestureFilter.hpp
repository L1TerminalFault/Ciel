#pragma once

#include <QEvent>
#include <QObject>
#include <QTimer>
#include <QWheelEvent>
#include <QtQml/qqmlregistration.h>

class SwipeGestureFilter : public QObject {
  Q_OBJECT
  QML_ELEMENT
  Q_PROPERTY(double backProgress READ backProgress NOTIFY gestureUpdated)
  Q_PROPERTY(double forwardProgress READ forwardProgress NOTIFY gestureUpdated)
  Q_PROPERTY(bool active READ active NOTIFY activeChanged)
  Q_PROPERTY(
      bool canGoBack READ canGoBack WRITE setCanGoBack NOTIFY canGoBackChanged)
  Q_PROPERTY(bool canGoForward READ canGoForward WRITE setCanGoForward NOTIFY
                 canGoForwardChanged)
  Q_PROPERTY(double threshold READ threshold WRITE setThreshold NOTIFY
                 thresholdChanged)

public:
  explicit SwipeGestureFilter(QObject *parent = nullptr);
  ~SwipeGestureFilter() override;

  double backProgress() const;
  double forwardProgress() const;
  bool active() const;

  bool canGoBack() const;
  void setCanGoBack(bool can);

  bool canGoForward() const;
  void setCanGoForward(bool can);

  double threshold() const;
  void setThreshold(double val);

  Q_INVOKABLE void reset();

signals:
  void gestureUpdated();
  void activeChanged();
  void backTriggered();
  void forwardTriggered();
  void canGoBackChanged();
  void canGoForwardChanged();
  void thresholdChanged();

protected:
  bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
  void commitGesture();

private:
  double m_accumulatedX{0.0};
  double m_backProgress{0.0};
  double m_forwardProgress{0.0};
  bool m_active{false};
  bool m_canGoBack{false};
  bool m_canGoForward{false};
  bool m_hasPhase{false};
  double m_threshold{90.0};

  QTimer m_inactivityTimer;
};
