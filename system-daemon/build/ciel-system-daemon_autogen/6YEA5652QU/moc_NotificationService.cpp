/****************************************************************************
** Meta object code from reading C++ file 'NotificationService.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/NotificationService.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NotificationService.hpp' doesn't include <QObject>."
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
struct qt_meta_tag_ZN19NotificationServiceE_t {};
} // unnamed namespace

template <> constexpr inline auto NotificationService::qt_create_metaobjectdata<qt_meta_tag_ZN19NotificationServiceE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "NotificationService",
        "D-Bus Interface",
        "org.freedesktop.Notifications",
        "NotificationClosed",
        "",
        "id",
        "reason",
        "ActionInvoked",
        "action_key",
        "NotificationAdded",
        "appName",
        "summary",
        "body",
        "appIcon",
        "urgency",
        "NotificationRemoved",
        "UnreadCountChanged",
        "count",
        "doNotDisturbChanged",
        "enabled",
        "unreadCountChanged",
        "Notify",
        "app_name",
        "replaces_id",
        "app_icon",
        "actions",
        "QVariantMap",
        "hints",
        "expire_timeout",
        "CloseNotification",
        "GetCapabilities",
        "GetServerInformation",
        "QString&",
        "vendor",
        "version",
        "spec_version",
        "SetDoNotDisturb",
        "ClearAll",
        "Dismiss",
        "DoNotDisturb",
        "UnreadCount"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'NotificationClosed'
        QtMocHelpers::SignalData<void(uint, uint)>(3, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 5 }, { QMetaType::UInt, 6 },
        }}),
        // Signal 'ActionInvoked'
        QtMocHelpers::SignalData<void(uint, const QString &)>(7, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 5 }, { QMetaType::QString, 8 },
        }}),
        // Signal 'NotificationAdded'
        QtMocHelpers::SignalData<void(uint, const QString &, const QString &, const QString &, const QString &, uchar)>(9, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 5 }, { QMetaType::QString, 10 }, { QMetaType::QString, 11 }, { QMetaType::QString, 12 },
            { QMetaType::QString, 13 }, { QMetaType::UChar, 14 },
        }}),
        // Signal 'NotificationRemoved'
        QtMocHelpers::SignalData<void(uint)>(15, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 5 },
        }}),
        // Signal 'UnreadCountChanged'
        QtMocHelpers::SignalData<void(uint)>(16, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 17 },
        }}),
        // Signal 'doNotDisturbChanged'
        QtMocHelpers::SignalData<void(bool)>(18, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 19 },
        }}),
        // Signal 'unreadCountChanged'
        QtMocHelpers::SignalData<void(uint)>(20, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 17 },
        }}),
        // Slot 'Notify'
        QtMocHelpers::SlotData<uint(const QString &, uint, const QString &, const QString &, const QString &, const QStringList &, const QVariantMap &, int)>(21, 4, QMC::AccessPublic, QMetaType::UInt, {{
            { QMetaType::QString, 22 }, { QMetaType::UInt, 23 }, { QMetaType::QString, 24 }, { QMetaType::QString, 11 },
            { QMetaType::QString, 12 }, { QMetaType::QStringList, 25 }, { 0x80000000 | 26, 27 }, { QMetaType::Int, 28 },
        }}),
        // Slot 'CloseNotification'
        QtMocHelpers::SlotData<void(uint)>(29, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 5 },
        }}),
        // Slot 'GetCapabilities'
        QtMocHelpers::SlotData<QStringList()>(30, 4, QMC::AccessPublic, QMetaType::QStringList),
        // Slot 'GetServerInformation'
        QtMocHelpers::SlotData<QString(QString &, QString &, QString &)>(31, 4, QMC::AccessPublic, QMetaType::QString, {{
            { 0x80000000 | 32, 33 }, { 0x80000000 | 32, 34 }, { 0x80000000 | 32, 35 },
        }}),
        // Slot 'SetDoNotDisturb'
        QtMocHelpers::SlotData<void(bool)>(36, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 19 },
        }}),
        // Slot 'ClearAll'
        QtMocHelpers::SlotData<void()>(37, 4, QMC::AccessPublic, QMetaType::Void),
        // Slot 'Dismiss'
        QtMocHelpers::SlotData<void(uint)>(38, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 5 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'DoNotDisturb'
        QtMocHelpers::PropertyData<bool>(39, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable, 5),
        // property 'UnreadCount'
        QtMocHelpers::PropertyData<uint>(40, QMetaType::UInt, QMC::DefaultPropertyFlags, 6),
    };
    QtMocHelpers::UintData qt_enums {
    };
    QtMocHelpers::UintData qt_constructors {};
    QtMocHelpers::ClassInfos qt_classinfo({
            {    1,    2 },
    });
    return QtMocHelpers::metaObjectData<NotificationService, qt_meta_tag_ZN19NotificationServiceE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject NotificationService::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19NotificationServiceE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19NotificationServiceE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN19NotificationServiceE_t>.metaTypes,
    nullptr
} };

void NotificationService::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<NotificationService *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->NotificationClosed((*reinterpret_cast<std::add_pointer_t<uint>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<uint>>(_a[2]))); break;
        case 1: _t->ActionInvoked((*reinterpret_cast<std::add_pointer_t<uint>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 2: _t->NotificationAdded((*reinterpret_cast<std::add_pointer_t<uint>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[5])),(*reinterpret_cast<std::add_pointer_t<uchar>>(_a[6]))); break;
        case 3: _t->NotificationRemoved((*reinterpret_cast<std::add_pointer_t<uint>>(_a[1]))); break;
        case 4: _t->UnreadCountChanged((*reinterpret_cast<std::add_pointer_t<uint>>(_a[1]))); break;
        case 5: _t->doNotDisturbChanged((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 6: _t->unreadCountChanged((*reinterpret_cast<std::add_pointer_t<uint>>(_a[1]))); break;
        case 7: { uint _r = _t->Notify((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<uint>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[5])),(*reinterpret_cast<std::add_pointer_t<QStringList>>(_a[6])),(*reinterpret_cast<std::add_pointer_t<QVariantMap>>(_a[7])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[8])));
            if (_a[0]) *reinterpret_cast<uint*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->CloseNotification((*reinterpret_cast<std::add_pointer_t<uint>>(_a[1]))); break;
        case 9: { QStringList _r = _t->GetCapabilities();
            if (_a[0]) *reinterpret_cast<QStringList*>(_a[0]) = std::move(_r); }  break;
        case 10: { QString _r = _t->GetServerInformation((*reinterpret_cast<std::add_pointer_t<QString&>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString&>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString&>>(_a[3])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 11: _t->SetDoNotDisturb((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 12: _t->ClearAll(); break;
        case 13: _t->Dismiss((*reinterpret_cast<std::add_pointer_t<uint>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (NotificationService::*)(uint , uint )>(_a, &NotificationService::NotificationClosed, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (NotificationService::*)(uint , const QString & )>(_a, &NotificationService::ActionInvoked, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (NotificationService::*)(uint , const QString & , const QString & , const QString & , const QString & , uchar )>(_a, &NotificationService::NotificationAdded, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (NotificationService::*)(uint )>(_a, &NotificationService::NotificationRemoved, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (NotificationService::*)(uint )>(_a, &NotificationService::UnreadCountChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (NotificationService::*)(bool )>(_a, &NotificationService::doNotDisturbChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (NotificationService::*)(uint )>(_a, &NotificationService::unreadCountChanged, 6))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<bool*>(_v) = _t->isDoNotDisturb(); break;
        case 1: *reinterpret_cast<uint*>(_v) = _t->unreadCount(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->SetDoNotDisturb(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *NotificationService::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NotificationService::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19NotificationServiceE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NotificationService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 14;
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
void NotificationService::NotificationClosed(uint _t1, uint _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}

// SIGNAL 1
void NotificationService::ActionInvoked(uint _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}

// SIGNAL 2
void NotificationService::NotificationAdded(uint _t1, const QString & _t2, const QString & _t3, const QString & _t4, const QString & _t5, uchar _t6)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2, _t3, _t4, _t5, _t6);
}

// SIGNAL 3
void NotificationService::NotificationRemoved(uint _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void NotificationService::UnreadCountChanged(uint _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void NotificationService::doNotDisturbChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void NotificationService::unreadCountChanged(uint _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}
QT_WARNING_POP
