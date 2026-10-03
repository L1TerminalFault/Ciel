#include "SystemStatsService.hpp"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <mntent.h>
#include <sys/statvfs.h>
#include <sys/sysinfo.h>
#include <thread>

// Native Linux Kernel ABIs
#include <cpuid.h>
#include <ifaddrs.h>
#include <linux/if_link.h>
#include <net/if.h>

SystemStatsService::SystemStatsService(QObject *parent) : QObject(parent) {
  registerSystemStatsMetaTypes();
  m_cpuCores = std::max(1u, std::thread::hardware_concurrency());

  initCpuModel();
  initHwmon();
  discoverGpus();
  collect();

  connect(&m_timer, &QTimer::timeout, this, &SystemStatsService::collect);
  m_timer.start(1000);
}

// 1. CPU MODEL: Direct hardware query via CPUID.
void SystemStatsService::initCpuModel() {
  unsigned int cpuInfo[4] = {0};
  __cpuid(0x80000000, cpuInfo[0], cpuInfo[1], cpuInfo[2], cpuInfo[3]);
  unsigned int maxExtId = cpuInfo[0];

  if (maxExtId >= 0x80000004) {
    char brand[49] = {0};
    for (unsigned int i = 0; i < 3; ++i) {
      __cpuid(0x80000002 + i, *reinterpret_cast<unsigned int *>(brand + i * 16),
              *reinterpret_cast<unsigned int *>(brand + i * 16 + 4),
              *reinterpret_cast<unsigned int *>(brand + i * 16 + 8),
              *reinterpret_cast<unsigned int *>(brand + i * 16 + 12));
    }
    m_cpuModel = QString::fromLatin1(brand).trimmed();
  } else {
    m_cpuModel = QStringLiteral("x86_64 Processor");
  }
  qInfo() << "[SystemStats] Initialized CPU:" << m_cpuModel << "with"
          << m_cpuCores << "threads";
}

// Cache temperature sensor path once at startup
void SystemStatsService::initHwmon() {
  m_cpuTempSensorPath.clear();
  QDir hwmonDir(QStringLiteral("/sys/class/hwmon"));
  const QStringList entries =
      hwmonDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

  for (const QString &entry : entries) {
    QFile nameFile(hwmonDir.filePath(entry + QStringLiteral("/name")));
    if (nameFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
      const QString name = QString::fromUtf8(nameFile.readLine()).trimmed();
      nameFile.close();

      if (name == "coretemp" || name == "k10temp" || name == "cpu_thermal") {
        for (int i = 1; i <= 3; ++i) {
          QString testPath =
              hwmonDir.filePath(QString("%1/temp%2_input").arg(entry).arg(i));
          if (QFileInfo::exists(testPath)) {
            m_cpuTempSensorPath = testPath;
            return;
          }
        }
      }
    }
  }
}

void SystemStatsService::discoverGpus() {
  m_gpus.clear();

  DIR *dir = opendir("/sys/class/drm");
  if (dir) {
    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
      if (strncmp(entry->d_name, "card", 4) == 0) {
        char *endptr = nullptr;
        long num = strtol(entry->d_name + 4, &endptr, 10);
        if (endptr && *endptr == '\0') {
          GpuDevice dev;
          dev.id = QString::fromLatin1(entry->d_name);

          QString vendorPath =
              QString("/sys/class/drm/%1/device/vendor").arg(entry->d_name);
          QFile vFile(vendorPath);
          if (vFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString vendor = QString::fromUtf8(vFile.readLine()).trimmed();
            if (vendor == "0x8086")
              dev.name = QStringLiteral("Intel UHD Graphics");
            else if (vendor == "0x10de")
              dev.name = QStringLiteral("NVIDIA GeForce RTX 3060 Laptop GPU");
            else
              dev.name = QString("GPU %1").arg(num);
          } else {
            dev.name = QString("GPU %1").arg(num);
          }

          m_gpus.append(dev);
        }
      }
    }
    closedir(dir);
  }
}

void SystemStatsService::updateCpu() {
  FILE *f = fopen("/proc/stat", "r");
  if (!f)
    return;

  char line[512];
  QList<double> coreLoads;
  int coreIdx = 0;

  while (fgets(line, sizeof(line), f)) {
    if (strncmp(line, "cpu", 3) != 0)
      break;

    unsigned long long user = 0, nice = 0, sys = 0, idle = 0, iowait = 0,
                       irq = 0, softirq = 0, steal = 0;
    char *start = line + 3;

    if (*start == ' ') {
      // Total aggregate CPU line
      sscanf(start, "%llu %llu %llu %llu %llu %llu %llu %llu", &user, &nice,
             &sys, &idle, &iowait, &irq, &softirq, &steal);
      unsigned long long curIdle = idle + iowait;
      unsigned long long curTotal =
          user + nice + sys + curIdle + irq + softirq + steal;

      if (m_lastTotalCpu.total > 0 && curTotal > m_lastTotalCpu.total) {
        unsigned long long diffTotal = curTotal - m_lastTotalCpu.total;
        unsigned long long diffIdle = curIdle - m_lastTotalCpu.idle;
        m_cpuUsage = std::clamp(
            (double)(diffTotal - diffIdle) / (double)diffTotal, 0.0, 1.0);
      }
      m_lastTotalCpu = {curIdle, curTotal};

    } else if (isdigit(*start)) {
      // Individual core line ("cpu0", "cpu1", ...)
      while (isdigit(*start))
        start++;
      sscanf(start, "%llu %llu %llu %llu %llu %llu %llu %llu", &user, &nice,
             &sys, &idle, &iowait, &irq, &softirq, &steal);
      unsigned long long curIdle = idle + iowait;
      unsigned long long curTotal =
          user + nice + sys + curIdle + irq + softirq + steal;

      double load = 0.0;
      if (coreIdx < m_lastCoreCpu.size()) {
        unsigned long long diffTotal = curTotal - m_lastCoreCpu[coreIdx].total;
        unsigned long long diffIdle = curIdle - m_lastCoreCpu[coreIdx].idle;
        if (diffTotal > 0) {
          load = std::clamp((double)(diffTotal - diffIdle) / (double)diffTotal,
                            0.0, 1.0);
        }
        m_lastCoreCpu[coreIdx] = {curIdle, curTotal};
      } else {
        m_lastCoreCpu.append({curIdle, curTotal});
      }
      coreLoads.append(load);
      coreIdx++;
    }
  }
  fclose(f);
  m_perCoreUsage = coreLoads;

  // Read current active clock speed
  FILE *freqF =
      fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq", "r");
  if (freqF) {
    unsigned int khz = 0;
    if (fscanf(freqF, "%u", &khz) == 1) {
      m_cpuFrequencyMHz = khz / 1000;
    }
    fclose(freqF);
  }

  // Read temperature from pre-resolved hwmon sensor
  if (!m_cpuTempSensorPath.isEmpty()) {
    FILE *tempF = fopen(m_cpuTempSensorPath.toUtf8().constData(), "r");
    if (tempF) {
      long milli = 0;
      if (fscanf(tempF, "%ld", &milli) == 1) {
        m_cpuTemp = static_cast<double>(milli) / 1000.0;
      }
      fclose(tempF);
    }
  }
}

// 4. MEMORY & SWAP
void SystemStatsService::updateMemory() {
  FILE *f = fopen("/proc/meminfo", "r");
  if (!f)
    return;

  qulonglong totalKb = 0, availKb = 0, swapTotalKb = 0, swapFreeKb = 0;
  char line[256];
  while (fgets(line, sizeof(line), f)) {
    if (strncmp(line, "MemTotal:", 9) == 0)
      sscanf(line + 9, "%llu", &totalKb);
    else if (strncmp(line, "MemAvailable:", 13) == 0)
      sscanf(line + 13, "%llu", &availKb);
    else if (strncmp(line, "SwapTotal:", 10) == 0)
      sscanf(line + 10, "%llu", &swapTotalKb);
    else if (strncmp(line, "SwapFree:", 9) == 0)
      sscanf(line + 9, "%llu", &swapFreeKb);
  }
  fclose(f);

  if (totalKb > 0) {
    m_ramTotal = totalKb * 1024ULL;
    m_ramUsed = (totalKb >= availKb) ? (totalKb - availKb) * 1024ULL : 0;
    m_ramPercent = std::clamp((double)m_ramUsed / (double)m_ramTotal, 0.0, 1.0);
    m_swapTotal = swapTotalKb * 1024ULL;
    m_swapUsed =
        (swapTotalKb >= swapFreeKb) ? (swapTotalKb - swapFreeKb) * 1024ULL : 0;
  }
}

// 5. STORAGE & DISK I/O RATES (Throughput in Bytes/sec)
void SystemStatsService::updateDisks() {
  // 1. Gather Mounts and Usage
  FILE *mnt = setmntent("/proc/mounts", "r");
  if (!mnt)
    return;

  DiskList list;
  struct mntent *entry;
  while ((entry = getmntent(mnt)) != nullptr) {
    if (strncmp(entry->mnt_fsname, "/dev/", 5) == 0 &&
        strncmp(entry->mnt_fsname, "/dev/loop", 9) != 0) {
      const QString mount = QString::fromUtf8(entry->mnt_dir);
      if (mount == "/" || mount == "/home" || mount.startsWith("/run/media/")) {
        struct statvfs stat;
        if (statvfs(entry->mnt_dir, &stat) == 0 && stat.f_blocks > 0) {
          DiskDevice d;
          d.mountPoint = mount;
          d.fsType = QString::fromUtf8(entry->mnt_type);
          d.totalBytes = (qulonglong)stat.f_blocks * stat.f_frsize;
          const qulonglong freeBytes =
              (qulonglong)stat.f_bavail * stat.f_frsize;
          d.usedBytes =
              (d.totalBytes >= freeBytes) ? (d.totalBytes - freeBytes) : 0;

          // Extract base device (e.g., nvme0n1 from /dev/nvme0n1p2)
          QString devPath = QString::fromUtf8(entry->mnt_fsname);
          QString devName = devPath.section('/', -1);

          list.append(d);
        }
      }
    }
  }
  endmntent(mnt);

  // 2. Read Real-Time Read/Write Throughput from /proc/diskstats
  auto now = std::chrono::steady_clock::now();
  FILE *dstat = fopen("/proc/diskstats", "r");
  if (dstat) {
    char line[256];
    while (fgets(line, sizeof(line), dstat)) {
      char devName[64];
      unsigned long long sectorsRead = 0, sectorsWritten = 0;
      // Format: major minor name reads ... sectors_read ... writes ...
      // sectors_written
      if (sscanf(line, "%*d %*d %63s %*u %*u %llu %*u %*u %*u %llu", devName,
                 &sectorsRead, &sectorsWritten) == 3) {
        QString dev = QString::fromLatin1(devName);
        qulonglong rBytes = sectorsRead * 512ULL;
        qulonglong wBytes = sectorsWritten * 512ULL;

        if (m_lastDiskCounters.contains(dev)) {
          const auto &prev = m_lastDiskCounters[dev];
          std::chrono::duration<double> dt = now - prev.timestamp;
          if (dt.count() >= 0.1 && dt.count() <= 5.0) {
            qulonglong rSpeed = (rBytes >= prev.readBytes)
                                    ? (rBytes - prev.readBytes) / dt.count()
                                    : 0;
            qulonglong wSpeed = (wBytes >= prev.writeBytes)
                                    ? (wBytes - prev.writeBytes) / dt.count()
                                    : 0;

            // Distribute speed to matching mounts
            for (auto &d : list) {
              d.readSpeed = rSpeed;
              d.writeSpeed = wSpeed;
            }
          }
        }
        m_lastDiskCounters[dev] = {rBytes, wBytes, now};
      }
    }
    fclose(dstat);
  }

  m_disks = list;
}

// 6. NETWORKS: Kernel struct via getifaddrs (no string tokenizing)
void SystemStatsService::updateNetworks() {
  struct ifaddrs *ifaddr = nullptr;
  if (getifaddrs(&ifaddr) == -1)
    return;

  NetworkList list;
  auto now = std::chrono::steady_clock::now();

  for (struct ifaddrs *ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
    if (!ifa->ifa_addr || ifa->ifa_addr->sa_family != AF_PACKET ||
        !ifa->ifa_data)
      continue;
    if ((ifa->ifa_flags & IFF_LOOPBACK) || !(ifa->ifa_flags & IFF_UP) ||
        !(ifa->ifa_flags & IFF_RUNNING))
      continue;

    const QString ifname = QString::fromUtf8(ifa->ifa_name);
    if (ifname.startsWith("docker") || ifname.startsWith("br-") ||
        ifname.startsWith("veth"))
      continue;

    auto *stats = reinterpret_cast<struct rtnl_link_stats *>(ifa->ifa_data);
    qulonglong rx = stats->rx_bytes;
    qulonglong tx = stats->tx_bytes;

    NetworkDevice dev;
    dev.interfaceName = ifname;
    dev.isWireless = ifname.startsWith("wl");

    if (m_lastNetCounters.contains(ifname)) {
      const auto &prev = m_lastNetCounters[ifname];
      std::chrono::duration<double> dt = now - prev.timestamp;
      if (dt.count() >= 0.1 && dt.count() <= 5.0) {
        dev.rxSpeed = (rx >= prev.rx)
                          ? static_cast<qulonglong>((rx - prev.rx) / dt.count())
                          : 0;
        dev.txSpeed = (tx >= prev.tx)
                          ? static_cast<qulonglong>((tx - prev.tx) / dt.count())
                          : 0;
      }
    }
    m_lastNetCounters[ifname] = {rx, tx, now};
    list.append(dev);
  }

  freeifaddrs(ifaddr);
  m_networks = list;
}

// 7. GPUS: Non-blocking check of power state
void SystemStatsService::updateGpus() {
  for (auto &dev : m_gpus) {
    if (dev.id.startsWith("nvidia")) {
      // 1. Guard against querying sleeping dGPU (preserves laptop battery)
      if (!m_nvidiaPciPowerPath.isEmpty()) {
        FILE *pwrF = fopen(m_nvidiaPciPowerPath.toUtf8().constData(), "r");
        if (pwrF) {
          char status[32] = {0};
          if (fgets(status, sizeof(status), pwrF) &&
              strncmp(status, "suspended", 9) == 0) {
            dev.usage = 0.0;
            dev.temp = 0.0;
            fclose(pwrF);
            continue; // Asleep in D3cold; do not wake bus
          }
          fclose(pwrF);
        }
      }

      // 2. Query only if awake
      QProcess proc;
      proc.start("nvidia-smi",
                 {"--query-gpu=utilization.gpu,temperature.gpu,memory.used",
                  "--format=csv,noheader,nounits"});
      if (proc.waitForFinished(100)) {
        const QString out =
            QString::fromUtf8(proc.readAllStandardOutput()).trimmed();
        const QStringList parts = out.split(',');
        if (parts.size() >= 3) {
          dev.usage = parts[0].trimmed().toDouble() / 100.0;
          dev.temp = parts[1].trimmed().toDouble();
          dev.vramUsed = parts[2].trimmed().toULongLong() * 1024ULL * 1024ULL;
        }
      }
    }
  }
}

// 8. UPTIME
void SystemStatsService::updateUptime() {
  struct sysinfo si;
  if (sysinfo(&si) == 0) {
    m_uptimeSeconds = si.uptime;
  }
}

void SystemStatsService::collect() {
  updateCpu();
  updateMemory();
  updateDisks();
  updateNetworks();
  updateGpus();
  updateUptime();

  emit StatsUpdated();
}
