#include "BootPerformanceModel.hpp"

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusMessage>
#include <QDBusReply>
#include <algorithm>

BootScanWorker::BootScanWorker(QObject *parent) : QObject(parent) {}

void BootScanWorker::performScan() {
  BootScanPayload payload;

  QDBusInterface manager(
      "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
      "org.freedesktop.systemd1.Manager", QDBusConnection::systemBus());

  if (manager.isValid()) {
    quint64 firmwareUsec =
        manager.property("FirmwareTimestampMonotonic").toULongLong();
    quint64 loaderUsec =
        manager.property("LoaderTimestampMonotonic").toULongLong();
    quint64 kernelUsec =
        manager.property("KernelTimestampMonotonic").toULongLong();
    quint64 initrdUsec =
        manager.property("InitRDTimestampMonotonic").toULongLong();
    quint64 userspaceUsec =
        manager.property("UserspaceTimestampMonotonic").toULongLong();
    quint64 finishUsec =
        manager.property("FinishTimestampMonotonic").toULongLong();

    if (loaderUsec > 0) {
      payload.firmwareSec = static_cast<double>(loaderUsec) / 1000000.0;
    }
    if (kernelUsec > loaderUsec) {
      payload.loaderSec =
          static_cast<double>(kernelUsec - loaderUsec) / 1000000.0;
    }
    if (initrdUsec > 0) {
      payload.kernelSec = static_cast<double>(initrdUsec) / 1000000.0;
      if (userspaceUsec > initrdUsec) {
        payload.initrdSec =
            static_cast<double>(userspaceUsec - initrdUsec) / 1000000.0;
      }
    } else {
      payload.kernelSec = static_cast<double>(userspaceUsec) / 1000000.0;
    }
    if (finishUsec > userspaceUsec) {
      payload.userspaceSec =
          static_cast<double>(finishUsec - userspaceUsec) / 1000000.0;
    }

    payload.totalSec = payload.firmwareSec + payload.loaderSec +
                       payload.kernelSec + payload.initrdSec +
                       payload.userspaceSec;

    QDBusMessage reply = manager.call("ListUnits");
    if (reply.type() == QDBusMessage::ReplyMessage &&
        !reply.arguments().isEmpty()) {
      const QDBusArgument &arg = reply.arguments().at(0).value<QDBusArgument>();
      arg.beginArray();

      quint64 maxDuration = 1;

      while (!arg.atEnd()) {
        QString id;
        QString desc;
        QString load;
        QString active;
        QString sub;
        QString following;
        QDBusObjectPath path;
        quint32 jobId;
        QString jobType;
        QDBusObjectPath jobPath;

        arg.beginStructure();
        arg >> id >> desc >> load >> active >> sub >> following >> path >>
            jobId >> jobType >> jobPath;
        arg.endStructure();

        if (id.endsWith(".service") || id.endsWith(".mount")) {
          QDBusInterface unit("org.freedesktop.systemd1", path.path(),
                              "org.freedesktop.systemd1.Unit",
                              QDBusConnection::systemBus());

          if (unit.isValid()) {
            quint64 enterUsec =
                unit.property("ActiveEnterTimestampMonotonic").toULongLong();
            quint64 exitUsec =
                unit.property("InactiveExitTimestampMonotonic").toULongLong();

            if (enterUsec > exitUsec && exitUsec > 0) {
              quint64 durationMs = (enterUsec - exitUsec) / 1000;
              if (durationMs >= 10) {
                BootServiceEntry item;
                item.unitName = id;
                item.durationMs = durationMs;

                if (durationMs > maxDuration) {
                  maxDuration = durationMs;
                }
                payload.services.append(item);
              }
            }
          }
        }
      }
      arg.endArray();

      std::sort(payload.services.begin(), payload.services.end(),
                [](const BootServiceEntry &a, const BootServiceEntry &b) {
                  return a.durationMs > b.durationMs;
                });

      for (BootServiceEntry &svc : payload.services) {
        svc.relativeRatio = static_cast<double>(svc.durationMs) /
                            static_cast<double>(maxDuration);
      }
    }
  }

  emit scanCompleted(payload);
}

BootPerformanceModel::BootPerformanceModel(QObject *parent)
    : QAbstractListModel(parent) {
  qRegisterMetaType<BootScanPayload>("BootScanPayload");

  m_worker = new BootScanWorker();
  m_worker->moveToThread(&m_workerThread);

  connect(&m_workerThread, &QThread::finished, m_worker, &QObject::deleteLater);
  connect(this, &BootPerformanceModel::requestScan, m_worker,
          &BootScanWorker::performScan);
  connect(m_worker, &BootScanWorker::scanCompleted, this,
          &BootPerformanceModel::onScanCompleted);

  m_workerThread.start();
  refresh();
}

BootPerformanceModel::~BootPerformanceModel() {
  m_workerThread.quit();
  m_workerThread.wait();
}

int BootPerformanceModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;
  return m_services.size();
}

QVariant BootPerformanceModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() < 0 || index.row() >= m_services.size())
    return QVariant();

  const BootServiceEntry &item = m_services.at(index.row());

  switch (role) {
  case UnitNameRole:
    return item.unitName;
  case DurationMsRole:
    return item.durationMs;
  case DurationStringRole:
    return formatDuration(item.durationMs);
  case RelativeRatioRole:
    return item.relativeRatio;
  default:
    return QVariant();
  }
}

QHash<int, QByteArray> BootPerformanceModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[UnitNameRole] = "unitName";
  roles[DurationMsRole] = "durationMs";
  roles[DurationStringRole] = "durationString";
  roles[RelativeRatioRole] = "relativeRatio";
  return roles;
}

double BootPerformanceModel::firmwareSec() const { return m_firmwareSec; }
double BootPerformanceModel::loaderSec() const { return m_loaderSec; }
double BootPerformanceModel::kernelSec() const { return m_kernelSec; }
double BootPerformanceModel::initrdSec() const { return m_initrdSec; }
double BootPerformanceModel::userspaceSec() const { return m_userspaceSec; }
double BootPerformanceModel::totalSec() const { return m_totalSec; }
bool BootPerformanceModel::isLoading() const { return m_isLoading; }

QString BootPerformanceModel::totalTimeString() const {
  return QString::number(m_totalSec, 'f', 2) + "s";
}

QVariantList BootPerformanceModel::segments() const {
  QVariantList list;

  if (m_firmwareSec > 0.001) {
    QVariantMap seg;
    seg["label"] = "Firmware";
    seg["value"] = m_firmwareSec;
    seg["color"] = "#AB47BC";
    list.append(seg);
  }
  if (m_loaderSec > 0.001) {
    QVariantMap seg;
    seg["label"] = "Loader";
    seg["value"] = m_loaderSec;
    seg["color"] = "#5C6BC0";
    list.append(seg);
  }
  if (m_kernelSec > 0.001) {
    QVariantMap seg;
    seg["label"] = "Kernel";
    seg["value"] = m_kernelSec;
    seg["color"] = "#26A69A";
    list.append(seg);
  }
  if (m_initrdSec > 0.001) {
    QVariantMap seg;
    seg["label"] = "Initrd";
    seg["value"] = m_initrdSec;
    seg["color"] = "#42A5F5";
    list.append(seg);
  }
  if (m_userspaceSec > 0.001) {
    QVariantMap seg;
    seg["label"] = "Userspace";
    seg["value"] = m_userspaceSec;
    seg["color"] = "#FFA726";
    list.append(seg);
  }

  return list;
}

void BootPerformanceModel::refresh() {
  if (m_isLoading)
    return;

  m_isLoading = true;
  emit loadingChanged();
  emit requestScan();
}

void BootPerformanceModel::onScanCompleted(const BootScanPayload &payload) {
  beginResetModel();
  m_firmwareSec = payload.firmwareSec;
  m_loaderSec = payload.loaderSec;
  m_kernelSec = payload.kernelSec;
  m_initrdSec = payload.initrdSec;
  m_userspaceSec = payload.userspaceSec;
  m_totalSec = payload.totalSec;
  m_services = payload.services;
  endResetModel();

  m_isLoading = false;
  emit loadingChanged();
  emit bootTimesChanged();
}

QString BootPerformanceModel::formatDuration(quint64 ms) {
  if (ms >= 1000) {
    return QString::number(static_cast<double>(ms) / 1000.0, 'f', 2) + "s";
  }
  return QString::number(ms) + "ms";
}
