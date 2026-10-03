/****************************************************************************
** Meta object code from reading C++ file 'NotificationsModel.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/NotificationsModel.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NotificationsModel.hpp' doesn't include <QObject>."
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
struct qt_meta_tag_ZN18NotificationsModelE_t {};
} // unnamed namespace

template <> constexpr inline auto NotificationsModel::qt_create_metaobjectdata<qt_meta_tag_ZN18NotificationsModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "NotificationsModel",
        "QML.Element",
        "auto",
        "QML.Singleton",
        "true",
        "unreadCountChanged",
        "",
        "doNotDisturbChanged",
        "popupRequested",
        "id",
        "appName",
        "summary",
        "body",
        "onNotificationAdded",
        "appIcon",
        "urgency",
        "onNotificationRemoved",
        "dismiss",
        "clearAll",
        "setDoNotDisturb",
        "enabled",
        "unreadCount",
        "doNotDisturb"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'unreadCountChanged'
        QtMocHelpers::SignalData<void()>(5, 6, QMC::AccessPublic, QMetaType::Void),
        // Signal 'doNotDisturbChanged'
        QtMocHelpers::SignalData<void()>(7, 6, QMC::AccessPublic, QMetaType::Void),
        // Signal 'popupRequested'
        QtMocHelpers::SignalData<void(uint, const QString &, const QString &, const QString &)>(8, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 9 }, { QMetaType::QString, 10 }, { QMetaType::QString, 11 }, { QMetaType::QString, 12 },
        }}),
        // Slot 'onNotificationAdded'
        QtMocHelpers::SlotData<void(uint, const QString &, const QString &, const QString &, const QString &, uchar)>(13, 6, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::UInt, 9 }, { QMetaType::QString, 10 }, { QMetaType::QString, 11 }, { QMetaType::QString, 12 },
            { QMetaType::QString, 14 }, { QMetaType::UChar, 15 },
        }}),
        // Slot 'onNotificationRemoved'
        QtMocHelpers::SlotData<void(uint)>(16, 6, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::UInt, 9 },
        }}),
        // Method 'dismiss'
        QtMocHelpers::MethodData<void(uint)>(17, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 9 },
        }}),
        // Method 'clearAll'
        QtMocHelpers::MethodData<void()>(18, 6, QMC::AccessPublic, QMetaType::Void),
        // Method 'setDoNotDisturb'
        QtMocHelpers::MethodData<void(bool)>(19, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 20 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'unreadCount'
        QtMocHelpers::PropertyData<int>(21, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'doNotDisturb'
        QtMocHelpers::PropertyData<bool>(22, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 1),
    };
    QtMocHelpers::UintData qt_enums {
    };
    QtMocHelpers::UintData qt_constructors {};
    QtMocHelpers::ClassInfos qt_classinfo({
            {    1,    2 },
            {    3,    4 },
    });
    return QtMocHelpers::metaObjectData<NotificationsModel, void>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject NotificationsModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractListModel::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18NotificationsModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18NotificationsModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN18NotificationsModelE_t>.metaTypes,
    nullptr
} };

void NotificationsModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<NotificationsModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->unreadCountChanged(); break;
        case 1: _t->doNotDisturbChanged(); break;
        case 2: _t->popupRequested((*reinterpret_cast<std::add_pointer_t<uint>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[4]))); break;
        case 3: _t->onNotificationAdded((*reinterpret_cast<std::add_pointer_t<uint>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[5])),(*reinterpret_cast<std::add_pointer_t<uchar>>(_a[6]))); break;
        case 4: _t->onNotificationRemoved((*reinterpret_cast<std::add_pointer_t<uint>>(_a[1]))); break;
        case 5: _t->dismiss((*reinterpret_cast<std::add_pointer_t<uint>>(_a[1]))); break;
        case 6: _t->clearAll(); break;
        case 7: _t->setDoNotDisturb((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (NotificationsModel::*)()>(_a, &NotificationsModel::unreadCountChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (NotificationsModel::*)()>(_a, &NotificationsModel::doNotDisturbChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (NotificationsModel::*)(uint , const QString & , const QString & , const QString & )>(_a, &NotificationsModel::popupRequested, 2))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->unreadCount(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->doNotDisturb(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 1: _t->setDoNotDisturb(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *NotificationsModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NotificationsModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18NotificationsModelE_t>.strings))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int NotificationsModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
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
void NotificationsModel::unreadCountChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void NotificationsModel::doNotDisturbChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void NotificationsModel::popupRequested(uint _t1, const QString & _t2, const QString & _t3, const QString & _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2, _t3, _t4);
}
QT_WARNING_POP
