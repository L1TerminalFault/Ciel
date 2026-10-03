#pragma once

#include "SystemStatsTypes.hpp"
#include <QMap>
#include <QObject>
#include <QString>
#include <QTimer>
#include <chrono>

class SystemStatsService : public QObject {
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "org.ciel.SystemStats")
  Q_PROPERTY(DoubleList PerCoreUsage READ PerCoreUsage NOTIFY StatsUpdated)
  // Properties matching org.ciel.SystemStats.xml
  Q_PROPERTY(QString CpuModel READ CpuModel NOTIFY StatsUpdated)
  Q_PROPERTY(uint CpuCores READ CpuCores NOTIFY StatsUpdated)
  Q_PROPERTY(double CpuUsage READ CpuUsage NOTIFY StatsUpdated)
  Q_PROPERTY(double CpuTemp READ CpuTemp NOTIFY StatsUpdated)
  Q_PROPERTY(uint CpuFrequencyMHz READ CpuFrequencyMHz NOTIFY StatsUpdated)

  Q_PROPERTY(qulonglong RamTotalBytes READ RamTotalBytes NOTIFY StatsUpdated)
  Q_PROPERTY(qulonglong RamUsedBytes READ RamUsedBytes NOTIFY StatsUpdated)
  Q_PROPERTY(double RamUsagePercent READ RamUsagePercent NOTIFY StatsUpdated)
  Q_PROPERTY(qulonglong SwapTotalBytes READ SwapTotalBytes NOTIFY StatsUpdated)
  Q_PROPERTY(qulonglong SwapUsedBytes READ SwapUsedBytes NOTIFY StatsUpdated)

  Q_PROPERTY(GpuList Gpus READ Gpus NOTIFY StatsUpdated)
  Q_PROPERTY(DiskList Disks READ Disks NOTIFY StatsUpdated)
  Q_PROPERTY(NetworkList Networks READ Networks NOTIFY StatsUpdated)
  Q_PROPERTY(qulonglong UptimeSeconds READ UptimeSeconds NOTIFY StatsUpdated)

public:
  explicit SystemStatsService(QObject *parent = nullptr);

  // Exact PascalCase Getters called by generated D-Bus Adaptor
  QString CpuModel() const { return m_cpuModel; }
  uint CpuCores() const { return m_cpuCores; }
  double CpuUsage() const { return m_cpuUsage; }
  double CpuTemp() const { return m_cpuTemp; }
  uint CpuFrequencyMHz() const { return m_cpuFrequencyMHz; }

  DoubleList PerCoreUsage() const { return m_perCoreUsage; }
  qulonglong RamTotalBytes() const { return m_ramTotal; }
  qulonglong RamUsedBytes() const { return m_ramUsed; }
  double RamUsagePercent() const { return m_ramPercent; }
  qulonglong SwapTotalBytes() const { return m_swapTotal; }
  qulonglong SwapUsedBytes() const { return m_swapUsed; }

  GpuList Gpus() const { return m_gpus; }
  DiskList Disks() const { return m_disks; }
  NetworkList Networks() const { return m_networks; }
  qulonglong UptimeSeconds() const { return m_uptimeSeconds; }

signals:
  void StatsUpdated();

private slots:
  void collect();

private:
  void initCpuModel();
  void initHwmon();
  void discoverGpus();

  void updateCpu();
  void updateMemory();
  void updateDisks();
  void updateNetworks();
  void updateGpus();
  void updateUptime();

  QTimer m_timer;

  // CPU
  QString m_cpuModel = QStringLiteral("Unknown CPU");
  uint m_cpuCores = 1;
  double m_cpuUsage = 0.0;
  double m_cpuTemp = 0.0;
  uint m_cpuFrequencyMHz = 0;
  QString m_cpuTempSensorPath;

  struct CpuSnap {
    unsigned long long idle = 0;
    unsigned long long total = 0;
  };
  CpuSnap m_lastTotalCpu;
  QList<CpuSnap> m_lastCoreCpu;

  // Memory
  qulonglong m_ramTotal = 0;
  qulonglong m_ramUsed = 0;
  double m_ramPercent = 0.0;
  qulonglong m_swapTotal = 0;
  qulonglong m_swapUsed = 0;

  // Storage
  DiskList m_disks;
  struct DiskIoSnap {
    qulonglong readBytes = 0;
    qulonglong writeBytes = 0;
    std::chrono::steady_clock::time_point timestamp;
  };
  QMap<QString, DiskIoSnap> m_lastDiskCounters;

  // Networks
  NetworkList m_networks;
  struct NetSnap {
    qulonglong rx = 0;
    qulonglong tx = 0;
    std::chrono::steady_clock::time_point timestamp;
  };
  QMap<QString, NetSnap> m_lastNetCounters;

  // GPUs
  GpuList m_gpus;
  QString m_nvidiaPciPowerPath;
  DoubleList m_perCoreUsage;
  // System
  qulonglong m_uptimeSeconds = 0;
};
