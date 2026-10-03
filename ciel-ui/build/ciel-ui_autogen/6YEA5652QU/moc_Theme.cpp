/****************************************************************************
** Meta object code from reading C++ file 'Theme.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/Theme.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Theme.hpp' doesn't include <QObject>."
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
struct qt_meta_tag_ZN5ThemeE_t {};
} // unnamed namespace

template <> constexpr inline auto Theme::qt_create_metaobjectdata<qt_meta_tag_ZN5ThemeE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Theme",
        "QML.Element",
        "auto",
        "QML.Singleton",
        "true",
        "themeChanged",
        "",
        "onThemeChangedSignal",
        "darkMode",
        "accentColor",
        "reducedMotion",
        "toggleMode",
        "setAccent",
        "hexColor",
        "metrics",
        "ThemeMetrics*",
        "background",
        "QColor",
        "transparent",
        "surface",
        "surfaceHover",
        "surfacePressed",
        "textPrimary",
        "textSecondary",
        "accent",
        "accentHover",
        "accentPressed",
        "border",
        "isDark",
        "transitionMs",
        "IconSize",
        "XXSMALL",
        "XSMALL",
        "SMALL",
        "MEDIUM",
        "LARGE",
        "XLARGE"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'themeChanged'
        QtMocHelpers::SignalData<void()>(5, 6, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onThemeChangedSignal'
        QtMocHelpers::SlotData<void(bool, const QString &, bool)>(7, 6, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 8 }, { QMetaType::QString, 9 }, { QMetaType::Bool, 10 },
        }}),
        // Method 'toggleMode'
        QtMocHelpers::MethodData<void()>(11, 6, QMC::AccessPublic, QMetaType::Void),
        // Method 'setAccent'
        QtMocHelpers::MethodData<void(const QString &)>(12, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 13 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'metrics'
        QtMocHelpers::PropertyData<ThemeMetrics*>(14, 0x80000000 | 15, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'background'
        QtMocHelpers::PropertyData<QColor>(16, 0x80000000 | 17, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'transparent'
        QtMocHelpers::PropertyData<QColor>(18, 0x80000000 | 17, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'surface'
        QtMocHelpers::PropertyData<QColor>(19, 0x80000000 | 17, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'surfaceHover'
        QtMocHelpers::PropertyData<QColor>(20, 0x80000000 | 17, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'surfacePressed'
        QtMocHelpers::PropertyData<QColor>(21, 0x80000000 | 17, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'textPrimary'
        QtMocHelpers::PropertyData<QColor>(22, 0x80000000 | 17, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'textSecondary'
        QtMocHelpers::PropertyData<QColor>(23, 0x80000000 | 17, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'accent'
        QtMocHelpers::PropertyData<QColor>(24, 0x80000000 | 17, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'accentHover'
        QtMocHelpers::PropertyData<QColor>(25, 0x80000000 | 17, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'accentPressed'
        QtMocHelpers::PropertyData<QColor>(26, 0x80000000 | 17, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'border'
        QtMocHelpers::PropertyData<QColor>(27, 0x80000000 | 17, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'isDark'
        QtMocHelpers::PropertyData<bool>(28, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'transitionMs'
        QtMocHelpers::PropertyData<int>(29, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'IconSize'
        QtMocHelpers::EnumData<enum IconSize>(30, 30, QMC::EnumIsScoped).add({
            {   31, IconSize::XXSMALL },
            {   32, IconSize::XSMALL },
            {   33, IconSize::SMALL },
            {   34, IconSize::MEDIUM },
            {   35, IconSize::LARGE },
            {   36, IconSize::XLARGE },
        }),
    };
    QtMocHelpers::UintData qt_constructors {};
    QtMocHelpers::ClassInfos qt_classinfo({
            {    1,    2 },
            {    3,    4 },
    });
    return QtMocHelpers::metaObjectData<Theme, void>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject Theme::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN5ThemeE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN5ThemeE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN5ThemeE_t>.metaTypes,
    nullptr
} };

void Theme::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Theme *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->themeChanged(); break;
        case 1: _t->onThemeChangedSignal((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[3]))); break;
        case 2: _t->toggleMode(); break;
        case 3: _t->setAccent((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Theme::*)()>(_a, &Theme::themeChanged, 0))
            return;
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< ThemeMetrics* >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<ThemeMetrics**>(_v) = _t->metrics(); break;
        case 1: *reinterpret_cast<QColor*>(_v) = _t->background(); break;
        case 2: *reinterpret_cast<QColor*>(_v) = _t->transparent(); break;
        case 3: *reinterpret_cast<QColor*>(_v) = _t->surface(); break;
        case 4: *reinterpret_cast<QColor*>(_v) = _t->surfaceHover(); break;
        case 5: *reinterpret_cast<QColor*>(_v) = _t->surfacePressed(); break;
        case 6: *reinterpret_cast<QColor*>(_v) = _t->textPrimary(); break;
        case 7: *reinterpret_cast<QColor*>(_v) = _t->textSecondary(); break;
        case 8: *reinterpret_cast<QColor*>(_v) = _t->accent(); break;
        case 9: *reinterpret_cast<QColor*>(_v) = _t->accentHover(); break;
        case 10: *reinterpret_cast<QColor*>(_v) = _t->accentPressed(); break;
        case 11: *reinterpret_cast<QColor*>(_v) = _t->border(); break;
        case 12: *reinterpret_cast<bool*>(_v) = _t->isDark(); break;
        case 13: *reinterpret_cast<int*>(_v) = _t->transitionMs(); break;
        default: break;
        }
    }
}

const QMetaObject *Theme::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Theme::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN5ThemeE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Theme::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void Theme::themeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
