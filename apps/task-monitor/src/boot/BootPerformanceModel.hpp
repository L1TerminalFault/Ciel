#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QString>
#include <QThread>
#include <QVariantList>
#include <QtQml/QQmlEngine>
#include <qtmetamacros.h>

struct BootServiceEntry {
  QString unitName;
  quint64 durationMs = 0;
  QString durationString;
  double relativeRatio = 0.0;
};

struct BootScanPayload {
  double firmwareSec = 0.0;
  double loaderSec = 0.0;
  double kernelSec = 0.0;
  double initrdSec = 0.0;
  double userspaceSec = 0.0;
  double totalSec = 0.0;
  QList<BootServiceEntry> services;
};
Q_DECLARE_METATYPE(BootScanPayload)

class BootScanWorker : public QObject {
  Q_OBJECT

public:
  explicit BootScanWorker(QObject *parent = nullptr);
  ~BootScanWorker() override = default;

public slots:
  void performScan();

signals:
  void scanCompleted(const BootScanPayload &payload);
};

class BootPerformanceModel : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(double firmwareSec READ firmwareSec NOTIFY bootTimesChanged)
  Q_PROPERTY(double loaderSec READ loaderSec NOTIFY bootTimesChanged)
  Q_PROPERTY(double kernelSec READ kernelSec NOTIFY bootTimesChanged)
  Q_PROPERTY(double initrdSec READ initrdSec NOTIFY bootTimesChanged)
  Q_PROPERTY(double userspaceSec READ userspaceSec NOTIFY bootTimesChanged)
  Q_PROPERTY(double totalSec READ totalSec NOTIFY bootTimesChanged)
  Q_PROPERTY(
      QString totalTimeString READ totalTimeString NOTIFY bootTimesChanged)
  Q_PROPERTY(QVariantList segments READ segments NOTIFY bootTimesChanged)
  Q_PROPERTY(bool isLoading READ isLoading NOTIFY loadingChanged)

public:
  enum Roles {
    UnitNameRole = Qt::UserRole + 1,
    DurationMsRole,
    DurationStringRole,
    RelativeRatioRole
  };

  explicit BootPerformanceModel(QObject *parent = nullptr);
  ~BootPerformanceModel() override;

  Q_INVOKABLE int
  rowCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  double firmwareSec() const;
  double loaderSec() const;
  double kernelSec() const;
  double initrdSec() const;
  double userspaceSec() const;
  double totalSec() const;
  QString totalTimeString() const;
  QVariantList segments() const;
  bool isLoading() const;

  Q_INVOKABLE void refresh();

signals:
  void bootTimesChanged();
  void loadingChanged();
  void requestScan();

private slots:
  void onScanCompleted(const BootScanPayload &payload);

private:
  QThread m_workerThread;
  BootScanWorker *m_worker = nullptr;

  bool m_isLoading = false;
  double m_firmwareSec = 0.0;
  double m_loaderSec = 0.0;
  double m_kernelSec = 0.0;
  double m_initrdSec = 0.0;
  double m_userspaceSec = 0.0;
  double m_totalSec = 0.0;

  QList<BootServiceEntry> m_services;

  static QString formatDuration(quint64 ms);
};
