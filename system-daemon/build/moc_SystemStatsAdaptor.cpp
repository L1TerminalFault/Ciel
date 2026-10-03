/****************************************************************************
** Meta object code from reading C++ file 'SystemStatsAdaptor.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "SystemStatsAdaptor.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SystemStatsAdaptor.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN18SystemStatsAdaptorE_t {};
} // unnamed namespace

template <> constexpr inline auto SystemStatsAdaptor::qt_create_metaobjectdata<qt_meta_tag_ZN18SystemStatsAdaptorE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "SystemStatsAdaptor",
        "D-Bus Interface",
        "org.ciel.SystemStats",
        "D-Bus Introspection",
        "  <interface name=\"org.ciel.SystemStats\">\n    <property access="
        "\"read\" type=\"s\" name=\"CpuModel\"/>\n    <property access=\"re"
        "ad\" type=\"u\" name=\"CpuCores\"/>\n    <property access=\"read\""
        " type=\"d\" name=\"CpuUsage\"/>\n    <property access=\"read\" typ"
        "e=\"d\" name=\"CpuTemp\"/>\n    <property access=\"read\" type=\"u"
        "\" name=\"CpuFrequencyMHz\"/>\n    <property access=\"read\" type="
        "\"ad\" name=\"PerCoreUsage\">\n      <annotation value=\"DoubleLis"
        "t\" name=\"org.qtproject.QtDBus.QtTypeName\"/>\n    </property>\n "
        "   <property access=\"read\" type=\"t\" name=\"RamTotalBytes\"/>\n"
        "    <property access=\"read\" type=\"t\" name=\"RamUsedBytes\"/>\n"
        "    <property access=\"read\" type=\"d\" name=\"RamUsagePercent\"/"
        ">\n    <property access=\"read\" type=\"t\" name=\"SwapTotalBytes\""
        "/>\n    <property access=\"read\" type=\"t\" name=\"SwapUsedBytes\""
        "/>\n    <property access=\"read\" type=\"a(ssddtt)\" name=\"Gpus\""
        ">\n      <annotation value=\"GpuList\" name=\"org.qtproject.QtDBus"
        ".QtTypeName\"/>\n    </property>\n    <property access=\"read\" ty"
        "pe=\"a(sstttt)\" name=\"Disks\">\n      <annotation value=\"DiskLi"
        "st\" name=\"org.qtproject.QtDBus.QtTypeName\"/>\n    </property>\n"
        "    <property access=\"read\" type=\"a(sbtt)\" name=\"Networks\">\n"
        "      <annotation value=\"NetworkList\" name=\"org.qtproject.QtDBu"
        "s.QtTypeName\"/>\n    </property>\n    <property access=\"read\" t"
        "ype=\"t\" name=\"UptimeSeconds\"/>\n    <signal name=\"StatsUpdate"
        "d\"/>\n  </interface>\n",
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
        QtMocHelpers::SignalData<void()>(5, 6, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'CpuCores'
        QtMocHelpers::PropertyData<uint>(7, QMetaType::UInt, QMC::DefaultPropertyFlags),
        // property 'CpuFrequencyMHz'
        QtMocHelpers::PropertyData<uint>(8, QMetaType::UInt, QMC::DefaultPropertyFlags),
        // property 'CpuModel'
        QtMocHelpers::PropertyData<QString>(9, QMetaType::QString, QMC::DefaultPropertyFlags),
        // property 'CpuTemp'
        QtMocHelpers::PropertyData<double>(10, QMetaType::Double, QMC::DefaultPropertyFlags),
        // property 'CpuUsage'
        QtMocHelpers::PropertyData<double>(11, QMetaType::Double, QMC::DefaultPropertyFlags),
        // property 'Disks'
        QtMocHelpers::PropertyData<DiskList>(12, 0x80000000 | 13, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'Gpus'
        QtMocHelpers::PropertyData<GpuList>(14, 0x80000000 | 15, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'Networks'
        QtMocHelpers::PropertyData<NetworkList>(16, 0x80000000 | 17, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'PerCoreUsage'
        QtMocHelpers::PropertyData<DoubleList>(18, 0x80000000 | 19, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'RamTotalBytes'
        QtMocHelpers::PropertyData<qulonglong>(20, QMetaType::ULongLong, QMC::DefaultPropertyFlags),
        // property 'RamUsagePercent'
        QtMocHelpers::PropertyData<double>(21, QMetaType::Double, QMC::DefaultPropertyFlags),
        // property 'RamUsedBytes'
        QtMocHelpers::PropertyData<qulonglong>(22, QMetaType::ULongLong, QMC::DefaultPropertyFlags),
        // property 'SwapTotalBytes'
        QtMocHelpers::PropertyData<qulonglong>(23, QMetaType::ULongLong, QMC::DefaultPropertyFlags),
        // property 'SwapUsedBytes'
        QtMocHelpers::PropertyData<qulonglong>(24, QMetaType::ULongLong, QMC::DefaultPropertyFlags),
        // property 'UptimeSeconds'
        QtMocHelpers::PropertyData<qulonglong>(25, QMetaType::ULongLong, QMC::DefaultPropertyFlags),
    };
    QtMocHelpers::UintData qt_enums {
    };
    QtMocHelpers::UintData qt_constructors {};
    QtMocHelpers::ClassInfos qt_classinfo({
            {    1,    2 },
            {    3,    4 },
    });
    return QtMocHelpers::metaObjectData<SystemStatsAdaptor, qt_meta_tag_ZN18SystemStatsAdaptorE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject SystemStatsAdaptor::staticMetaObject = { {
    QMetaObject::SuperData::link<QDBusAbstractAdaptor::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18SystemStatsAdaptorE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18SystemStatsAdaptorE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN18SystemStatsAdaptorE_t>.metaTypes,
    nullptr
} };

void SystemStatsAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SystemStatsAdaptor *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->StatsUpdated(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (SystemStatsAdaptor::*)()>(_a, &SystemStatsAdaptor::StatsUpdated, 0))
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

const QMetaObject *SystemStatsAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SystemStatsAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18SystemStatsAdaptorE_t>.strings))
        return static_cast<void*>(this);
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int SystemStatsAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
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
void SystemStatsAdaptor::StatsUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
