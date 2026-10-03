/****************************************************************************
** Meta object code from reading C++ file 'CielNotificationsAdaptor.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CielNotificationsAdaptor.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CielNotificationsAdaptor.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN24CielNotificationsAdaptorE_t {};
} // unnamed namespace

template <> constexpr inline auto CielNotificationsAdaptor::qt_create_metaobjectdata<qt_meta_tag_ZN24CielNotificationsAdaptorE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "CielNotificationsAdaptor",
        "D-Bus Interface",
        "org.ciel.Notifications",
        "D-Bus Introspection",
        "  <interface name=\"org.ciel.Notifications\">\n    <property acces"
        "s=\"readwrite\" type=\"b\" name=\"DoNotDisturb\"/>\n    <property "
        "access=\"read\" type=\"u\" name=\"UnreadCount\"/>\n    <method nam"
        "e=\"SetDoNotDisturb\">\n      <arg direction=\"in\" type=\"b\" nam"
        "e=\"enabled\"/>\n    </method>\n    <method name=\"ClearAll\"/>\n "
        "   <method name=\"Dismiss\">\n      <arg direction=\"in\" type=\"u"
        "\" name=\"id\"/>\n    </method>\n    <signal name=\"NotificationAd"
        "ded\">\n      <arg type=\"u\" name=\"id\"/>\n      <arg type=\"s\""
        " name=\"appName\"/>\n      <arg type=\"s\" name=\"summary\"/>\n   "
        "   <arg type=\"s\" name=\"body\"/>\n      <arg type=\"s\" name=\"a"
        "ppIcon\"/>\n      <arg type=\"y\" name=\"urgency\"/>\n    </signal"
        ">\n    <signal name=\"NotificationRemoved\">\n      <arg type=\"u\""
        " name=\"id\"/>\n    </signal>\n    <signal name=\"UnreadCountChang"
        "ed\">\n      <arg type=\"u\" name=\"count\"/>\n    </signal>\n  </"
        "interface>\n",
        "NotificationAdded",
        "",
        "id",
        "appName",
        "summary",
        "body",
        "appIcon",
        "urgency",
        "NotificationRemoved",
        "UnreadCountChanged",
        "count",
        "ClearAll",
        "Dismiss",
        "SetDoNotDisturb",
        "enabled",
        "DoNotDisturb",
        "UnreadCount"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'NotificationAdded'
        QtMocHelpers::SignalData<void(uint, const QString &, const QString &, const QString &, const QString &, uchar)>(5, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 7 }, { QMetaType::QString, 8 }, { QMetaType::QString, 9 }, { QMetaType::QString, 10 },
            { QMetaType::QString, 11 }, { QMetaType::UChar, 12 },
        }}),
        // Signal 'NotificationRemoved'
        QtMocHelpers::SignalData<void(uint)>(13, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 7 },
        }}),
        // Signal 'UnreadCountChanged'
        QtMocHelpers::SignalData<void(uint)>(14, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 15 },
        }}),
        // Slot 'ClearAll'
        QtMocHelpers::SlotData<void()>(16, 6, QMC::AccessPublic, QMetaType::Void),
        // Slot 'Dismiss'
        QtMocHelpers::SlotData<void(uint)>(17, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 7 },
        }}),
        // Slot 'SetDoNotDisturb'
        QtMocHelpers::SlotData<void(bool)>(18, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 19 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'DoNotDisturb'
        QtMocHelpers::PropertyData<bool>(20, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'UnreadCount'
        QtMocHelpers::PropertyData<uint>(21, QMetaType::UInt, QMC::DefaultPropertyFlags),
    };
    QtMocHelpers::UintData qt_enums {
    };
    QtMocHelpers::UintData qt_constructors {};
    QtMocHelpers::ClassInfos qt_classinfo({
            {    1,    2 },
            {    3,    4 },
    });
    return QtMocHelpers::metaObjectData<CielNotificationsAdaptor, qt_meta_tag_ZN24CielNotificationsAdaptorE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject CielNotificationsAdaptor::staticMetaObject = { {
    QMetaObject::SuperData::link<QDBusAbstractAdaptor::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN24CielNotificationsAdaptorE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN24CielNotificationsAdaptorE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN24CielNotificationsAdaptorE_t>.metaTypes,
    nullptr
} };

void CielNotificationsAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<CielNotificationsAdaptor *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->NotificationAdded((*reinterpret_cast<std::add_pointer_t<uint>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[5])),(*reinterpret_cast<std::add_pointer_t<uchar>>(_a[6]))); break;
        case 1: _t->NotificationRemoved((*reinterpret_cast<std::add_pointer_t<uint>>(_a[1]))); break;
        case 2: _t->UnreadCountChanged((*reinterpret_cast<std::add_pointer_t<uint>>(_a[1]))); break;
        case 3: _t->ClearAll(); break;
        case 4: _t->Dismiss((*reinterpret_cast<std::add_pointer_t<uint>>(_a[1]))); break;
        case 5: _t->SetDoNotDisturb((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (CielNotificationsAdaptor::*)(uint , const QString & , const QString & , const QString & , const QString & , uchar )>(_a, &CielNotificationsAdaptor::NotificationAdded, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (CielNotificationsAdaptor::*)(uint )>(_a, &CielNotificationsAdaptor::NotificationRemoved, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (CielNotificationsAdaptor::*)(uint )>(_a, &CielNotificationsAdaptor::UnreadCountChanged, 2))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<bool*>(_v) = _t->doNotDisturb(); break;
        case 1: *reinterpret_cast<uint*>(_v) = _t->unreadCount(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setDoNotDisturb(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *CielNotificationsAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CielNotificationsAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN24CielNotificationsAdaptorE_t>.strings))
        return static_cast<void*>(this);
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int CielNotificationsAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void CielNotificationsAdaptor::NotificationAdded(uint _t1, const QString & _t2, const QString & _t3, const QString & _t4, const QString & _t5, uchar _t6)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3, _t4, _t5, _t6);
}

// SIGNAL 1
void CielNotificationsAdaptor::NotificationRemoved(uint _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void CielNotificationsAdaptor::UnreadCountChanged(uint _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}
QT_WARNING_POP
