/****************************************************************************
** Meta object code from reading C++ file 'SystemStatsInterface.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "SystemStatsInterface.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SystemStatsInterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN27OrgCielSystemStatsInterfaceE_t {};
} // unnamed namespace

template <> constexpr inline auto OrgCielSystemStatsInterface::qt_create_metaobjectdata<qt_meta_tag_ZN27OrgCielSystemStatsInterfaceE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "OrgCielSystemStatsInterface",
        "StatsUpdated",
        "",
        "CpuCores",
        "CpuFrequencyMHz",
        "CpuModel",
        "CpuTemp",
        "CpuUsage",
        "Disks",
        "DiskList",
        "Gpus",
        "GpuList",
        "Networks",
        "NetworkList",
        "PerCoreUsage",
        "DoubleList",
        "RamTotalBytes",
        "RamUsagePercent",
        "RamUsedBytes",
        "SwapTotalBytes",
        "SwapUsedBytes",
        "UptimeSeconds"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'StatsUpdated'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'CpuCores'
        QtMocHelpers::PropertyData<uint>(3, QMetaType::UInt, QMC::DefaultPropertyFlags),
        // property 'CpuFrequencyMHz'
        QtMocHelpers::PropertyData<uint>(4, QMetaType::UInt, QMC::DefaultPropertyFlags),
        // property 'CpuModel'
        QtMocHelpers::PropertyData<QString>(5, QMetaType::QString, QMC::DefaultPropertyFlags),
        // property 'CpuTemp'
        QtMocHelpers::PropertyData<double>(6, QMetaType::Double, QMC::DefaultPropertyFlags),
        // property 'CpuUsage'
        QtMocHelpers::PropertyData<double>(7, QMetaType::Double, QMC::DefaultPropertyFlags),
        // property 'Disks'
        QtMocHelpers::PropertyData<DiskList>(8, 0x80000000 | 9, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'Gpus'
        QtMocHelpers::PropertyData<GpuList>(10, 0x80000000 | 11, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'Networks'
        QtMocHelpers::PropertyData<NetworkList>(12, 0x80000000 | 13, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'PerCoreUsage'
        QtMocHelpers::PropertyData<DoubleList>(14, 0x80000000 | 15, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'RamTotalBytes'
        QtMocHelpers::PropertyData<qulonglong>(16, QMetaType::ULongLong, QMC::DefaultPropertyFlags),
        // property 'RamUsagePercent'
        QtMocHelpers::PropertyData<double>(17, QMetaType::Double, QMC::DefaultPropertyFlags),
        // property 'RamUsedBytes'
        QtMocHelpers::PropertyData<qulonglong>(18, QMetaType::ULongLong, QMC::DefaultPropertyFlags),
        // property 'SwapTotalBytes'
        QtMocHelpers::PropertyData<qulonglong>(19, QMetaType::ULongLong, QMC::DefaultPropertyFlags),
        // property 'SwapUsedBytes'
        QtMocHelpers::PropertyData<qulonglong>(20, QMetaType::ULongLong, QMC::DefaultPropertyFlags),
        // property 'UptimeSeconds'
        QtMocHelpers::PropertyData<qulonglong>(21, QMetaType::ULongLong, QMC::DefaultPropertyFlags),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<OrgCielSystemStatsInterface, qt_meta_tag_ZN27OrgCielSystemStatsInterfaceE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject OrgCielSystemStatsInterface::staticMetaObject = { {
    QMetaObject::SuperData::link<QDBusAbstractInterface::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN27OrgCielSystemStatsInterfaceE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN27OrgCielSystemStatsInterfaceE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN27OrgCielSystemStatsInterfaceE_t>.metaTypes,
    nullptr
} };

void OrgCielSystemStatsInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<OrgCielSystemStatsInterface *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->StatsUpdated(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (OrgCielSystemStatsInterface::*)()>(_a, &OrgCielSystemStatsInterface::StatsUpdated, 0))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<uint*>(_v) = _t->cpuCores(); break;
        case 1: *reinterpret_cast<uint*>(_v) = _t->cpuFrequencyMHz(); break;
        case 2: *reinterpret_cast<QString*>(_v) = _t->cpuModel(); break;
        case 3: *reinterpret_cast<double*>(_v) = _t->cpuTemp(); break;
        case 4: *reinterpret_cast<double*>(_v) = _t->cpuUsage(); break;
        case 5: *reinterpret_cast<DiskList*>(_v) = _t->disks(); break;
        case 6: *reinterpret_cast<GpuList*>(_v) = _t->gpus(); break;
        case 7: *reinterpret_cast<NetworkList*>(_v) = _t->networks(); break;
        case 8: *reinterpret_cast<DoubleList*>(_v) = _t->perCoreUsage(); break;
        case 9: *reinterpret_cast<qulonglong*>(_v) = _t->ramTotalBytes(); break;
        case 10: *reinterpret_cast<double*>(_v) = _t->ramUsagePercent(); break;
        case 11: *reinterpret_cast<qulonglong*>(_v) = _t->ramUsedBytes(); break;
        case 12: *reinterpret_cast<qulonglong*>(_v) = _t->swapTotalBytes(); break;
        case 13: *reinterpret_cast<qulonglong*>(_v) = _t->swapUsedBytes(); break;
        case 14: *reinterpret_cast<qulonglong*>(_v) = _t->uptimeSeconds(); break;
        default: break;
        }
    }
}

const QMetaObject *OrgCielSystemStatsInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OrgCielSystemStatsInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN27OrgCielSystemStatsInterfaceE_t>.strings))
        return static_cast<void*>(this);
    return QDBusAbstractInterface::qt_metacast(_clname);
}

int OrgCielSystemStatsInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void OrgCielSystemStatsInterface::StatsUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
