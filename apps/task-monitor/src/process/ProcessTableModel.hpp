#pragma once

#include <QAbstractTableModel>
#include <QElapsedTimer>
#include <QHash>
#include <QList>
#include <QSet>
#include <QString>
#include <QThread>
#include <QVariantList>
#include <QtQml/QQmlEngine>

#include "SystemStatsInterface.h"

struct ProcessNode {
  int pid;
  int ppid;
  QString name;
  QString detailName;
  double cpuPercent;
  qulonglong ramBytes;
  qulonglong diskBytesSec;
  qulonglong netBytesSec;
};

struct CpuTicks {
  quint64 total{0};
  quint64 idle{0};
};

struct ProcessGroup {
  QString groupName;
  int rootPid;
  double totalCpu = 0.0;
  qulonglong totalRam = 0;
  qulonglong totalDisk = 0;
  qulonglong totalNet = 0;
  QList<ProcessNode> children;
};

struct ProcessStaticInfo {
  QString name;
  QString detailName;
  int ppid = 0;
};

struct WorkerScanResult {
  double systemCpuUsage = 0.0;
  double systemRamUsagePercent = 0.0;
  qulonglong systemRamUsedBytes = 0;
  qulonglong systemRamTotalBytes = 0;
  qulonglong systemRamAvailBytes = 0;
  qulonglong systemSwapUsedBytes = 0;
  qulonglong systemSwapTotalBytes = 0;

  qulonglong systemDiskBytesSec = 0;
  qulonglong systemDiskReadBytesSec = 0;
  qulonglong systemDiskWriteBytesSec = 0;

  qulonglong systemNetBytesSec = 0;
  qulonglong systemNetRxBytesSec = 0;
  qulonglong systemNetTxBytesSec = 0;

  QList<double> perCoreUsage;
  GpuList gpus;
  DiskList disks;
  NetworkList networks;
  QList<ProcessGroup> allGroups;
};
Q_DECLARE_METATYPE(WorkerScanResult)

class ProcessScanWorker : public QObject {
  Q_OBJECT

public:
  explicit ProcessScanWorker(QObject *parent = nullptr);
  ~ProcessScanWorker() override = default;

public slots:
  void performScan(int sortColumn, int sortOrder);

signals:
  void scanCompleted(const WorkerScanResult &result);

private:
  long m_pageSize = 4096;
  int m_cpuOnlineCores = 1;

  quint64 m_prevTotalSystemTicks = 0;
  quint64 m_prevIdleSystemTicks = 0;
  QList<CpuTicks> m_prevCoreTicks;

  QHash<int, quint64> m_prevProcTicks;
  QHash<int, quint64> m_prevDiskBytes;
  QHash<int, double> m_prevSmoothedCpu;
  QHash<int, ProcessStaticInfo> m_staticCache;

  quint64 m_prevDiskReadBytes = 0;
  quint64 m_prevDiskWriteBytes = 0;
  quint64 m_prevNetRxBytes = 0;
  quint64 m_prevNetTxBytes = 0;

  QElapsedTimer m_ioTimer;
  int m_mountCheckCounter = 0;
  DiskList m_cachedDisks;
  GpuList m_cachedGpuMeta;
  bool m_gpusInitialized = false;

  void readSystemStats(quint64 &outTotal, quint64 &outIdle);
  void readPerCoreStats(QList<double> &outUsages);
  void readSystemRam(WorkerScanResult &res);
  void readSystemDiskStats(WorkerScanResult &res);
  void readSystemNetStats(WorkerScanResult &res);
  void initGpuDevices();
  void readGpuStats(WorkerScanResult &res);
  void scanProcesses(QList<ProcessNode> &outList, quint64 totalSystemTicks,
                     WorkerScanResult &res);
  void buildGroups(const QList<ProcessNode> &rawNodes,
                   QList<ProcessGroup> &outGroups, int sortColumn,
                   int sortOrder);

  QString extractProcessName(int pid, const QString &fallbackComm);
  QString extractDetailName(int pid, const QString &fallbackComm);
};

class ProcessTableModel : public QAbstractTableModel {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(QString filterText READ filterText WRITE setFilterText NOTIFY
                 filterTextChanged)
  Q_PROPERTY(bool isSearching READ isSearching NOTIFY isSearchingChanged)
  Q_PROPERTY(int sortColumn READ sortColumn WRITE setSortColumn NOTIFY
                 sortColumnChanged)
  Q_PROPERTY(
      int sortOrder READ sortOrder WRITE setSortOrder NOTIFY sortOrderChanged)

  Q_PROPERTY(
      double systemCpuUsage READ systemCpuUsage NOTIFY systemTotalsChanged)
  Q_PROPERTY(double systemRamUsagePercent READ systemRamUsagePercent NOTIFY
                 systemTotalsChanged)
  Q_PROPERTY(qulonglong systemRamUsedBytes READ systemRamUsedBytes NOTIFY
                 systemTotalsChanged)
  Q_PROPERTY(qulonglong systemRamTotalBytes READ systemRamTotalBytes NOTIFY
                 systemTotalsChanged)
  Q_PROPERTY(qulonglong systemRamAvailBytes READ systemRamAvailBytes NOTIFY
                 systemTotalsChanged)
  Q_PROPERTY(qulonglong systemSwapUsedBytes READ systemSwapUsedBytes NOTIFY
                 systemTotalsChanged)
  Q_PROPERTY(qulonglong systemSwapTotalBytes READ systemSwapTotalBytes NOTIFY
                 systemTotalsChanged)

  Q_PROPERTY(
      QString systemDiskSpeed READ systemDiskSpeed NOTIFY systemTotalsChanged)
  Q_PROPERTY(qulonglong systemDiskBytesSec READ systemDiskBytesSec NOTIFY
                 systemTotalsChanged)
  Q_PROPERTY(qulonglong systemDiskReadBytesSec READ systemDiskReadBytesSec
                 NOTIFY systemTotalsChanged)
  Q_PROPERTY(qulonglong systemDiskWriteBytesSec READ systemDiskWriteBytesSec
                 NOTIFY systemTotalsChanged)

  Q_PROPERTY(
      QString systemNetSpeed READ systemNetSpeed NOTIFY systemTotalsChanged)
  Q_PROPERTY(qulonglong systemNetBytesSec READ systemNetBytesSec NOTIFY
                 systemTotalsChanged)
  Q_PROPERTY(qulonglong systemNetRxBytesSec READ systemNetRxBytesSec NOTIFY
                 systemTotalsChanged)
  Q_PROPERTY(qulonglong systemNetTxBytesSec READ systemNetTxBytesSec NOTIFY
                 systemTotalsChanged)

  Q_PROPERTY(QList<double> cpuHistory READ cpuHistory NOTIFY historyUpdated)
  Q_PROPERTY(QList<double> ramHistory READ ramHistory NOTIFY historyUpdated)
  Q_PROPERTY(QList<double> diskHistory READ diskHistory NOTIFY historyUpdated)
  Q_PROPERTY(QList<double> netHistory READ netHistory NOTIFY historyUpdated)
  Q_PROPERTY(
      QList<double> perCoreUsage READ perCoreUsage NOTIFY perCoreUsageChanged)
  Q_PROPERTY(int coreCount READ coreCount CONSTANT)
  Q_PROPERTY(int gpuCount READ gpuCount NOTIFY gpusChanged)
  Q_PROPERTY(int diskCount READ diskCount NOTIFY disksChanged)
  Q_PROPERTY(int networkCount READ networkCount NOTIFY networksChanged)
  Q_PROPERTY(int historyRevision READ historyRevision NOTIFY historyUpdated)

public:
  enum Roles {
    PidRole = Qt::UserRole + 1,
    NameRole,
    CpuRole,
    RamRole,
    DiskRole,
    NetRole,
    ChildCountRole,
    ChildrenRole
  };

  explicit ProcessTableModel(QObject *parent = nullptr);
  ~ProcessTableModel() override;

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QHash<int, QByteArray> roleNames() const override;

  QString filterText() const;
  void setFilterText(const QString &text);
  bool isSearching() const;

  int sortColumn() const;
  void setSortColumn(int column);
  int sortOrder() const;
  void setSortOrder(int order);

  QList<double> cpuHistory() const;
  QList<double> ramHistory() const;
  QList<double> diskHistory() const;
  QList<double> netHistory() const;
  QList<double> perCoreUsage() const;
  int coreCount() const;
  int gpuCount() const;
  int diskCount() const;
  int networkCount() const;
  int historyRevision() const;

  double systemCpuUsage() const;
  double systemRamUsagePercent() const;
  qulonglong systemRamUsedBytes() const;
  qulonglong systemRamTotalBytes() const;
  qulonglong systemRamAvailBytes() const;
  qulonglong systemSwapUsedBytes() const;
  qulonglong systemSwapTotalBytes() const;

  QString systemDiskSpeed() const;
  qulonglong systemDiskBytesSec() const;
  qulonglong systemDiskReadBytesSec() const;
  qulonglong systemDiskWriteBytesSec() const;

  QString systemNetSpeed() const;
  qulonglong systemNetBytesSec() const;
  qulonglong systemNetRxBytesSec() const;
  qulonglong systemNetTxBytesSec() const;

  Q_INVOKABLE QList<double> coreHistory(int coreIndex) const;

  Q_INVOKABLE QString gpuName(int index) const;
  Q_INVOKABLE QString gpuId(int index) const;
  Q_INVOKABLE double gpuUsage(int index) const;
  Q_INVOKABLE double gpuTemp(int index) const;
  Q_INVOKABLE qulonglong gpuVramUsed(int index) const;
  Q_INVOKABLE qulonglong gpuVramTotal(int index) const;
  Q_INVOKABLE QList<double> gpuHistory(int index) const;

  Q_INVOKABLE QString diskMount(int index) const;
  Q_INVOKABLE QString diskFsType(int index) const;
  Q_INVOKABLE qulonglong diskTotalBytes(int index) const;
  Q_INVOKABLE qulonglong diskUsedBytes(int index) const;
  Q_INVOKABLE qulonglong diskReadSpeed(int index) const;
  Q_INVOKABLE qulonglong diskWriteSpeed(int index) const;

  Q_INVOKABLE QString networkName(int index) const;
  Q_INVOKABLE bool networkIsWireless(int index) const;
  Q_INVOKABLE qulonglong networkRxSpeed(int index) const;
  Q_INVOKABLE qulonglong networkTxSpeed(int index) const;

  Q_INVOKABLE QString formatSpeed(qulonglong bytesSec) const;
  Q_INVOKABLE QString formatBytes(qulonglong bytes) const;

  Q_INVOKABLE void sortByColumn(int column, int order = Qt::DescendingOrder);
  Q_INVOKABLE void refresh();

signals:
  void filterTextChanged();
  void isSearchingChanged();
  void sortColumnChanged();
  void sortOrderChanged();
  void systemTotalsChanged();
  void historyUpdated();
  void perCoreUsageChanged();
  void gpusChanged();
  void disksChanged();
  void networksChanged();
  void requestScan(int sortColumn, int sortOrder);

private slots:
  void onScanCompleted(const WorkerScanResult &result);

private:
  static constexpr int HistoryLength = 50;

  QList<ProcessGroup> m_allGroups;
  QList<ProcessGroup> m_visibleGroups;
  OrgCielSystemStatsInterface *m_interface = nullptr;

  QThread m_workerThread;
  ProcessScanWorker *m_worker = nullptr;
  bool m_scanInProgress = false;
  bool m_pendingScan = false;

  QString m_filterText;
  int m_sortColumn = 3;
  int m_sortOrder = Qt::DescendingOrder;

  double m_systemCpuUsage = 0.0;
  double m_systemRamUsagePercent = 0.0;
  qulonglong m_systemRamUsedBytes = 0;
  qulonglong m_systemRamTotalBytes = 0;
  qulonglong m_systemRamAvailBytes = 0;
  qulonglong m_systemSwapUsedBytes = 0;
  qulonglong m_systemSwapTotalBytes = 0;

  qulonglong m_systemDiskBytesSec = 0;
  qulonglong m_systemDiskReadBytesSec = 0;
  qulonglong m_systemDiskWriteBytesSec = 0;

  qulonglong m_systemNetBytesSec = 0;
  qulonglong m_systemNetRxBytesSec = 0;
  qulonglong m_systemNetTxBytesSec = 0;

  double m_diskPeakRate = 1048576.0;
  double m_netPeakRate = 1048576.0;

  int m_cpuOnlineCores = 1;
  int m_historyRevision{0};
  QList<double> m_perCoreUsage;
  QList<double> m_cpuHistory;
  QList<double> m_ramHistory;
  QList<double> m_diskHistory;
  QList<double> m_netHistory;
  QList<QList<double>> m_perCoreHistory;

  GpuList m_gpus;
  QList<QList<double>> m_gpuHistories;
  DiskList m_disks;
  NetworkList m_networks;

  void appendHistory(QList<double> &buffer, double val);
  void updateVisibleGroups();
};
