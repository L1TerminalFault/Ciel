/****************************************************************************
** Meta object code from reading C++ file 'NotificationsAdaptor.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "NotificationsAdaptor.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NotificationsAdaptor.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN20NotificationsAdaptorE_t {};
} // unnamed namespace

template <> constexpr inline auto NotificationsAdaptor::qt_create_metaobjectdata<qt_meta_tag_ZN20NotificationsAdaptorE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "NotificationsAdaptor",
        "D-Bus Interface",
        "org.freedesktop.Notifications",
        "D-Bus Introspection",
        "  <interface name=\"org.freedesktop.Notifications\">\n    <method "
        "name=\"Notify\">\n      <annotation value=\"QVariantMap\" name=\"o"
        "rg.qtproject.QtDBus.QtTypeName.In6\"/>\n      <arg direction=\"in\""
        " type=\"s\" name=\"app_name\"/>\n      <arg direction=\"in\" type="
        "\"u\" name=\"replaces_id\"/>\n      <arg direction=\"in\" type=\"s"
        "\" name=\"app_icon\"/>\n      <arg direction=\"in\" type=\"s\" nam"
        "e=\"summary\"/>\n      <arg direction=\"in\" type=\"s\" name=\"bod"
        "y\"/>\n      <arg direction=\"in\" type=\"as\" name=\"actions\"/>\n"
        "      <arg direction=\"in\" type=\"a{sv}\" name=\"hints\"/>\n     "
        " <arg direction=\"in\" type=\"i\" name=\"expire_timeout\"/>\n     "
        " <arg direction=\"out\" type=\"u\" name=\"id\"/>\n    </method>\n "
        "   <method name=\"CloseNotification\">\n      <arg direction=\"in\""
        " type=\"u\" name=\"id\"/>\n    </method>\n    <method name=\"GetCa"
        "pabilities\">\n      <arg direction=\"out\" type=\"as\" name=\"cap"
        "abilities\"/>\n    </method>\n    <method name=\"GetServerInformat"
        "ion\">\n      <arg direction=\"out\" type=\"s\" name=\"name\"/>\n "
        "     <arg direction=\"out\" type=\"s\" name=\"vendor\"/>\n      <a"
        "rg direction=\"out\" type=\"s\" name=\"version\"/>\n      <arg dir"
        "ection=\"out\" type=\"s\" name=\"spec_version\"/>\n    </method>\n"
        "    <signal name=\"NotificationClosed\">\n      <arg type=\"u\" na"
        "me=\"id\"/>\n      <arg type=\"u\" name=\"reason\"/>\n    </signal"
        ">\n    <signal name=\"ActionInvoked\">\n      <arg type=\"u\" name"
        "=\"id\"/>\n      <arg type=\"s\" name=\"action_key\"/>\n    </sign"
        "al>\n  </interface>\n",
        "ActionInvoked",
        "",
        "id",
        "action_key",
        "NotificationClosed",
        "reason",
        "CloseNotification",
        "GetCapabilities",
        "GetServerInformation",
        "QString&",
        "vendor",
        "version",
        "spec_version",
        "Notify",
        "app_name",
        "replaces_id",
        "app_icon",
        "summary",
        "body",
        "actions",
        "QVariantMap",
        "hints",
        "expire_timeout"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'ActionInvoked'
        QtMocHelpers::SignalData<void(uint, const QString &)>(5, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 7 }, { QMetaType::QString, 8 },
        }}),
        // Signal 'NotificationClosed'
        QtMocHelpers::SignalData<void(uint, uint)>(9, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 7 }, { QMetaType::UInt, 10 },
        }}),
        // Slot 'CloseNotification'
        QtMocHelpers::SlotData<void(uint)>(11, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 7 },
        }}),
        // Slot 'GetCapabilities'
        QtMocHelpers::SlotData<QStringList()>(12, 6, QMC::AccessPublic, QMetaType::QStringList),
        // Slot 'GetServerInformation'
        QtMocHelpers::SlotData<QString(QString &, QString &, QString &)>(13, 6, QMC::AccessPublic, QMetaType::QString, {{
            { 0x80000000 | 14, 15 }, { 0x80000000 | 14, 16 }, { 0x80000000 | 14, 17 },
        }}),
        // Slot 'Notify'
        QtMocHelpers::SlotData<uint(const QString &, uint, const QString &, const QString &, const QString &, const QStringList &, const QVariantMap &, int)>(18, 6, QMC::AccessPublic, QMetaType::UInt, {{
            { QMetaType::QString, 19 }, { QMetaType::UInt, 20 }, { QMetaType::QString, 21 }, { QMetaType::QString, 22 },
            { QMetaType::QString, 23 }, { QMetaType::QStringList, 24 }, { 0x80000000 | 25, 26 }, { QMetaType::Int, 27 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    QtMocHelpers::UintData qt_constructors {};
    QtMocHelpers::ClassInfos qt_classinfo({
            {    1,    2 },
            {    3,    4 },
    });
    return QtMocHelpers::metaObjectData<NotificationsAdaptor, qt_meta_tag_ZN20NotificationsAdaptorE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject NotificationsAdaptor::staticMetaObject = { {
    QMetaObject::SuperData::link<QDBusAbstractAdaptor::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20NotificationsAdaptorE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20NotificationsAdaptorE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN20NotificationsAdaptorE_t>.metaTypes,
    nullptr
} };

void NotificationsAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<NotificationsAdaptor *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->ActionInvoked((*reinterpret_cast<std::add_pointer_t<uint>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 1: _t->NotificationClosed((*reinterpret_cast<std::add_pointer_t<uint>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<uint>>(_a[2]))); break;
        case 2: _t->CloseNotification((*reinterpret_cast<std::add_pointer_t<uint>>(_a[1]))); break;
        case 3: { QStringList _r = _t->GetCapabilities();
            if (_a[0]) *reinterpret_cast<QStringList*>(_a[0]) = std::move(_r); }  break;
        case 4: { QString _r = _t->GetServerInformation((*reinterpret_cast<std::add_pointer_t<QString&>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString&>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString&>>(_a[3])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 5: { uint _r = _t->Notify((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<uint>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[5])),(*reinterpret_cast<std::add_pointer_t<QStringList>>(_a[6])),(*reinterpret_cast<std::add_pointer_t<QVariantMap>>(_a[7])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[8])));
            if (_a[0]) *reinterpret_cast<uint*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (NotificationsAdaptor::*)(uint , const QString & )>(_a, &NotificationsAdaptor::ActionInvoked, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (NotificationsAdaptor::*)(uint , uint )>(_a, &NotificationsAdaptor::NotificationClosed, 1))
            return;
    }
}

const QMetaObject *NotificationsAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NotificationsAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20NotificationsAdaptorE_t>.strings))
        return static_cast<void*>(this);
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int NotificationsAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
    return _id;
}

// SIGNAL 0
void NotificationsAdaptor::ActionInvoked(uint _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}

// SIGNAL 1
void NotificationsAdaptor::NotificationClosed(uint _t1, uint _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}
QT_WARNING_POP
