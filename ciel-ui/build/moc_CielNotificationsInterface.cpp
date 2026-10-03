/****************************************************************************
** Meta object code from reading C++ file 'CielNotificationsInterface.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CielNotificationsInterface.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CielNotificationsInterface.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN29OrgCielNotificationsInterfaceE_t {};
} // unnamed namespace

template <> constexpr inline auto OrgCielNotificationsInterface::qt_create_metaobjectdata<qt_meta_tag_ZN29OrgCielNotificationsInterfaceE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "OrgCielNotificationsInterface",
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
        "QDBusPendingReply<>",
        "Dismiss",
        "SetDoNotDisturb",
        "enabled",
        "DoNotDisturb",
        "UnreadCount"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'NotificationAdded'
        QtMocHelpers::SignalData<void(uint, const QString &, const QString &, const QString &, const QString &, uchar)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 3 }, { QMetaType::QString, 4 }, { QMetaType::QString, 5 }, { QMetaType::QString, 6 },
            { QMetaType::QString, 7 }, { QMetaType::UChar, 8 },
        }}),
        // Signal 'NotificationRemoved'
        QtMocHelpers::SignalData<void(uint)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 3 },
        }}),
        // Signal 'UnreadCountChanged'
        QtMocHelpers::SignalData<void(uint)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 11 },
        }}),
        // Slot 'ClearAll'
        QtMocHelpers::SlotData<QDBusPendingReply<>()>(12, 2, QMC::AccessPublic, 0x80000000 | 13),
        // Slot 'Dismiss'
        QtMocHelpers::SlotData<QDBusPendingReply<>(uint)>(14, 2, QMC::AccessPublic, 0x80000000 | 13, {{
            { QMetaType::UInt, 3 },
        }}),
        // Slot 'SetDoNotDisturb'
        QtMocHelpers::SlotData<QDBusPendingReply<>(bool)>(15, 2, QMC::AccessPublic, 0x80000000 | 13, {{
            { QMetaType::Bool, 16 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'DoNotDisturb'
        QtMocHelpers::PropertyData<bool>(17, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'UnreadCount'
        QtMocHelpers::PropertyData<uint>(18, QMetaType::UInt, QMC::DefaultPropertyFlags),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<OrgCielNotificationsInterface, qt_meta_tag_ZN29OrgCielNotificationsInterfaceE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject OrgCielNotificationsInterface::staticMetaObject = { {
    QMetaObject::SuperData::link<QDBusAbstractInterface::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN29OrgCielNotificationsInterfaceE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN29OrgCielNotificationsInterfaceE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN29OrgCielNotificationsInterfaceE_t>.metaTypes,
    nullptr
} };

void OrgCielNotificationsInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<OrgCielNotificationsInterface *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->NotificationAdded((*reinterpret_cast<std::add_pointer_t<uint>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[5])),(*reinterpret_cast<std::add_pointer_t<uchar>>(_a[6]))); break;
        case 1: _t->NotificationRemoved((*reinterpret_cast<std::add_pointer_t<uint>>(_a[1]))); break;
        case 2: _t->UnreadCountChanged((*reinterpret_cast<std::add_pointer_t<uint>>(_a[1]))); break;
        case 3: { QDBusPendingReply<> _r = _t->ClearAll();
            if (_a[0]) *reinterpret_cast<QDBusPendingReply<>*>(_a[0]) = std::move(_r); }  break;
        case 4: { QDBusPendingReply<> _r = _t->Dismiss((*reinterpret_cast<std::add_pointer_t<uint>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QDBusPendingReply<>*>(_a[0]) = std::move(_r); }  break;
        case 5: { QDBusPendingReply<> _r = _t->SetDoNotDisturb((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QDBusPendingReply<>*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (OrgCielNotificationsInterface::*)(uint , const QString & , const QString & , const QString & , const QString & , uchar )>(_a, &OrgCielNotificationsInterface::NotificationAdded, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (OrgCielNotificationsInterface::*)(uint )>(_a, &OrgCielNotificationsInterface::NotificationRemoved, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (OrgCielNotificationsInterface::*)(uint )>(_a, &OrgCielNotificationsInterface::UnreadCountChanged, 2))
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

const QMetaObject *OrgCielNotificationsInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OrgCielNotificationsInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN29OrgCielNotificationsInterfaceE_t>.strings))
        return static_cast<void*>(this);
    return QDBusAbstractInterface::qt_metacast(_clname);
}

int OrgCielNotificationsInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractInterface::qt_metacall(_c, _id, _a);
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
void OrgCielNotificationsInterface::NotificationAdded(uint _t1, const QString & _t2, const QString & _t3, const QString & _t4, const QString & _t5, uchar _t6)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3, _t4, _t5, _t6);
}

// SIGNAL 1
void OrgCielNotificationsInterface::NotificationRemoved(uint _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void OrgCielNotificationsInterface::UnreadCountChanged(uint _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}
QT_WARNING_POP
