/****************************************************************************
** Meta object code from reading C++ file 'SwipeGestureFilter.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/core/SwipeGestureFilter.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SwipeGestureFilter.hpp' doesn't include <QObject>."
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
struct qt_meta_tag_ZN18SwipeGestureFilterE_t {};
} // unnamed namespace

template <> constexpr inline auto SwipeGestureFilter::qt_create_metaobjectdata<qt_meta_tag_ZN18SwipeGestureFilterE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "SwipeGestureFilter",
        "QML.Element",
        "auto",
        "gestureUpdated",
        "",
        "activeChanged",
        "backTriggered",
        "forwardTriggered",
        "canGoBackChanged",
        "canGoForwardChanged",
        "thresholdChanged",
        "commitGesture",
        "reset",
        "backProgress",
        "forwardProgress",
        "active",
        "canGoBack",
        "canGoForward",
        "threshold"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'gestureUpdated'
        QtMocHelpers::SignalData<void()>(3, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'activeChanged'
        QtMocHelpers::SignalData<void()>(5, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'backTriggered'
        QtMocHelpers::SignalData<void()>(6, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'forwardTriggered'
        QtMocHelpers::SignalData<void()>(7, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'canGoBackChanged'
        QtMocHelpers::SignalData<void()>(8, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'canGoForwardChanged'
        QtMocHelpers::SignalData<void()>(9, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'thresholdChanged'
        QtMocHelpers::SignalData<void()>(10, 4, QMC::AccessPublic, QMetaType::Void),
        // Slot 'commitGesture'
        QtMocHelpers::SlotData<void()>(11, 4, QMC::AccessPrivate, QMetaType::Void),
        // Method 'reset'
        QtMocHelpers::MethodData<void()>(12, 4, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'backProgress'
        QtMocHelpers::PropertyData<double>(13, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'forwardProgress'
        QtMocHelpers::PropertyData<double>(14, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'active'
        QtMocHelpers::PropertyData<bool>(15, QMetaType::Bool, QMC::DefaultPropertyFlags, 1),
        // property 'canGoBack'
        QtMocHelpers::PropertyData<bool>(16, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 4),
        // property 'canGoForward'
        QtMocHelpers::PropertyData<bool>(17, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 5),
        // property 'threshold'
        QtMocHelpers::PropertyData<double>(18, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 6),
    };
    QtMocHelpers::UintData qt_enums {
    };
    QtMocHelpers::UintData qt_constructors {};
    QtMocHelpers::ClassInfos qt_classinfo({
            {    1,    2 },
    });
    return QtMocHelpers::metaObjectData<SwipeGestureFilter, void>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject SwipeGestureFilter::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18SwipeGestureFilterE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18SwipeGestureFilterE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN18SwipeGestureFilterE_t>.metaTypes,
    nullptr
} };

void SwipeGestureFilter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SwipeGestureFilter *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->gestureUpdated(); break;
        case 1: _t->activeChanged(); break;
        case 2: _t->backTriggered(); break;
        case 3: _t->forwardTriggered(); break;
        case 4: _t->canGoBackChanged(); break;
        case 5: _t->canGoForwardChanged(); break;
        case 6: _t->thresholdChanged(); break;
        case 7: _t->commitGesture(); break;
        case 8: _t->reset(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (SwipeGestureFilter::*)()>(_a, &SwipeGestureFilter::gestureUpdated, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (SwipeGestureFilter::*)()>(_a, &SwipeGestureFilter::activeChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (SwipeGestureFilter::*)()>(_a, &SwipeGestureFilter::backTriggered, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (SwipeGestureFilter::*)()>(_a, &SwipeGestureFilter::forwardTriggered, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (SwipeGestureFilter::*)()>(_a, &SwipeGestureFilter::canGoBackChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (SwipeGestureFilter::*)()>(_a, &SwipeGestureFilter::canGoForwardChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (SwipeGestureFilter::*)()>(_a, &SwipeGestureFilter::thresholdChanged, 6))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<double*>(_v) = _t->backProgress(); break;
        case 1: *reinterpret_cast<double*>(_v) = _t->forwardProgress(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->active(); break;
        case 3: *reinterpret_cast<bool*>(_v) = _t->canGoBack(); break;
        case 4: *reinterpret_cast<bool*>(_v) = _t->canGoForward(); break;
        case 5: *reinterpret_cast<double*>(_v) = _t->threshold(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 3: _t->setCanGoBack(*reinterpret_cast<bool*>(_v)); break;
        case 4: _t->setCanGoForward(*reinterpret_cast<bool*>(_v)); break;
        case 5: _t->setThreshold(*reinterpret_cast<double*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *SwipeGestureFilter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SwipeGestureFilter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18SwipeGestureFilterE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SwipeGestureFilter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void SwipeGestureFilter::gestureUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SwipeGestureFilter::activeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SwipeGestureFilter::backTriggered()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void SwipeGestureFilter::forwardTriggered()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void SwipeGestureFilter::canGoBackChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void SwipeGestureFilter::canGoForwardChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void SwipeGestureFilter::thresholdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP
