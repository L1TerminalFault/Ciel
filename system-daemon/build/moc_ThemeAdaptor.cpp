/****************************************************************************
** Meta object code from reading C++ file 'ThemeAdaptor.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ThemeAdaptor.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ThemeAdaptor.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN12ThemeAdaptorE_t {};
} // unnamed namespace

template <> constexpr inline auto ThemeAdaptor::qt_create_metaobjectdata<qt_meta_tag_ZN12ThemeAdaptorE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ThemeAdaptor",
        "D-Bus Interface",
        "org.ciel.Theme",
        "D-Bus Introspection",
        "  <interface name=\"org.ciel.Theme\">\n    <property access=\"read"
        "write\" type=\"b\" name=\"DarkMode\"/>\n    <property access=\"rea"
        "dwrite\" type=\"s\" name=\"AccentColor\"/>\n    <property access=\""
        "readwrite\" type=\"b\" name=\"ReducedMotion\"/>\n    <method name="
        "\"SetDarkMode\">\n      <arg direction=\"in\" type=\"b\" name=\"en"
        "abled\"/>\n    </method>\n    <method name=\"SetAccentColor\">\n  "
        "    <arg direction=\"in\" type=\"s\" name=\"hexColor\"/>\n    </me"
        "thod>\n    <method name=\"SetReducedMotion\">\n      <arg directio"
        "n=\"in\" type=\"b\" name=\"enabled\"/>\n    </method>\n    <signal"
        " name=\"ThemeChanged\">\n      <arg type=\"b\" name=\"darkMode\"/>"
        "\n      <arg type=\"s\" name=\"accentColor\"/>\n      <arg type=\""
        "b\" name=\"reducedMotion\"/>\n    </signal>\n  </interface>\n",
        "ThemeChanged",
        "",
        "darkMode",
        "accentColor",
        "reducedMotion",
        "SetAccentColor",
        "hexColor",
        "SetDarkMode",
        "enabled",
        "SetReducedMotion",
        "AccentColor",
        "DarkMode",
        "ReducedMotion"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'ThemeChanged'
        QtMocHelpers::SignalData<void(bool, const QString &, bool)>(5, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 7 }, { QMetaType::QString, 8 }, { QMetaType::Bool, 9 },
        }}),
        // Slot 'SetAccentColor'
        QtMocHelpers::SlotData<void(const QString &)>(10, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 11 },
        }}),
        // Slot 'SetDarkMode'
        QtMocHelpers::SlotData<void(bool)>(12, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 13 },
        }}),
        // Slot 'SetReducedMotion'
        QtMocHelpers::SlotData<void(bool)>(14, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 13 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'AccentColor'
        QtMocHelpers::PropertyData<QString>(15, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'DarkMode'
        QtMocHelpers::PropertyData<bool>(16, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'ReducedMotion'
        QtMocHelpers::PropertyData<bool>(17, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
    };
    QtMocHelpers::UintData qt_constructors {};
    QtMocHelpers::ClassInfos qt_classinfo({
            {    1,    2 },
            {    3,    4 },
    });
    return QtMocHelpers::metaObjectData<ThemeAdaptor, qt_meta_tag_ZN12ThemeAdaptorE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject ThemeAdaptor::staticMetaObject = { {
    QMetaObject::SuperData::link<QDBusAbstractAdaptor::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ThemeAdaptorE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ThemeAdaptorE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12ThemeAdaptorE_t>.metaTypes,
    nullptr
} };

void ThemeAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ThemeAdaptor *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->ThemeChanged((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[3]))); break;
        case 1: _t->SetAccentColor((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->SetDarkMode((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->SetReducedMotion((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ThemeAdaptor::*)(bool , const QString & , bool )>(_a, &ThemeAdaptor::ThemeChanged, 0))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->accentColor(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->darkMode(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->reducedMotion(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setAccentColor(*reinterpret_cast<QString*>(_v)); break;
        case 1: _t->setDarkMode(*reinterpret_cast<bool*>(_v)); break;
        case 2: _t->setReducedMotion(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *ThemeAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ThemeAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ThemeAdaptorE_t>.strings))
        return static_cast<void*>(this);
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int ThemeAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ThemeAdaptor::ThemeChanged(bool _t1, const QString & _t2, bool _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3);
}
QT_WARNING_POP
