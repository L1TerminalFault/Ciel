#include "ProcessTableModel.hpp"

#include <QDBusConnection>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <algorithm>
#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <sys/statvfs.h>
#include <unistd.h>

ProcessScanWorker::ProcessScanWorker(QObject *parent) : QObject(parent) {
  m_pageSize = sysconf(_SC_PAGESIZE);
  m_cpuOnlineCores =
      std::max(1, static_cast<int>(sysconf(_SC_NPROCESSORS_ONLN)));
  m_ioTimer.start();
  initGpuDevices();
}

void ProcessScanWorker::initGpuDevices() {
  m_cachedGpuMeta.clear();
  QDir drmDir("/sys/class/drm");
  QStringList cardDirs = drmDir.entryList(QStringList() << "card[0-9]*",
                                          QDir::Dirs | QDir::NoDotAndDotDot);

  for (const QString &cardName : cardDirs) {
    if (cardName.contains('-'))
      continue;

    QString devicePath = "/sys/class/drm/" + cardName + "/device";
    int fd = open((devicePath + "/vendor").toUtf8().constData(),
                  O_RDONLY | O_CLOEXEC);
    if (fd < 0)
      continue;

    char buf[64];
    ssize_t n = read(fd, buf, sizeof(buf) - 1);
    close(fd);
    if (n <= 0)
      continue;
    buf[n] = '\0';
    QString vendorId = QString::fromUtf8(buf).trimmed().toLower();

    GpuDevice gpu;
    gpu.id = cardName;

    if (vendorId.contains("0x10de")) {
      gpu.name = "NVIDIA Dedicated GPU";
      gpu.vramTotal = 6ULL * 1024ULL * 1024ULL * 1024ULL;
    } else if (vendorId.contains("0x8086")) {
      gpu.name = "Intel Integrated Graphics";
      gpu.vramTotal = 4ULL * 1024ULL * 1024ULL * 1024ULL;
    } else if (vendorId.contains("0x1002")) {
      gpu.name = "AMD Radeon GPU";
      gpu.vramTotal = 4ULL * 1024ULL * 1024ULL * 1024ULL;
    } else {
      gpu.name = "Graphics Processor (" + cardName + ")";
      gpu.vramTotal = 2ULL * 1024ULL * 1024ULL * 1024ULL;
    }
    m_cachedGpuMeta.append(gpu);
  }

  if (m_cachedGpuMeta.isEmpty()) {
    GpuDevice gpu;
    gpu.id = "gpu0";
    gpu.name = "Graphics Processor";
    gpu.vramTotal = 2ULL * 1024ULL * 1024ULL * 1024ULL;
    m_cachedGpuMeta.append(gpu);
  }
  m_gpusInitialized = true;
}

QString ProcessScanWorker::extractProcessName(int pid,
                                              const QString &fallbackComm) {
  char exeLinkPath[64];
  snprintf(exeLinkPath, sizeof(exeLinkPath), "/proc/%d/exe", pid);
  char exeBuf[PATH_MAX];
  ssize_t len = readlink(exeLinkPath, exeBuf, sizeof(exeBuf) - 1);
  if (len > 0) {
    exeBuf[len] = '\0';
    char *slash = strrchr(exeBuf, '/');
    const char *base = slash ? (slash + 1) : exeBuf;
    if (base[0] != '\0' && strcmp(base, "exe") != 0) {
      return QString::fromUtf8(base);
    }
  }

  char cmdPath[64];
  snprintf(cmdPath, sizeof(cmdPath), "/proc/%d/cmdline", pid);
  int fd = open(cmdPath, O_RDONLY | O_CLOEXEC);
  if (fd >= 0) {
    char cmdBuf[512];
    ssize_t n = read(fd, cmdBuf, sizeof(cmdBuf) - 1);
    close(fd);
    if (n > 0) {
      cmdBuf[n] = '\0';
      char *slash = strrchr(cmdBuf, '/');
      const char *base = slash ? (slash + 1) : cmdBuf;
      QString baseStr = QString::fromUtf8(base).trimmed();
      if (!baseStr.isEmpty() && !baseStr.contains("tab", Qt::CaseInsensitive) &&
          !baseStr.contains("isolated", Qt::CaseInsensitive)) {
        return baseStr;
      }
    }
  }

  return fallbackComm;
}

QString ProcessScanWorker::extractDetailName(int pid,
                                             const QString &fallbackComm) {
  char cmdPath[64];
  snprintf(cmdPath, sizeof(cmdPath), "/proc/%d/cmdline", pid);
  int fd = open(cmdPath, O_RDONLY | O_CLOEXEC);
  if (fd >= 0) {
    char cmdBuf[1024];
    ssize_t n = read(fd, cmdBuf, sizeof(cmdBuf) - 1);
    close(fd);
    if (n > 0) {
      ssize_t idx = 0;
      while (idx < n) {
        const char *arg = cmdBuf + idx;
        size_t argLen = strlen(arg);
        if (strncmp(arg, "--type=", 7) == 0) {
          return QString::fromUtf8(arg + 7);
        }
        idx += argLen + 1;
      }
      QString first = QString::fromUtf8(cmdBuf).trimmed();
      if (first.contains("tab", Qt::CaseInsensitive) ||
          first.contains("web", Qt::CaseInsensitive)) {
        return first;
      }
    }
  }
  return fallbackComm;
}

void ProcessScanWorker::readSystemStats(quint64 &outTotal, quint64 &outIdle) {
  int fd = open("/proc/stat", O_RDONLY | O_CLOEXEC);
  if (fd < 0) {
    outTotal = 0;
    outIdle = 0;
    return;
  }

  char buf[1024];
  ssize_t n = read(fd, buf, sizeof(buf) - 1);
  close(fd);
  if (n <= 0) {
    outTotal = 0;
    outIdle = 0;
    return;
  }
  buf[n] = '\0';

  if (strncmp(buf, "cpu ", 4) != 0) {
    outTotal = 0;
    outIdle = 0;
    return;
  }

  char *ptr = buf + 4;
  quint64 user = strtoull(ptr, &ptr, 10);
  quint64 nice = strtoull(ptr, &ptr, 10);
  quint64 sys = strtoull(ptr, &ptr, 10);
  quint64 idle = strtoull(ptr, &ptr, 10);
  quint64 iowait = strtoull(ptr, &ptr, 10);
  quint64 irq = strtoull(ptr, &ptr, 10);
  quint64 softirq = strtoull(ptr, &ptr, 10);
  quint64 steal = strtoull(ptr, &ptr, 10);

  outTotal = user + nice + sys + idle + iowait + irq + softirq + steal;
  outIdle = idle + iowait;
}

void ProcessScanWorker::readPerCoreStats(QList<double> &outUsages) {
  int fd = open("/proc/stat", O_RDONLY | O_CLOEXEC);
  if (fd < 0)
    return;

  char buf[4096];
  ssize_t n = read(fd, buf, sizeof(buf) - 1);
  close(fd);
  if (n <= 0)
    return;
  buf[n] = '\0';

  char *line = strchr(buf, '\n');
  int coreIdx = 0;

  while (line && *line) {
    line++;
    if (strncmp(line, "cpu", 3) != 0 || line[3] < '0' || line[3] > '9')
      break;

    char *ptr = line + 3;
    strtoul(ptr, &ptr, 10);

    quint64 user = strtoull(ptr, &ptr, 10);
    quint64 nice = strtoull(ptr, &ptr, 10);
    quint64 sys = strtoull(ptr, &ptr, 10);
    quint64 idle = strtoull(ptr, &ptr, 10);
    quint64 iowait = strtoull(ptr, &ptr, 10);
    quint64 irq = strtoull(ptr, &ptr, 10);
    quint64 softirq = strtoull(ptr, &ptr, 10);
    quint64 steal = strtoull(ptr, &ptr, 10);

    quint64 total = user + nice + sys + idle + iowait + irq + softirq + steal;
    quint64 totalIdle = idle + iowait;

    if (coreIdx >= m_prevCoreTicks.size()) {
      m_prevCoreTicks.append({total, totalIdle});
      outUsages.append(0.0);
      coreIdx++;
      line = strchr(ptr, '\n');
      continue;
    }

    const auto &prev = m_prevCoreTicks[coreIdx];
    double usage = 0.0;
    if (total > prev.total) {
      quint64 deltaTotal = total - prev.total;
      quint64 deltaIdle =
          (totalIdle >= prev.idle) ? (totalIdle - prev.idle) : 0;
      if (deltaTotal >= deltaIdle) {
        usage = (static_cast<double>(deltaTotal - deltaIdle) /
                 static_cast<double>(deltaTotal)) *
                100.0;
      }
    }
    outUsages.append(std::clamp(usage, 0.0, 100.0));
    m_prevCoreTicks[coreIdx] = {total, totalIdle};
    coreIdx++;
    line = strchr(ptr, '\n');
  }
}

void ProcessScanWorker::readSystemRam(WorkerScanResult &res) {
  int fd = open("/proc/meminfo", O_RDONLY | O_CLOEXEC);
  if (fd < 0)
    return;

  char buf[2048];
  ssize_t n = read(fd, buf, sizeof(buf) - 1);
  close(fd);
  if (n <= 0)
    return;
  buf[n] = '\0';

  qulonglong totalMem = 0;
  qulonglong availMem = 0;
  qulonglong swapTotal = 0;
  qulonglong swapFree = 0;

  const char *pos = strstr(buf, "MemTotal:");
  if (pos)
    totalMem = strtoull(pos + 9, nullptr, 10) * 1024ULL;

  pos = strstr(buf, "MemAvailable:");
  if (pos)
    availMem = strtoull(pos + 13, nullptr, 10) * 1024ULL;

  pos = strstr(buf, "SwapTotal:");
  if (pos)
    swapTotal = strtoull(pos + 10, nullptr, 10) * 1024ULL;

  pos = strstr(buf, "SwapFree:");
  if (pos)
    swapFree = strtoull(pos + 9, nullptr, 10) * 1024ULL;

  res.systemRamTotalBytes = totalMem;
  res.systemRamAvailBytes = availMem;
  res.systemSwapTotalBytes = swapTotal;
  res.systemSwapUsedBytes =
      (swapTotal >= swapFree) ? (swapTotal - swapFree) : 0;

  if (totalMem > 0 && totalMem >= availMem) {
    res.systemRamUsedBytes = totalMem - availMem;
    res.systemRamUsagePercent = (static_cast<double>(res.systemRamUsedBytes) /
                                 static_cast<double>(totalMem)) *
                                100.0;
  }
}

void ProcessScanWorker::readSystemDiskStats(WorkerScanResult &res) {
  int fd = open("/proc/diskstats", O_RDONLY | O_CLOEXEC);
  if (fd < 0)
    return;

  char buf[8192];
  ssize_t n = read(fd, buf, sizeof(buf) - 1);
  close(fd);

  quint64 totalReadSectors = 0;
  quint64 totalWriteSectors = 0;

  if (n > 0) {
    buf[n] = '\0';
    char *line = buf;
    while (line && *line) {
      char *nextLine = strchr(line, '\n');
      if (nextLine)
        *nextLine = '\0';

      char devName[64];
      quint64 rSec = 0;
      quint64 wSec = 0;
      int matched = sscanf(line, "%*d %*d %63s %*u %*u %llu %*u %*u %*u %llu",
                           devName, &rSec, &wSec);
      if (matched == 3) {
        if (strncmp(devName, "nvme", 4) == 0 && strchr(devName, 'n') &&
            !strchr(devName, 'p')) {
          totalReadSectors += rSec;
          totalWriteSectors += wSec;
        } else if ((strncmp(devName, "sd", 2) == 0 ||
                    strncmp(devName, "vd", 2) == 0) &&
                   strlen(devName) == 3) {
          totalReadSectors += rSec;
          totalWriteSectors += wSec;
        }
      }
      line = nextLine ? (nextLine + 1) : nullptr;
    }
  }

  quint64 totalReadBytes = totalReadSectors * 512ULL;
  quint64 totalWriteBytes = totalWriteSectors * 512ULL;

  qint64 elapsedMs = m_ioTimer.restart();
  if (elapsedMs <= 0)
    elapsedMs = 1000;
  double elapsedSec = static_cast<double>(elapsedMs) / 1000.0;

  if (m_prevDiskReadBytes > 0 && totalReadBytes >= m_prevDiskReadBytes) {
    res.systemDiskReadBytesSec = static_cast<qulonglong>(
        (totalReadBytes - m_prevDiskReadBytes) / elapsedSec);
  }
  if (m_prevDiskWriteBytes > 0 && totalWriteBytes >= m_prevDiskWriteBytes) {
    res.systemDiskWriteBytesSec = static_cast<qulonglong>(
        (totalWriteBytes - m_prevDiskWriteBytes) / elapsedSec);
  }

  m_prevDiskReadBytes = totalReadBytes;
  m_prevDiskWriteBytes = totalWriteBytes;
  res.systemDiskBytesSec =
      res.systemDiskReadBytesSec + res.systemDiskWriteBytesSec;

  m_mountCheckCounter++;
  if (m_cachedDisks.isEmpty() || m_mountCheckCounter >= 8) {
    m_mountCheckCounter = 0;
    FILE *mountsFile = fopen("/proc/mounts", "re");
    if (mountsFile) {
      DiskList discovered;
      char lineBuf[512];
      while (fgets(lineBuf, sizeof(lineBuf), mountsFile)) {
        char dev[128];
        char mnt[128];
        char fs[64];
        if (sscanf(lineBuf, "%127s %127s %63s", dev, mnt, fs) >= 3) {
          if (strncmp(dev, "/dev/", 5) != 0 ||
              strncmp(dev, "/dev/loop", 9) == 0)
            continue;

          struct statvfs stat;
          if (statvfs(mnt, &stat) == 0) {
            DiskDevice ddev;
            ddev.mountPoint = QString::fromUtf8(mnt);
            ddev.fsType = QString::fromUtf8(fs);
            ddev.totalBytes =
                static_cast<qulonglong>(stat.f_blocks) * stat.f_frsize;
            qulonglong freeBytes =
                static_cast<qulonglong>(stat.f_bavail) * stat.f_frsize;
            ddev.usedBytes = (ddev.totalBytes >= freeBytes)
                                 ? (ddev.totalBytes - freeBytes)
                                 : 0;
            ddev.readSpeed = res.systemDiskReadBytesSec;
            ddev.writeSpeed = res.systemDiskWriteBytesSec;
            discovered.append(ddev);
          }
        }
      }
      fclose(mountsFile);
      if (!discovered.isEmpty()) {
        m_cachedDisks = discovered;
      }
    }
  }

  for (DiskDevice &d : m_cachedDisks) {
    d.readSpeed = res.systemDiskReadBytesSec;
    d.writeSpeed = res.systemDiskWriteBytesSec;
  }
  res.disks = m_cachedDisks;
}

void ProcessScanWorker::readSystemNetStats(WorkerScanResult &res) {
  int fd = open("/proc/net/dev", O_RDONLY | O_CLOEXEC);
  if (fd < 0)
    return;

  char buf[4096];
  ssize_t n = read(fd, buf, sizeof(buf) - 1);
  close(fd);

  quint64 totalRx = 0;
  quint64 totalTx = 0;
  NetworkList discovered;

  if (n > 0) {
    buf[n] = '\0';
    char *line = strchr(buf, '\n');
    if (line)
      line = strchr(line + 1, '\n');

    while (line && *line) {
      line++;
      char *colon = strchr(line, ':');
      char *nextLine = strchr(line, '\n');
      if (nextLine)
        *nextLine = '\0';

      if (colon) {
        *colon = '\0';
        char iface[64];
        sscanf(line, "%63s", iface);
        if (strcmp(iface, "lo") != 0) {
          quint64 rx = 0;
          quint64 tx = 0;
          sscanf(colon + 1, "%llu %*u %*u %*u %*u %*u %*u %*u %llu", &rx, &tx);
          totalRx += rx;
          totalTx += tx;

          NetworkDevice ndev;
          ndev.interfaceName = QString::fromUtf8(iface);
          ndev.isWireless = (strncmp(iface, "wl", 2) == 0);
          discovered.append(ndev);
        }
      }
      line = nextLine;
    }
  }

  double elapsedSec = 1.0;
  if (m_prevNetRxBytes > 0 && totalRx >= m_prevNetRxBytes) {
    res.systemNetRxBytesSec =
        static_cast<qulonglong>((totalRx - m_prevNetRxBytes) / elapsedSec);
  }
  if (m_prevNetTxBytes > 0 && totalTx >= m_prevNetTxBytes) {
    res.systemNetTxBytesSec =
        static_cast<qulonglong>((totalTx - m_prevNetTxBytes) / elapsedSec);
  }

  m_prevNetRxBytes = totalRx;
  m_prevNetTxBytes = totalTx;
  res.systemNetBytesSec = res.systemNetRxBytesSec + res.systemNetTxBytesSec;

  if (!discovered.isEmpty()) {
    discovered[0].rxSpeed = res.systemNetRxBytesSec;
    discovered[0].txSpeed = res.systemNetTxBytesSec;
  }
  res.networks = discovered;
}

void ProcessScanWorker::readGpuStats(WorkerScanResult &res) {
  if (!m_gpusInitialized)
    initGpuDevices();

  res.gpus = m_cachedGpuMeta;

  for (GpuDevice &gpu : res.gpus) {
    QString devicePath = "/sys/class/drm/" + gpu.id + "/device";
    double usage = 0.0;
    qulonglong vramUsed = 0;

    int fdBusy = open((devicePath + "/gpu_busy_percent").toUtf8().constData(),
                      O_RDONLY | O_CLOEXEC);
    if (fdBusy >= 0) {
      char bBuf[32];
      ssize_t bn = read(fdBusy, bBuf, sizeof(bBuf) - 1);
      close(fdBusy);
      if (bn > 0) {
        bBuf[bn] = '\0';
        usage = strtod(bBuf, nullptr);
      }
      int fdVram =
          open((devicePath + "/mem_info_vram_used").toUtf8().constData(),
               O_RDONLY | O_CLOEXEC);
      if (fdVram >= 0) {
        char vBuf[48];
        ssize_t vn = read(fdVram, vBuf, sizeof(vBuf) - 1);
        close(fdVram);
        if (vn > 0) {
          vBuf[vn] = '\0';
          vramUsed = strtoull(vBuf, nullptr, 10);
        }
      }
    } else {
      int fdAct = open((devicePath + "/drm/" + gpu.id + "/gt_act_freq_mhz")
                           .toUtf8()
                           .constData(),
                       O_RDONLY | O_CLOEXEC);
      int fdMax = open((devicePath + "/drm/" + gpu.id + "/gt_max_freq_mhz")
                           .toUtf8()
                           .constData(),
                       O_RDONLY | O_CLOEXEC);
      if (fdAct < 0) {
        fdAct =
            open((devicePath + "/gt/gt0/rps_act_freq_mhz").toUtf8().constData(),
                 O_RDONLY | O_CLOEXEC);
        fdMax =
            open((devicePath + "/gt/gt0/rps_max_freq_mhz").toUtf8().constData(),
                 O_RDONLY | O_CLOEXEC);
      }
      if (fdAct >= 0 && fdMax >= 0) {
        char aBuf[32], mBuf[32];
        ssize_t an = read(fdAct, aBuf, sizeof(aBuf) - 1);
        ssize_t mn = read(fdMax, mBuf, sizeof(mBuf) - 1);
        close(fdAct);
        close(fdMax);
        if (an > 0 && mn > 0) {
          aBuf[an] = '\0';
          mBuf[mn] = '\0';
          double actFreq = strtod(aBuf, nullptr);
          double maxFreq = strtod(mBuf, nullptr);
          if (maxFreq > 0.0) {
            usage = (actFreq / maxFreq) * 100.0;
          }
        }
      }
      vramUsed =
          (res.systemRamUsedBytes > 0) ? (res.systemRamUsedBytes / 8) : 0;
    }

    gpu.usage = std::clamp(usage, 0.0, 100.0);
    gpu.vramUsed = vramUsed;
  }
}

void ProcessScanWorker::scanProcesses(QList<ProcessNode> &outList,
                                      quint64 totalSystemTicks,
                                      WorkerScanResult &res) {
  DIR *procDir = opendir("/proc");
  if (!procDir)
    return;

  quint64 deltaSystem = (totalSystemTicks > m_prevTotalSystemTicks)
                            ? (totalSystemTicks - m_prevTotalSystemTicks)
                            : 0;

  QHash<int, quint64> currentProcTicks;
  QHash<int, quint64> currentDiskBytes;
  QHash<int, double> currentSmoothedCpu;
  QSet<int> seenPids;

  struct dirent *entry = nullptr;
  while ((entry = readdir(procDir)) != nullptr) {
    if (entry->d_type != DT_DIR && entry->d_type != DT_UNKNOWN)
      continue;

    char *endptr = nullptr;
    long pid = strtol(entry->d_name, &endptr, 10);
    if (*endptr != '\0' || pid <= 2)
      continue;

    seenPids.insert(pid);

    char statPath[64];
    snprintf(statPath, sizeof(statPath), "/proc/%ld/stat", pid);
    int fd = open(statPath, O_RDONLY | O_CLOEXEC);
    if (fd < 0)
      continue;

    char statBuf[1024];
    ssize_t n = read(fd, statBuf, sizeof(statBuf) - 1);
    close(fd);
    if (n <= 0)
      continue;
    statBuf[n] = '\0';

    char *openParen = strchr(statBuf, '(');
    char *closeParen = strrchr(statBuf, ')');
    if (!openParen || !closeParen || closeParen < openParen ||
        closeParen + 2 >= statBuf + n)
      continue;

    char *rest = closeParen + 2;
    char state = ' ';
    int ppid = 0;
    quint64 utime = 0;
    quint64 stime = 0;
    qulonglong rssPages = 0;

    int scanned = sscanf(rest,
                         "%c %d %*d %*d %*d %*d %*u %*u %*u %*u %*u %llu %llu "
                         "%*d %*d %*d %*d %*d %*d %*u %*u %llu",
                         &state, &ppid, &utime, &stime, &rssPages);

    if (scanned < 5 || ppid == 2)
      continue;

    QString name;
    QString detail;

    auto it = m_staticCache.constFind(pid);
    if (it != m_staticCache.constEnd()) {
      name = it->name;
      detail = it->detailName;
    } else {
      *closeParen = '\0';
      QString commName = QString::fromUtf8(openParen + 1);
      name = extractProcessName(pid, commName);
      detail = extractDetailName(pid, commName);
      m_staticCache.insert(pid, {name, detail, ppid});
    }

    quint64 procTicks = utime + stime;
    currentProcTicks[pid] = procTicks;

    double rawCpu = 0.0;
    if (deltaSystem > 0 && m_prevProcTicks.contains(pid)) {
      quint64 prevTicks = m_prevProcTicks[pid];
      if (procTicks >= prevTicks) {
        quint64 deltaProc = procTicks - prevTicks;
        rawCpu = (static_cast<double>(deltaProc) /
                  static_cast<double>(deltaSystem)) *
                 100.0 * m_cpuOnlineCores;
      }
    }

    double smoothedCpu = rawCpu;
    if (m_prevSmoothedCpu.contains(pid)) {
      smoothedCpu = (0.25 * rawCpu) + (0.75 * m_prevSmoothedCpu[pid]);
    }
    currentSmoothedCpu[pid] = smoothedCpu;

    qulonglong ramBytes = rssPages * m_pageSize;
    qulonglong diskBytesSec = 0;
    qulonglong netBytesSec = 0;

    char ioPath[64];
    snprintf(ioPath, sizeof(ioPath), "/proc/%ld/io", pid);
    int fdIo = open(ioPath, O_RDONLY | O_CLOEXEC);
    if (fdIo >= 0) {
      char ioBuf[512];
      ssize_t ion = read(fdIo, ioBuf, sizeof(ioBuf) - 1);
      close(fdIo);
      if (ion > 0) {
        ioBuf[ion] = '\0';
        quint64 rbytes = 0;
        quint64 wbytes = 0;
        quint64 rchar = 0;
        quint64 wchar = 0;

        const char *rpos = strstr(ioBuf, "read_bytes:");
        if (rpos)
          rbytes = strtoull(rpos + 11, nullptr, 10);
        const char *wpos = strstr(ioBuf, "write_bytes:");
        if (wpos)
          wbytes = strtoull(wpos + 12, nullptr, 10);
        const char *rcpos = strstr(ioBuf, "rchar:");
        if (rcpos)
          rchar = strtoull(rcpos + 6, nullptr, 10);
        const char *wcpos = strstr(ioBuf, "wchar:");
        if (wcpos)
          wchar = strtoull(wcpos + 6, nullptr, 10);

        quint64 totalDisk = rbytes + wbytes;
        currentDiskBytes[pid] = totalDisk;

        if (m_prevDiskBytes.contains(pid) &&
            totalDisk >= m_prevDiskBytes[pid]) {
          diskBytesSec = totalDisk - m_prevDiskBytes[pid];
        }

        quint64 totalIO = rchar + wchar;
        if (totalIO > totalDisk) {
          netBytesSec = (totalIO - totalDisk) / 20;
        }
      }
    }

    outList.append({static_cast<int>(pid), ppid, name, detail, smoothedCpu,
                    ramBytes, diskBytesSec, netBytesSec});
  }
  closedir(procDir);

  for (auto it = m_staticCache.begin(); it != m_staticCache.end();) {
    if (!seenPids.contains(it.key())) {
      it = m_staticCache.erase(it);
    } else {
      ++it;
    }
  }

  m_prevProcTicks = currentProcTicks;
  m_prevDiskBytes = currentDiskBytes;
  m_prevSmoothedCpu = currentSmoothedCpu;
  m_prevTotalSystemTicks = totalSystemTicks;
}

void ProcessScanWorker::buildGroups(const QList<ProcessNode> &rawNodes,
                                    QList<ProcessGroup> &outGroups,
                                    int sortColumn, int sortOrder) {
  QHash<QString, ProcessGroup> groupMap;

  for (const ProcessNode &node : rawNodes) {
    const QString &groupKey = node.name;
    auto &grp = groupMap[groupKey];
    if (grp.groupName.isEmpty()) {
      grp.groupName = groupKey;
      grp.rootPid = node.pid;
    }
    grp.totalCpu += node.cpuPercent;
    grp.totalRam += node.ramBytes;
    grp.totalDisk += node.diskBytesSec;
    grp.totalNet += node.netBytesSec;
    grp.children.append(node);
  }

  outGroups = groupMap.values();
  bool desc = (sortOrder == Qt::DescendingOrder);

  std::sort(
      outGroups.begin(), outGroups.end(),
      [sortColumn, desc](const ProcessGroup &a, const ProcessGroup &b) {
        if (sortColumn == 0)
          return desc ? a.rootPid > b.rootPid : a.rootPid < b.rootPid;
        if (sortColumn == 1)
          return desc ? a.groupName > b.groupName : a.groupName < b.groupName;
        if (sortColumn == 2)
          return desc ? a.totalCpu > b.totalCpu : a.totalCpu < b.totalCpu;
        if (sortColumn == 3)
          return desc ? a.totalRam > b.totalRam : a.totalRam < b.totalRam;
        if (sortColumn == 4)
          return desc ? a.totalDisk > b.totalDisk : a.totalDisk < b.totalDisk;
        if (sortColumn == 5)
          return desc ? a.totalNet > b.totalNet : a.totalNet < b.totalNet;
        return desc ? a.totalRam > b.totalRam : a.totalRam < b.totalRam;
      });

  for (ProcessGroup &grp : outGroups) {
    std::sort(grp.children.begin(), grp.children.end(),
              [](const ProcessNode &a, const ProcessNode &b) {
                return a.ramBytes > b.ramBytes;
              });
  }
}

void ProcessScanWorker::performScan(int sortColumn, int sortOrder) {
  WorkerScanResult res;
  quint64 totalSystemTicks = 0;
  quint64 idleSystemTicks = 0;

  readSystemStats(totalSystemTicks, idleSystemTicks);

  if (totalSystemTicks > m_prevTotalSystemTicks) {
    quint64 deltaTotal = totalSystemTicks - m_prevTotalSystemTicks;
    quint64 deltaIdle = (idleSystemTicks >= m_prevIdleSystemTicks)
                            ? (idleSystemTicks - m_prevIdleSystemTicks)
                            : 0;
    if (deltaTotal >= deltaIdle) {
      res.systemCpuUsage = (static_cast<double>(deltaTotal - deltaIdle) /
                            static_cast<double>(deltaTotal)) *
                           100.0;
    }
  }
  m_prevIdleSystemTicks = idleSystemTicks;

  readSystemRam(res);
  readSystemDiskStats(res);
  readSystemNetStats(res);
  readGpuStats(res);
  readPerCoreStats(res.perCoreUsage);

  QList<ProcessNode> rawNodes;
  scanProcesses(rawNodes, totalSystemTicks, res);
  buildGroups(rawNodes, res.allGroups, sortColumn, sortOrder);

  emit scanCompleted(res);
}

ProcessTableModel::ProcessTableModel(QObject *parent)
    : QAbstractTableModel(parent) {
  qRegisterMetaType<WorkerScanResult>("WorkerScanResult");
  registerSystemStatsMetaTypes();

  m_cpuOnlineCores =
      std::max(1, static_cast<int>(sysconf(_SC_NPROCESSORS_ONLN)));
  m_cpuHistory = QList<double>(HistoryLength, 0.0);
  m_ramHistory = QList<double>(HistoryLength, 0.0);
  m_diskHistory = QList<double>(HistoryLength, 0.0);
  m_netHistory = QList<double>(HistoryLength, 0.0);

  for (int i = 0; i < m_cpuOnlineCores; ++i) {
    m_perCoreHistory.append(QList<double>(HistoryLength, 0.0));
  }

  m_worker = new ProcessScanWorker();
  m_worker->moveToThread(&m_workerThread);

  connect(&m_workerThread, &QThread::finished, m_worker, &QObject::deleteLater);
  connect(this, &ProcessTableModel::requestScan, m_worker,
          &ProcessScanWorker::performScan);
  connect(m_worker, &ProcessScanWorker::scanCompleted, this,
          &ProcessTableModel::onScanCompleted);

  m_workerThread.start();

  m_interface = new OrgCielSystemStatsInterface(
      "org.ciel.SystemStats", "/org/ciel/SystemStats",
      QDBusConnection::sessionBus(), this);

  if (m_interface->isValid()) {
    connect(m_interface, &OrgCielSystemStatsInterface::StatsUpdated, this,
            &ProcessTableModel::refresh);
  }

  refresh();
}

ProcessTableModel::~ProcessTableModel() {
  m_workerThread.quit();
  m_workerThread.wait();
}

int ProcessTableModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;
  return m_visibleGroups.size();
}

int ProcessTableModel::columnCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;
  return 6;
}

double ProcessTableModel::systemCpuUsage() const { return m_systemCpuUsage; }

double ProcessTableModel::systemRamUsagePercent() const {
  return m_systemRamUsagePercent;
}

qulonglong ProcessTableModel::systemRamUsedBytes() const {
  return m_systemRamUsedBytes;
}

qulonglong ProcessTableModel::systemRamTotalBytes() const {
  return m_systemRamTotalBytes;
}

qulonglong ProcessTableModel::systemRamAvailBytes() const {
  return m_systemRamAvailBytes;
}

qulonglong ProcessTableModel::systemSwapUsedBytes() const {
  return m_systemSwapUsedBytes;
}

qulonglong ProcessTableModel::systemSwapTotalBytes() const {
  return m_systemSwapTotalBytes;
}

QString ProcessTableModel::systemDiskSpeed() const {
  return formatSpeed(m_systemDiskBytesSec);
}

qulonglong ProcessTableModel::systemDiskBytesSec() const {
  return m_systemDiskBytesSec;
}

qulonglong ProcessTableModel::systemDiskReadBytesSec() const {
  return m_systemDiskReadBytesSec;
}

qulonglong ProcessTableModel::systemDiskWriteBytesSec() const {
  return m_systemDiskWriteBytesSec;
}

QString ProcessTableModel::systemNetSpeed() const {
  return formatSpeed(m_systemNetBytesSec);
}

qulonglong ProcessTableModel::systemNetBytesSec() const {
  return m_systemNetBytesSec;
}

qulonglong ProcessTableModel::systemNetRxBytesSec() const {
  return m_systemNetRxBytesSec;
}

qulonglong ProcessTableModel::systemNetTxBytesSec() const {
  return m_systemNetTxBytesSec;
}

QString ProcessTableModel::formatSpeed(qulonglong bytesSec) const {
  if (bytesSec >= 1024ULL * 1024ULL * 1024ULL) {
    return QString::number(static_cast<double>(bytesSec) /
                               (1024.0 * 1024.0 * 1024.0),
                           'f', 1) +
           " GB/s";
  }
  if (bytesSec >= 1024ULL * 1024ULL) {
    return QString::number(static_cast<double>(bytesSec) / (1024.0 * 1024.0),
                           'f', 1) +
           " MB/s";
  }
  if (bytesSec >= 1024ULL) {
    return QString::number(static_cast<double>(bytesSec) / 1024.0, 'f', 0) +
           " KB/s";
  }
  return QString::number(bytesSec) + " B/s";
}

QString ProcessTableModel::formatBytes(qulonglong bytes) const {
  if (bytes >= 1024ULL * 1024ULL * 1024ULL) {
    return QString::number(static_cast<double>(bytes) /
                               (1024.0 * 1024.0 * 1024.0),
                           'f', 1) +
           " GB";
  }
  if (bytes >= 1024ULL * 1024ULL) {
    return QString::number(static_cast<double>(bytes) / (1024.0 * 1024.0), 'f',
                           0) +
           " MB";
  }
  if (bytes >= 1024ULL) {
    return QString::number(static_cast<double>(bytes) / 1024.0, 'f', 0) + " KB";
  }
  return QString::number(bytes) + " B";
}

QVariant ProcessTableModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() >= m_visibleGroups.size()) {
    return QVariant();
  }

  const ProcessGroup &grp = m_visibleGroups.at(index.row());

  switch (role) {
  case Qt::DisplayRole:
    switch (index.column()) {
    case 0:
      return QString::number(grp.rootPid);
    case 1:
      return grp.children.size() > 1
                 ? (grp.groupName + " (" +
                    QString::number(grp.children.size()) + ")")
                 : grp.groupName;
    case 2:
      return QString::number(grp.totalCpu, 'f', 1) + "%";
    case 3:
      return QString::number(grp.totalRam / (1024 * 1024)) + " MB";
    case 4:
      return formatSpeed(grp.totalDisk);
    case 5:
      return formatSpeed(grp.totalNet);
    default:
      return QVariant();
    }
  case PidRole:
    return grp.rootPid;
  case NameRole:
    return grp.children.size() > 1
               ? (grp.groupName + " (" + QString::number(grp.children.size()) +
                  ")")
               : grp.groupName;
  case CpuRole:
    return QString::number(grp.totalCpu, 'f', 1) + "%";
  case RamRole:
    return QString::number(grp.totalRam / (1024 * 1024)) + " MB";
  case DiskRole:
    return formatSpeed(grp.totalDisk);
  case NetRole:
    return formatSpeed(grp.totalNet);
  case ChildCountRole:
    return grp.children.size();
  case ChildrenRole: {
    QVariantList list;
    for (const ProcessNode &child : grp.children) {
      QVariantMap map;
      map["pid"] = child.pid;
      map["name"] = child.detailName;
      map["cpu"] = QString::number(child.cpuPercent, 'f', 1) + "%";
      map["ram"] = QString::number(child.ramBytes / (1024 * 1024)) + " MB";
      map["disk"] = formatSpeed(child.diskBytesSec);
      map["net"] = formatSpeed(child.netBytesSec);
      list.append(map);
    }
    return list;
  }
  }

  return QVariant();
}

QHash<int, QByteArray> ProcessTableModel::roleNames() const {
  QHash<int, QByteArray> roles = QAbstractTableModel::roleNames();
  roles[Qt::DisplayRole] = "display";
  roles[PidRole] = "pid";
  roles[NameRole] = "procName";
  roles[CpuRole] = "cpu";
  roles[RamRole] = "ram";
  roles[DiskRole] = "disk";
  roles[NetRole] = "net";
  roles[ChildCountRole] = "childCount";
  roles[ChildrenRole] = "subProcesses";
  return roles;
}

QString ProcessTableModel::filterText() const { return m_filterText; }

bool ProcessTableModel::isSearching() const {
  return !m_filterText.trimmed().isEmpty();
}

void ProcessTableModel::setFilterText(const QString &text) {
  if (m_filterText == text)
    return;

  bool wasSearching = isSearching();
  m_filterText = text;

  beginResetModel();
  updateVisibleGroups();
  endResetModel();

  emit filterTextChanged();
  if (wasSearching != isSearching()) {
    emit isSearchingChanged();
  }
}

int ProcessTableModel::sortColumn() const { return m_sortColumn; }

void ProcessTableModel::setSortColumn(int column) {
  if (m_sortColumn == column)
    return;
  sortByColumn(column, m_sortOrder);
}

int ProcessTableModel::sortOrder() const { return m_sortOrder; }

void ProcessTableModel::setSortOrder(int order) {
  if (m_sortOrder == order)
    return;
  sortByColumn(m_sortColumn, order);
}

void ProcessTableModel::sortByColumn(int column, int order) {
  m_sortColumn = column;
  m_sortOrder = order;
  emit sortColumnChanged();
  emit sortOrderChanged();
  refresh();
}

void ProcessTableModel::refresh() {
  if (m_scanInProgress) {
    m_pendingScan = true;
    return;
  }
  m_scanInProgress = true;
  emit requestScan(m_sortColumn, m_sortOrder);
}

void ProcessTableModel::onScanCompleted(const WorkerScanResult &res) {
  m_scanInProgress = false;

  m_systemCpuUsage = res.systemCpuUsage;
  m_systemRamUsagePercent = res.systemRamUsagePercent;
  m_systemRamUsedBytes = res.systemRamUsedBytes;
  m_systemRamTotalBytes = res.systemRamTotalBytes;
  m_systemRamAvailBytes = res.systemRamAvailBytes;
  m_systemSwapUsedBytes = res.systemSwapUsedBytes;
  m_systemSwapTotalBytes = res.systemSwapTotalBytes;

  m_systemDiskBytesSec = res.systemDiskBytesSec;
  m_systemDiskReadBytesSec = res.systemDiskReadBytesSec;
  m_systemDiskWriteBytesSec = res.systemDiskWriteBytesSec;

  m_systemNetBytesSec = res.systemNetBytesSec;
  m_systemNetRxBytesSec = res.systemNetRxBytesSec;
  m_systemNetTxBytesSec = res.systemNetTxBytesSec;

  emit systemTotalsChanged();

  m_diskPeakRate =
      std::max(m_diskPeakRate * 0.98,
               std::max(1048576.0, static_cast<double>(m_systemDiskBytesSec)));
  m_netPeakRate =
      std::max(m_netPeakRate * 0.98,
               std::max(1048576.0, static_cast<double>(m_systemNetBytesSec)));

  appendHistory(m_cpuHistory, m_systemCpuUsage / 100.0);
  appendHistory(m_ramHistory, m_systemRamUsagePercent / 100.0);
  appendHistory(
      m_diskHistory,
      std::clamp(static_cast<double>(m_systemDiskBytesSec) / m_diskPeakRate,
                 0.0, 1.0));
  appendHistory(
      m_netHistory,
      std::clamp(static_cast<double>(m_systemNetBytesSec) / m_netPeakRate, 0.0,
                 1.0));

  m_perCoreUsage = res.perCoreUsage;
  for (int i = 0; i < m_perCoreUsage.size(); ++i) {
    if (i >= m_perCoreHistory.size()) {
      m_perCoreHistory.append(QList<double>(HistoryLength, 0.0));
    }
    appendHistory(m_perCoreHistory[i], m_perCoreUsage[i] / 100.0);
  }
  emit perCoreUsageChanged();

  bool gpusCountChanged = (m_gpus.size() != res.gpus.size());
  m_gpus = res.gpus;
  while (m_gpuHistories.size() < m_gpus.size()) {
    m_gpuHistories.append(QList<double>(HistoryLength, 0.0));
  }
  for (int i = 0; i < m_gpus.size(); ++i) {
    appendHistory(m_gpuHistories[i], m_gpus[i].usage / 100.0);
  }
  if (gpusCountChanged) {
    emit gpusChanged();
  }

  m_disks = res.disks;
  emit disksChanged();

  m_networks = res.networks;
  emit networksChanged();

  m_allGroups = res.allGroups;
  int oldSize = m_visibleGroups.size();
  updateVisibleGroups();

  if (oldSize != m_visibleGroups.size()) {
    beginResetModel();
    endResetModel();
  } else if (!m_visibleGroups.isEmpty()) {
    emit dataChanged(index(0, 0), index(m_visibleGroups.size() - 1, 5));
  }

  m_historyRevision++;
  emit historyUpdated();

  if (m_pendingScan) {
    m_pendingScan = false;
    refresh();
  }
}

void ProcessTableModel::updateVisibleGroups() {
  QString query = m_filterText.trimmed();
  if (query.isEmpty()) {
    m_visibleGroups = m_allGroups;
  } else {
    m_visibleGroups.clear();
    for (const ProcessGroup &grp : m_allGroups) {
      if (grp.groupName.contains(query, Qt::CaseInsensitive)) {
        m_visibleGroups.append(grp);
        continue;
      }
      for (const ProcessNode &child : grp.children) {
        if (child.detailName.contains(query, Qt::CaseInsensitive) ||
            QString::number(child.pid).contains(query)) {
          m_visibleGroups.append(grp);
          break;
        }
      }
    }
  }
}

void ProcessTableModel::appendHistory(QList<double> &buffer, double val) {
  buffer.append(val);
  if (buffer.size() > HistoryLength)
    buffer.removeFirst();
}

QList<double> ProcessTableModel::cpuHistory() const { return m_cpuHistory; }

QList<double> ProcessTableModel::ramHistory() const { return m_ramHistory; }

QList<double> ProcessTableModel::diskHistory() const { return m_diskHistory; }

QList<double> ProcessTableModel::netHistory() const { return m_netHistory; }

QList<double> ProcessTableModel::perCoreUsage() const { return m_perCoreUsage; }

int ProcessTableModel::coreCount() const { return m_cpuOnlineCores; }

QList<double> ProcessTableModel::coreHistory(int coreIndex) const {
  if (coreIndex >= 0 && coreIndex < m_perCoreHistory.size())
    return m_perCoreHistory[coreIndex];
  return {};
}

int ProcessTableModel::gpuCount() const { return m_gpus.size(); }

QString ProcessTableModel::gpuName(int index) const {
  if (index >= 0 && index < m_gpus.size())
    return m_gpus[index].name;
  return QString();
}

QString ProcessTableModel::gpuId(int index) const {
  if (index >= 0 && index < m_gpus.size())
    return m_gpus[index].id;
  return QString();
}

double ProcessTableModel::gpuUsage(int index) const {
  if (index >= 0 && index < m_gpus.size())
    return m_gpus[index].usage;
  return 0.0;
}

double ProcessTableModel::gpuTemp(int index) const {
  if (index >= 0 && index < m_gpus.size())
    return m_gpus[index].temp;
  return 0.0;
}

qulonglong ProcessTableModel::gpuVramUsed(int index) const {
  if (index >= 0 && index < m_gpus.size())
    return m_gpus[index].vramUsed;
  return 0;
}

qulonglong ProcessTableModel::gpuVramTotal(int index) const {
  if (index >= 0 && index < m_gpus.size())
    return m_gpus[index].vramTotal;
  return 0;
}

QList<double> ProcessTableModel::gpuHistory(int index) const {
  if (index >= 0 && index < m_gpuHistories.size())
    return m_gpuHistories[index];
  return {};
}

int ProcessTableModel::diskCount() const { return m_disks.size(); }

QString ProcessTableModel::diskMount(int index) const {
  if (index >= 0 && index < m_disks.size())
    return m_disks[index].mountPoint;
  return QString();
}

QString ProcessTableModel::diskFsType(int index) const {
  if (index >= 0 && index < m_disks.size())
    return m_disks[index].fsType;
  return QString();
}

qulonglong ProcessTableModel::diskTotalBytes(int index) const {
  if (index >= 0 && index < m_disks.size())
    return m_disks[index].totalBytes;
  return 0;
}

qulonglong ProcessTableModel::diskUsedBytes(int index) const {
  if (index >= 0 && index < m_disks.size())
    return m_disks[index].usedBytes;
  return 0;
}

qulonglong ProcessTableModel::diskReadSpeed(int index) const {
  if (index >= 0 && index < m_disks.size())
    return m_disks[index].readSpeed;
  return 0;
}

qulonglong ProcessTableModel::diskWriteSpeed(int index) const {
  if (index >= 0 && index < m_disks.size())
    return m_disks[index].writeSpeed;
  return 0;
}

int ProcessTableModel::networkCount() const { return m_networks.size(); }

QString ProcessTableModel::networkName(int index) const {
  if (index >= 0 && index < m_networks.size())
    return m_networks[index].interfaceName;
  return QString();
}

bool ProcessTableModel::networkIsWireless(int index) const {
  if (index >= 0 && index < m_networks.size())
    return m_networks[index].isWireless;
  return false;
}

qulonglong ProcessTableModel::networkRxSpeed(int index) const {
  if (index >= 0 && index < m_networks.size())
    return m_networks[index].rxSpeed;
  return 0;
}

qulonglong ProcessTableModel::networkTxSpeed(int index) const {
  if (index >= 0 && index < m_networks.size())
    return m_networks[index].txSpeed;
  return 0;
}

int ProcessTableModel::historyRevision() const { return m_historyRevision; }
