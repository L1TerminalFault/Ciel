/****************************************************************************
** Meta object code from reading C++ file 'SystemStatsService.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/SystemStatsService.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SystemStatsService.hpp' doesn't include <QObject>."
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
struct qt_meta_tag_ZN18SystemStatsServiceE_t {};
} // unnamed namespace

template <> constexpr inline auto SystemStatsService::qt_create_metaobjectdata<qt_meta_tag_ZN18SystemStatsServiceE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "SystemStatsService",
        "D-Bus Interface",
        "org.ciel.SystemStats",
        "StatsUpdated",
        "",
        "collect",
        "PerCoreUsage",
        "DoubleList",
        "CpuModel",
        "CpuCores",
        "CpuUsage",
        "CpuTemp",
        "CpuFrequencyMHz",
        "RamTotalBytes",
        "RamUsedBytes",
        "RamUsagePercent",
        "SwapTotalBytes",
        "SwapUsedBytes",
        "Gpus",
        "GpuList",
        "Disks",
        "DiskList",
        "Networks",
        "NetworkList",
        "UptimeSeconds"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'StatsUpdated'
        QtMocHelpers::SignalData<void()>(3, 4, QMC::AccessPublic, QMetaType::Void),
        // Slot 'collect'
        QtMocHelpers::SlotData<void()>(5, 4, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'PerCoreUsage'
        QtMocHelpers::PropertyData<DoubleList>(6, 0x80000000 | 7, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'CpuModel'
        QtMocHelpers::PropertyData<QString>(8, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'CpuCores'
        QtMocHelpers::PropertyData<uint>(9, QMetaType::UInt, QMC::DefaultPropertyFlags, 0),
        // property 'CpuUsage'
        QtMocHelpers::PropertyData<double>(10, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'CpuTemp'
        QtMocHelpers::PropertyData<double>(11, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'CpuFrequencyMHz'
        QtMocHelpers::PropertyData<uint>(12, QMetaType::UInt, QMC::DefaultPropertyFlags, 0),
        // property 'RamTotalBytes'
        QtMocHelpers::PropertyData<qulonglong>(13, QMetaType::ULongLong, QMC::DefaultPropertyFlags, 0),
        // property 'RamUsedBytes'
        QtMocHelpers::PropertyData<qulonglong>(14, QMetaType::ULongLong, QMC::DefaultPropertyFlags, 0),
        // property 'RamUsagePercent'
        QtMocHelpers::PropertyData<double>(15, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'SwapTotalBytes'
        QtMocHelpers::PropertyData<qulonglong>(16, QMetaType::ULongLong, QMC::DefaultPropertyFlags, 0),
        // property 'SwapUsedBytes'
        QtMocHelpers::PropertyData<qulonglong>(17, QMetaType::ULongLong, QMC::DefaultPropertyFlags, 0),
        // property 'Gpus'
        QtMocHelpers::PropertyData<GpuList>(18, 0x80000000 | 19, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'Disks'
        QtMocHelpers::PropertyData<DiskList>(20, 0x80000000 | 21, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'Networks'
        QtMocHelpers::PropertyData<NetworkList>(22, 0x80000000 | 23, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'UptimeSeconds'
        QtMocHelpers::PropertyData<qulonglong>(24, QMetaType::ULongLong, QMC::DefaultPropertyFlags, 0),
    };
    QtMocHelpers::UintData qt_enums {
    };
    QtMocHelpers::UintData qt_constructors {};
    QtMocHelpers::ClassInfos qt_classinfo({
            {    1,    2 },
    });
    return QtMocHelpers::metaObjectData<SystemStatsService, qt_meta_tag_ZN18SystemStatsServiceE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject SystemStatsService::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18SystemStatsServiceE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18SystemStatsServiceE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN18SystemStatsServiceE_t>.metaTypes,
    nullptr
} };

void SystemStatsService::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SystemStatsService *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->StatsUpdated(); break;
        case 1: _t->collect(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (SystemStatsService::*)()>(_a, &SystemStatsService::StatsUpdated, 0))
            return;
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 12:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< DiskList >(); break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< DoubleList >(); break;
        case 11:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< GpuList >(); break;
        case 13:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< NetworkList >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<DoubleList*>(_v) = _t->PerCoreUsage(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->CpuModel(); break;
        case 2: *reinterpret_cast<uint*>(_v) = _t->CpuCores(); break;
        case 3: *reinterpret_cast<double*>(_v) = _t->CpuUsage(); break;
        case 4: *reinterpret_cast<double*>(_v) = _t->CpuTemp(); break;
        case 5: *reinterpret_cast<uint*>(_v) = _t->CpuFrequencyMHz(); break;
        case 6: *reinterpret_cast<qulonglong*>(_v) = _t->RamTotalBytes(); break;
        case 7: *reinterpret_cast<qulonglong*>(_v) = _t->RamUsedBytes(); break;
        case 8: *reinterpret_cast<double*>(_v) = _t->RamUsagePercent(); break;
        case 9: *reinterpret_cast<qulonglong*>(_v) = _t->SwapTotalBytes(); break;
        case 10: *reinterpret_cast<qulonglong*>(_v) = _t->SwapUsedBytes(); break;
        case 11: *reinterpret_cast<GpuList*>(_v) = _t->Gpus(); break;
        case 12: *reinterpret_cast<DiskList*>(_v) = _t->Disks(); break;
        case 13: *reinterpret_cast<NetworkList*>(_v) = _t->Networks(); break;
        case 14: *reinterpret_cast<qulonglong*>(_v) = _t->UptimeSeconds(); break;
        default: break;
        }
    }
}

const QMetaObject *SystemStatsService::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SystemStatsService::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18SystemStatsServiceE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SystemStatsService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
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
void SystemStatsService::StatsUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
