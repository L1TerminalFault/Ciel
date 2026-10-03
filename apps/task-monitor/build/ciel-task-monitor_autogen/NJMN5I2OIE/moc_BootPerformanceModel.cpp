/****************************************************************************
** Meta object code from reading C++ file 'BootPerformanceModel.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/boot/BootPerformanceModel.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BootPerformanceModel.hpp' doesn't include <QObject>."
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
struct qt_meta_tag_ZN14BootScanWorkerE_t {};
} // unnamed namespace

template <> constexpr inline auto BootScanWorker::qt_create_metaobjectdata<qt_meta_tag_ZN14BootScanWorkerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "BootScanWorker",
        "scanCompleted",
        "",
        "BootScanPayload",
        "payload",
        "performScan"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'scanCompleted'
        QtMocHelpers::SignalData<void(const BootScanPayload &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Slot 'performScan'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<BootScanWorker, qt_meta_tag_ZN14BootScanWorkerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject BootScanWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14BootScanWorkerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14BootScanWorkerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14BootScanWorkerE_t>.metaTypes,
    nullptr
} };

void BootScanWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<BootScanWorker *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->scanCompleted((*reinterpret_cast<std::add_pointer_t<BootScanPayload>>(_a[1]))); break;
        case 1: _t->performScan(); break;
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
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< BootScanPayload >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (BootScanWorker::*)(const BootScanPayload & )>(_a, &BootScanWorker::scanCompleted, 0))
            return;
    }
}

const QMetaObject *BootScanWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BootScanWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14BootScanWorkerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int BootScanWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void BootScanWorker::scanCompleted(const BootScanPayload & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}
namespace {
struct qt_meta_tag_ZN20BootPerformanceModelE_t {};
} // unnamed namespace

template <> constexpr inline auto BootPerformanceModel::qt_create_metaobjectdata<qt_meta_tag_ZN20BootPerformanceModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "BootPerformanceModel",
        "QML.Element",
        "auto",
        "bootTimesChanged",
        "",
        "loadingChanged",
        "requestScan",
        "onScanCompleted",
        "BootScanPayload",
        "payload",
        "rowCount",
        "QModelIndex",
        "parent",
        "refresh",
        "firmwareSec",
        "loaderSec",
        "kernelSec",
        "initrdSec",
        "userspaceSec",
        "totalSec",
        "totalTimeString",
        "segments",
        "QVariantList",
        "isLoading"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'bootTimesChanged'
        QtMocHelpers::SignalData<void()>(3, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'loadingChanged'
        QtMocHelpers::SignalData<void()>(5, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'requestScan'
        QtMocHelpers::SignalData<void()>(6, 4, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onScanCompleted'
        QtMocHelpers::SlotData<void(const BootScanPayload &)>(7, 4, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 8, 9 },
        }}),
        // Method 'rowCount'
        QtMocHelpers::MethodData<int(const QModelIndex &) const>(10, 4, QMC::AccessPublic, QMetaType::Int, {{
            { 0x80000000 | 11, 12 },
        }}),
        // Method 'rowCount'
        QtMocHelpers::MethodData<int() const>(10, 4, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Int),
        // Method 'refresh'
        QtMocHelpers::MethodData<void()>(13, 4, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'firmwareSec'
        QtMocHelpers::PropertyData<double>(14, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'loaderSec'
        QtMocHelpers::PropertyData<double>(15, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'kernelSec'
        QtMocHelpers::PropertyData<double>(16, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'initrdSec'
        QtMocHelpers::PropertyData<double>(17, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'userspaceSec'
        QtMocHelpers::PropertyData<double>(18, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'totalSec'
        QtMocHelpers::PropertyData<double>(19, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'totalTimeString'
        QtMocHelpers::PropertyData<QString>(20, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'segments'
        QtMocHelpers::PropertyData<QVariantList>(21, 0x80000000 | 22, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'isLoading'
        QtMocHelpers::PropertyData<bool>(23, QMetaType::Bool, QMC::DefaultPropertyFlags, 1),
    };
    QtMocHelpers::UintData qt_enums {
    };
    QtMocHelpers::UintData qt_constructors {};
    QtMocHelpers::ClassInfos qt_classinfo({
            {    1,    2 },
    });
    return QtMocHelpers::metaObjectData<BootPerformanceModel, void>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject BootPerformanceModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractListModel::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20BootPerformanceModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20BootPerformanceModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN20BootPerformanceModelE_t>.metaTypes,
    nullptr
} };

void BootPerformanceModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<BootPerformanceModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->bootTimesChanged(); break;
        case 1: _t->loadingChanged(); break;
        case 2: _t->requestScan(); break;
        case 3: _t->onScanCompleted((*reinterpret_cast<std::add_pointer_t<BootScanPayload>>(_a[1]))); break;
        case 4: { int _r = _t->rowCount((*reinterpret_cast<std::add_pointer_t<QModelIndex>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 5: { int _r = _t->rowCount();
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 6: _t->refresh(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< BootScanPayload >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (BootPerformanceModel::*)()>(_a, &BootPerformanceModel::bootTimesChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (BootPerformanceModel::*)()>(_a, &BootPerformanceModel::loadingChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (BootPerformanceModel::*)()>(_a, &BootPerformanceModel::requestScan, 2))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<double*>(_v) = _t->firmwareSec(); break;
        case 1: *reinterpret_cast<double*>(_v) = _t->loaderSec(); break;
        case 2: *reinterpret_cast<double*>(_v) = _t->kernelSec(); break;
        case 3: *reinterpret_cast<double*>(_v) = _t->initrdSec(); break;
        case 4: *reinterpret_cast<double*>(_v) = _t->userspaceSec(); break;
        case 5: *reinterpret_cast<double*>(_v) = _t->totalSec(); break;
        case 6: *reinterpret_cast<QString*>(_v) = _t->totalTimeString(); break;
        case 7: *reinterpret_cast<QVariantList*>(_v) = _t->segments(); break;
        case 8: *reinterpret_cast<bool*>(_v) = _t->isLoading(); break;
        default: break;
        }
    }
}

const QMetaObject *BootPerformanceModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BootPerformanceModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20BootPerformanceModelE_t>.strings))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int BootPerformanceModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void BootPerformanceModel::bootTimesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void BootPerformanceModel::loadingChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void BootPerformanceModel::requestScan()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
