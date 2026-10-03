#pragma once

#include <QDBusArgument>
#include <QDBusMetaType>
#include <QList>
#include <QString>

// Alias for array of doubles (CPU per-core usage)
using DoubleList = QList<double>;
Q_DECLARE_METATYPE(DoubleList)

struct GpuDevice {
  QString id;
  QString name;
  double usage = 0.0;
  double temp = 0.0;
  qulonglong vramUsed = 0;
  qulonglong vramTotal = 0;
};
Q_DECLARE_METATYPE(GpuDevice)
using GpuList = QList<GpuDevice>;
Q_DECLARE_METATYPE(GpuList)

struct DiskDevice {
  QString mountPoint;
  QString fsType;
  qulonglong totalBytes = 0;
  qulonglong usedBytes = 0;
  qulonglong readSpeed = 0;
  qulonglong writeSpeed = 0;
};
Q_DECLARE_METATYPE(DiskDevice)
using DiskList = QList<DiskDevice>;
Q_DECLARE_METATYPE(DiskList)

struct NetworkDevice {
  QString interfaceName;
  bool isWireless = false;
  qulonglong rxSpeed = 0;
  qulonglong txSpeed = 0;
};
Q_DECLARE_METATYPE(NetworkDevice)
using NetworkList = QList<NetworkDevice>;
Q_DECLARE_METATYPE(NetworkList)

// D-Bus Serializers for GpuDevice
inline QDBusArgument &operator<<(QDBusArgument &argument,
                                 const GpuDevice &dev) {
  argument.beginStructure();
  argument << dev.id << dev.name << dev.usage << dev.temp << dev.vramUsed
           << dev.vramTotal;
  argument.endStructure();
  return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument,
                                       GpuDevice &dev) {
  argument.beginStructure();
  argument >> dev.id >> dev.name >> dev.usage >> dev.temp >> dev.vramUsed >>
      dev.vramTotal;
  argument.endStructure();
  return argument;
}

// D-Bus Serializers for DiskDevice
inline QDBusArgument &operator<<(QDBusArgument &argument,
                                 const DiskDevice &dev) {
  argument.beginStructure();
  argument << dev.mountPoint << dev.fsType << dev.totalBytes << dev.usedBytes
           << dev.readSpeed << dev.writeSpeed;
  argument.endStructure();
  return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument,
                                       DiskDevice &dev) {
  argument.beginStructure();
  argument >> dev.mountPoint >> dev.fsType >> dev.totalBytes >> dev.usedBytes >>
      dev.readSpeed >> dev.writeSpeed;
  argument.endStructure();
  return argument;
}

// D-Bus Serializers for NetworkDevice
inline QDBusArgument &operator<<(QDBusArgument &argument,
                                 const NetworkDevice &dev) {
  argument.beginStructure();
  argument << dev.interfaceName << dev.isWireless << dev.rxSpeed << dev.txSpeed;
  argument.endStructure();
  return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument,
                                       NetworkDevice &dev) {
  argument.beginStructure();
  argument >> dev.interfaceName >> dev.isWireless >> dev.rxSpeed >> dev.txSpeed;
  argument.endStructure();
  return argument;
}

inline void registerSystemStatsMetaTypes() {
  qDBusRegisterMetaType<DoubleList>();
  qDBusRegisterMetaType<GpuDevice>();
  qDBusRegisterMetaType<GpuList>();
  qDBusRegisterMetaType<DiskDevice>();
  qDBusRegisterMetaType<DiskList>();
  qDBusRegisterMetaType<NetworkDevice>();
  qDBusRegisterMetaType<NetworkList>();
}
