/****************************************************************************
** Meta object code from reading C++ file 'ProcessTableModel.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/process/ProcessTableModel.hpp"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ProcessTableModel.hpp' doesn't include <QObject>."
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
struct qt_meta_tag_ZN17ProcessScanWorkerE_t {};
} // unnamed namespace

template <> constexpr inline auto ProcessScanWorker::qt_create_metaobjectdata<qt_meta_tag_ZN17ProcessScanWorkerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ProcessScanWorker",
        "scanCompleted",
        "",
        "WorkerScanResult",
        "result",
        "performScan",
        "sortColumn",
        "sortOrder"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'scanCompleted'
        QtMocHelpers::SignalData<void(const WorkerScanResult &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Slot 'performScan'
        QtMocHelpers::SlotData<void(int, int)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 6 }, { QMetaType::Int, 7 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ProcessScanWorker, qt_meta_tag_ZN17ProcessScanWorkerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ProcessScanWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17ProcessScanWorkerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17ProcessScanWorkerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN17ProcessScanWorkerE_t>.metaTypes,
    nullptr
} };

void ProcessScanWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ProcessScanWorker *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->scanCompleted((*reinterpret_cast<std::add_pointer_t<WorkerScanResult>>(_a[1]))); break;
        case 1: _t->performScan((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< WorkerScanResult >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ProcessScanWorker::*)(const WorkerScanResult & )>(_a, &ProcessScanWorker::scanCompleted, 0))
            return;
    }
}

const QMetaObject *ProcessScanWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProcessScanWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17ProcessScanWorkerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ProcessScanWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void ProcessScanWorker::scanCompleted(const WorkerScanResult & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}
namespace {
struct qt_meta_tag_ZN17ProcessTableModelE_t {};
} // unnamed namespace

template <> constexpr inline auto ProcessTableModel::qt_create_metaobjectdata<qt_meta_tag_ZN17ProcessTableModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ProcessTableModel",
        "QML.Element",
        "auto",
        "filterTextChanged",
        "",
        "isSearchingChanged",
        "sortColumnChanged",
        "sortOrderChanged",
        "systemTotalsChanged",
        "historyUpdated",
        "perCoreUsageChanged",
        "gpusChanged",
        "disksChanged",
        "networksChanged",
        "requestScan",
        "sortColumn",
        "sortOrder",
        "onScanCompleted",
        "WorkerScanResult",
        "result",
        "coreHistory",
        "QList<double>",
        "coreIndex",
        "gpuName",
        "index",
        "gpuId",
        "gpuUsage",
        "gpuTemp",
        "gpuVramUsed",
        "gpuVramTotal",
        "gpuHistory",
        "diskMount",
        "diskFsType",
        "diskTotalBytes",
        "diskUsedBytes",
        "diskReadSpeed",
        "diskWriteSpeed",
        "networkName",
        "networkIsWireless",
        "networkRxSpeed",
        "networkTxSpeed",
        "formatSpeed",
        "bytesSec",
        "formatBytes",
        "bytes",
        "sortByColumn",
        "column",
        "order",
        "refresh",
        "filterText",
        "isSearching",
        "systemCpuUsage",
        "systemRamUsagePercent",
        "systemRamUsedBytes",
        "systemRamTotalBytes",
        "systemRamAvailBytes",
        "systemSwapUsedBytes",
        "systemSwapTotalBytes",
        "systemDiskSpeed",
        "systemDiskBytesSec",
        "systemDiskReadBytesSec",
        "systemDiskWriteBytesSec",
        "systemNetSpeed",
        "systemNetBytesSec",
        "systemNetRxBytesSec",
        "systemNetTxBytesSec",
        "cpuHistory",
        "ramHistory",
        "diskHistory",
        "netHistory",
        "perCoreUsage",
        "coreCount",
        "gpuCount",
        "diskCount",
        "networkCount",
        "historyRevision"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'filterTextChanged'
        QtMocHelpers::SignalData<void()>(3, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'isSearchingChanged'
        QtMocHelpers::SignalData<void()>(5, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'sortColumnChanged'
        QtMocHelpers::SignalData<void()>(6, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'sortOrderChanged'
        QtMocHelpers::SignalData<void()>(7, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'systemTotalsChanged'
        QtMocHelpers::SignalData<void()>(8, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'historyUpdated'
        QtMocHelpers::SignalData<void()>(9, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'perCoreUsageChanged'
        QtMocHelpers::SignalData<void()>(10, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'gpusChanged'
        QtMocHelpers::SignalData<void()>(11, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'disksChanged'
        QtMocHelpers::SignalData<void()>(12, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'networksChanged'
        QtMocHelpers::SignalData<void()>(13, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'requestScan'
        QtMocHelpers::SignalData<void(int, int)>(14, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 15 }, { QMetaType::Int, 16 },
        }}),
        // Slot 'onScanCompleted'
        QtMocHelpers::SlotData<void(const WorkerScanResult &)>(17, 4, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 18, 19 },
        }}),
        // Method 'coreHistory'
        QtMocHelpers::MethodData<QList<double>(int) const>(20, 4, QMC::AccessPublic, 0x80000000 | 21, {{
            { QMetaType::Int, 22 },
        }}),
        // Method 'gpuName'
        QtMocHelpers::MethodData<QString(int) const>(23, 4, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::Int, 24 },
        }}),
        // Method 'gpuId'
        QtMocHelpers::MethodData<QString(int) const>(25, 4, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::Int, 24 },
        }}),
        // Method 'gpuUsage'
        QtMocHelpers::MethodData<double(int) const>(26, 4, QMC::AccessPublic, QMetaType::Double, {{
            { QMetaType::Int, 24 },
        }}),
        // Method 'gpuTemp'
        QtMocHelpers::MethodData<double(int) const>(27, 4, QMC::AccessPublic, QMetaType::Double, {{
            { QMetaType::Int, 24 },
        }}),
        // Method 'gpuVramUsed'
        QtMocHelpers::MethodData<qulonglong(int) const>(28, 4, QMC::AccessPublic, QMetaType::ULongLong, {{
            { QMetaType::Int, 24 },
        }}),
        // Method 'gpuVramTotal'
        QtMocHelpers::MethodData<qulonglong(int) const>(29, 4, QMC::AccessPublic, QMetaType::ULongLong, {{
            { QMetaType::Int, 24 },
        }}),
        // Method 'gpuHistory'
        QtMocHelpers::MethodData<QList<double>(int) const>(30, 4, QMC::AccessPublic, 0x80000000 | 21, {{
            { QMetaType::Int, 24 },
        }}),
        // Method 'diskMount'
        QtMocHelpers::MethodData<QString(int) const>(31, 4, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::Int, 24 },
        }}),
        // Method 'diskFsType'
        QtMocHelpers::MethodData<QString(int) const>(32, 4, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::Int, 24 },
        }}),
        // Method 'diskTotalBytes'
        QtMocHelpers::MethodData<qulonglong(int) const>(33, 4, QMC::AccessPublic, QMetaType::ULongLong, {{
            { QMetaType::Int, 24 },
        }}),
        // Method 'diskUsedBytes'
        QtMocHelpers::MethodData<qulonglong(int) const>(34, 4, QMC::AccessPublic, QMetaType::ULongLong, {{
            { QMetaType::Int, 24 },
        }}),
        // Method 'diskReadSpeed'
        QtMocHelpers::MethodData<qulonglong(int) const>(35, 4, QMC::AccessPublic, QMetaType::ULongLong, {{
            { QMetaType::Int, 24 },
        }}),
        // Method 'diskWriteSpeed'
        QtMocHelpers::MethodData<qulonglong(int) const>(36, 4, QMC::AccessPublic, QMetaType::ULongLong, {{
            { QMetaType::Int, 24 },
        }}),
        // Method 'networkName'
        QtMocHelpers::MethodData<QString(int) const>(37, 4, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::Int, 24 },
        }}),
        // Method 'networkIsWireless'
        QtMocHelpers::MethodData<bool(int) const>(38, 4, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 24 },
        }}),
        // Method 'networkRxSpeed'
        QtMocHelpers::MethodData<qulonglong(int) const>(39, 4, QMC::AccessPublic, QMetaType::ULongLong, {{
            { QMetaType::Int, 24 },
        }}),
        // Method 'networkTxSpeed'
        QtMocHelpers::MethodData<qulonglong(int) const>(40, 4, QMC::AccessPublic, QMetaType::ULongLong, {{
            { QMetaType::Int, 24 },
        }}),
        // Method 'formatSpeed'
        QtMocHelpers::MethodData<QString(qulonglong) const>(41, 4, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::ULongLong, 42 },
        }}),
        // Method 'formatBytes'
        QtMocHelpers::MethodData<QString(qulonglong) const>(43, 4, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::ULongLong, 44 },
        }}),
        // Method 'sortByColumn'
        QtMocHelpers::MethodData<void(int, int)>(45, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 46 }, { QMetaType::Int, 47 },
        }}),
        // Method 'sortByColumn'
        QtMocHelpers::MethodData<void(int)>(45, 4, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::Int, 46 },
        }}),
        // Method 'refresh'
        QtMocHelpers::MethodData<void()>(48, 4, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'filterText'
        QtMocHelpers::PropertyData<QString>(49, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'isSearching'
        QtMocHelpers::PropertyData<bool>(50, QMetaType::Bool, QMC::DefaultPropertyFlags, 1),
        // property 'sortColumn'
        QtMocHelpers::PropertyData<int>(15, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 2),
        // property 'sortOrder'
        QtMocHelpers::PropertyData<int>(16, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 3),
        // property 'systemCpuUsage'
        QtMocHelpers::PropertyData<double>(51, QMetaType::Double, QMC::DefaultPropertyFlags, 4),
        // property 'systemRamUsagePercent'
        QtMocHelpers::PropertyData<double>(52, QMetaType::Double, QMC::DefaultPropertyFlags, 4),
        // property 'systemRamUsedBytes'
        QtMocHelpers::PropertyData<qulonglong>(53, QMetaType::ULongLong, QMC::DefaultPropertyFlags, 4),
        // property 'systemRamTotalBytes'
        QtMocHelpers::PropertyData<qulonglong>(54, QMetaType::ULongLong, QMC::DefaultPropertyFlags, 4),
        // property 'systemRamAvailBytes'
        QtMocHelpers::PropertyData<qulonglong>(55, QMetaType::ULongLong, QMC::DefaultPropertyFlags, 4),
        // property 'systemSwapUsedBytes'
        QtMocHelpers::PropertyData<qulonglong>(56, QMetaType::ULongLong, QMC::DefaultPropertyFlags, 4),
        // property 'systemSwapTotalBytes'
        QtMocHelpers::PropertyData<qulonglong>(57, QMetaType::ULongLong, QMC::DefaultPropertyFlags, 4),
        // property 'systemDiskSpeed'
        QtMocHelpers::PropertyData<QString>(58, QMetaType::QString, QMC::DefaultPropertyFlags, 4),
        // property 'systemDiskBytesSec'
        QtMocHelpers::PropertyData<qulonglong>(59, QMetaType::ULongLong, QMC::DefaultPropertyFlags, 4),
        // property 'systemDiskReadBytesSec'
        QtMocHelpers::PropertyData<qulonglong>(60, QMetaType::ULongLong, QMC::DefaultPropertyFlags, 4),
        // property 'systemDiskWriteBytesSec'
        QtMocHelpers::PropertyData<qulonglong>(61, QMetaType::ULongLong, QMC::DefaultPropertyFlags, 4),
        // property 'systemNetSpeed'
        QtMocHelpers::PropertyData<QString>(62, QMetaType::QString, QMC::DefaultPropertyFlags, 4),
        // property 'systemNetBytesSec'
        QtMocHelpers::PropertyData<qulonglong>(63, QMetaType::ULongLong, QMC::DefaultPropertyFlags, 4),
        // property 'systemNetRxBytesSec'
        QtMocHelpers::PropertyData<qulonglong>(64, QMetaType::ULongLong, QMC::DefaultPropertyFlags, 4),
        // property 'systemNetTxBytesSec'
        QtMocHelpers::PropertyData<qulonglong>(65, QMetaType::ULongLong, QMC::DefaultPropertyFlags, 4),
        // property 'cpuHistory'
        QtMocHelpers::PropertyData<QList<double>>(66, 0x80000000 | 21, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 5),
        // property 'ramHistory'
        QtMocHelpers::PropertyData<QList<double>>(67, 0x80000000 | 21, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 5),
        // property 'diskHistory'
        QtMocHelpers::PropertyData<QList<double>>(68, 0x80000000 | 21, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 5),
        // property 'netHistory'
        QtMocHelpers::PropertyData<QList<double>>(69, 0x80000000 | 21, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 5),
        // property 'perCoreUsage'
        QtMocHelpers::PropertyData<QList<double>>(70, 0x80000000 | 21, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 6),
        // property 'coreCount'
        QtMocHelpers::PropertyData<int>(71, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'gpuCount'
        QtMocHelpers::PropertyData<int>(72, QMetaType::Int, QMC::DefaultPropertyFlags, 7),
        // property 'diskCount'
        QtMocHelpers::PropertyData<int>(73, QMetaType::Int, QMC::DefaultPropertyFlags, 8),
        // property 'networkCount'
        QtMocHelpers::PropertyData<int>(74, QMetaType::Int, QMC::DefaultPropertyFlags, 9),
        // property 'historyRevision'
        QtMocHelpers::PropertyData<int>(75, QMetaType::Int, QMC::DefaultPropertyFlags, 5),
    };
    QtMocHelpers::UintData qt_enums {
    };
    QtMocHelpers::UintData qt_constructors {};
    QtMocHelpers::ClassInfos qt_classinfo({
            {    1,    2 },
    });
    return QtMocHelpers::metaObjectData<ProcessTableModel, void>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject ProcessTableModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractTableModel::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17ProcessTableModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17ProcessTableModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN17ProcessTableModelE_t>.metaTypes,
    nullptr
} };

void ProcessTableModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ProcessTableModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->filterTextChanged(); break;
        case 1: _t->isSearchingChanged(); break;
        case 2: _t->sortColumnChanged(); break;
        case 3: _t->sortOrderChanged(); break;
        case 4: _t->systemTotalsChanged(); break;
        case 5: _t->historyUpdated(); break;
        case 6: _t->perCoreUsageChanged(); break;
        case 7: _t->gpusChanged(); break;
        case 8: _t->disksChanged(); break;
        case 9: _t->networksChanged(); break;
        case 10: _t->requestScan((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 11: _t->onScanCompleted((*reinterpret_cast<std::add_pointer_t<WorkerScanResult>>(_a[1]))); break;
        case 12: { QList<double> _r = _t->coreHistory((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QList<double>*>(_a[0]) = std::move(_r); }  break;
        case 13: { QString _r = _t->gpuName((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 14: { QString _r = _t->gpuId((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 15: { double _r = _t->gpuUsage((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<double*>(_a[0]) = std::move(_r); }  break;
        case 16: { double _r = _t->gpuTemp((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<double*>(_a[0]) = std::move(_r); }  break;
        case 17: { qulonglong _r = _t->gpuVramUsed((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<qulonglong*>(_a[0]) = std::move(_r); }  break;
        case 18: { qulonglong _r = _t->gpuVramTotal((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<qulonglong*>(_a[0]) = std::move(_r); }  break;
        case 19: { QList<double> _r = _t->gpuHistory((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QList<double>*>(_a[0]) = std::move(_r); }  break;
        case 20: { QString _r = _t->diskMount((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 21: { QString _r = _t->diskFsType((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 22: { qulonglong _r = _t->diskTotalBytes((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<qulonglong*>(_a[0]) = std::move(_r); }  break;
        case 23: { qulonglong _r = _t->diskUsedBytes((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<qulonglong*>(_a[0]) = std::move(_r); }  break;
        case 24: { qulonglong _r = _t->diskReadSpeed((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<qulonglong*>(_a[0]) = std::move(_r); }  break;
        case 25: { qulonglong _r = _t->diskWriteSpeed((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<qulonglong*>(_a[0]) = std::move(_r); }  break;
        case 26: { QString _r = _t->networkName((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 27: { bool _r = _t->networkIsWireless((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 28: { qulonglong _r = _t->networkRxSpeed((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<qulonglong*>(_a[0]) = std::move(_r); }  break;
        case 29: { qulonglong _r = _t->networkTxSpeed((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<qulonglong*>(_a[0]) = std::move(_r); }  break;
        case 30: { QString _r = _t->formatSpeed((*reinterpret_cast<std::add_pointer_t<qulonglong>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 31: { QString _r = _t->formatBytes((*reinterpret_cast<std::add_pointer_t<qulonglong>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 32: _t->sortByColumn((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 33: _t->sortByColumn((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 34: _t->refresh(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< WorkerScanResult >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ProcessTableModel::*)()>(_a, &ProcessTableModel::filterTextChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProcessTableModel::*)()>(_a, &ProcessTableModel::isSearchingChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProcessTableModel::*)()>(_a, &ProcessTableModel::sortColumnChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProcessTableModel::*)()>(_a, &ProcessTableModel::sortOrderChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProcessTableModel::*)()>(_a, &ProcessTableModel::systemTotalsChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProcessTableModel::*)()>(_a, &ProcessTableModel::historyUpdated, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProcessTableModel::*)()>(_a, &ProcessTableModel::perCoreUsageChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProcessTableModel::*)()>(_a, &ProcessTableModel::gpusChanged, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProcessTableModel::*)()>(_a, &ProcessTableModel::disksChanged, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProcessTableModel::*)()>(_a, &ProcessTableModel::networksChanged, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProcessTableModel::*)(int , int )>(_a, &ProcessTableModel::requestScan, 10))
            return;
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 23:
        case 22:
        case 21:
        case 20:
        case 19:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<double> >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->filterText(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->isSearching(); break;
        case 2: *reinterpret_cast<int*>(_v) = _t->sortColumn(); break;
        case 3: *reinterpret_cast<int*>(_v) = _t->sortOrder(); break;
        case 4: *reinterpret_cast<double*>(_v) = _t->systemCpuUsage(); break;
        case 5: *reinterpret_cast<double*>(_v) = _t->systemRamUsagePercent(); break;
        case 6: *reinterpret_cast<qulonglong*>(_v) = _t->systemRamUsedBytes(); break;
        case 7: *reinterpret_cast<qulonglong*>(_v) = _t->systemRamTotalBytes(); break;
        case 8: *reinterpret_cast<qulonglong*>(_v) = _t->systemRamAvailBytes(); break;
        case 9: *reinterpret_cast<qulonglong*>(_v) = _t->systemSwapUsedBytes(); break;
        case 10: *reinterpret_cast<qulonglong*>(_v) = _t->systemSwapTotalBytes(); break;
        case 11: *reinterpret_cast<QString*>(_v) = _t->systemDiskSpeed(); break;
        case 12: *reinterpret_cast<qulonglong*>(_v) = _t->systemDiskBytesSec(); break;
        case 13: *reinterpret_cast<qulonglong*>(_v) = _t->systemDiskReadBytesSec(); break;
        case 14: *reinterpret_cast<qulonglong*>(_v) = _t->systemDiskWriteBytesSec(); break;
        case 15: *reinterpret_cast<QString*>(_v) = _t->systemNetSpeed(); break;
        case 16: *reinterpret_cast<qulonglong*>(_v) = _t->systemNetBytesSec(); break;
        case 17: *reinterpret_cast<qulonglong*>(_v) = _t->systemNetRxBytesSec(); break;
        case 18: *reinterpret_cast<qulonglong*>(_v) = _t->systemNetTxBytesSec(); break;
        case 19: *reinterpret_cast<QList<double>*>(_v) = _t->cpuHistory(); break;
        case 20: *reinterpret_cast<QList<double>*>(_v) = _t->ramHistory(); break;
        case 21: *reinterpret_cast<QList<double>*>(_v) = _t->diskHistory(); break;
        case 22: *reinterpret_cast<QList<double>*>(_v) = _t->netHistory(); break;
        case 23: *reinterpret_cast<QList<double>*>(_v) = _t->perCoreUsage(); break;
        case 24: *reinterpret_cast<int*>(_v) = _t->coreCount(); break;
        case 25: *reinterpret_cast<int*>(_v) = _t->gpuCount(); break;
        case 26: *reinterpret_cast<int*>(_v) = _t->diskCount(); break;
        case 27: *reinterpret_cast<int*>(_v) = _t->networkCount(); break;
        case 28: *reinterpret_cast<int*>(_v) = _t->historyRevision(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setFilterText(*reinterpret_cast<QString*>(_v)); break;
        case 2: _t->setSortColumn(*reinterpret_cast<int*>(_v)); break;
        case 3: _t->setSortOrder(*reinterpret_cast<int*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *ProcessTableModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProcessTableModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17ProcessTableModelE_t>.strings))
        return static_cast<void*>(this);
    return QAbstractTableModel::qt_metacast(_clname);
}

int ProcessTableModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractTableModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 35)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 35;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 35)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 35;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 29;
    }
    return _id;
}

// SIGNAL 0
void ProcessTableModel::filterTextChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ProcessTableModel::isSearchingChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ProcessTableModel::sortColumnChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void ProcessTableModel::sortOrderChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void ProcessTableModel::systemTotalsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void ProcessTableModel::historyUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void ProcessTableModel::perCoreUsageChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void ProcessTableModel::gpusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void ProcessTableModel::disksChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void ProcessTableModel::networksChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void ProcessTableModel::requestScan(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 10, nullptr, _t1, _t2);
}
QT_WARNING_POP
