/****************************************************************************
** Meta object code from reading C++ file 'ThemeMetrics.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/ThemeMetrics.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ThemeMetrics.hpp' doesn't include <QObject>."
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
struct qt_meta_tag_ZN12ThemeMetricsE_t {};
} // unnamed namespace

template <> constexpr inline auto ThemeMetrics::qt_create_metaobjectdata<qt_meta_tag_ZN12ThemeMetricsE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ThemeMetrics",
        "QML.Element",
        "auto",
        "QML.Creatable",
        "false",
        "QML.UncreatableReason",
        "ThemeMetrics is accessed via Theme.metrics",
        "spacingXs",
        "spacingSm",
        "spacingMd",
        "spacingLg",
        "spacingXl",
        "radiusSm",
        "radiusMd",
        "radiusLg",
        "squircleExponent"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'spacingXs'
        QtMocHelpers::PropertyData<int>(7, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'spacingSm'
        QtMocHelpers::PropertyData<int>(8, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'spacingMd'
        QtMocHelpers::PropertyData<int>(9, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'spacingLg'
        QtMocHelpers::PropertyData<int>(10, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'spacingXl'
        QtMocHelpers::PropertyData<int>(11, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'radiusSm'
        QtMocHelpers::PropertyData<qreal>(12, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'radiusMd'
        QtMocHelpers::PropertyData<qreal>(13, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'radiusLg'
        QtMocHelpers::PropertyData<qreal>(14, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'squircleExponent'
        QtMocHelpers::PropertyData<qreal>(15, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Constant),
    };
    QtMocHelpers::UintData qt_enums {
    };
    QtMocHelpers::UintData qt_constructors {};
    QtMocHelpers::ClassInfos qt_classinfo({
            {    1,    2 },
            {    3,    4 },
            {    5,    6 },
    });
    return QtMocHelpers::metaObjectData<ThemeMetrics, void>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject ThemeMetrics::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ThemeMetricsE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ThemeMetricsE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12ThemeMetricsE_t>.metaTypes,
    nullptr
} };

void ThemeMetrics::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ThemeMetrics *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->spacingXs(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->spacingSm(); break;
        case 2: *reinterpret_cast<int*>(_v) = _t->spacingMd(); break;
        case 3: *reinterpret_cast<int*>(_v) = _t->spacingLg(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->spacingXl(); break;
        case 5: *reinterpret_cast<qreal*>(_v) = _t->radiusSm(); break;
        case 6: *reinterpret_cast<qreal*>(_v) = _t->radiusMd(); break;
        case 7: *reinterpret_cast<qreal*>(_v) = _t->radiusLg(); break;
        case 8: *reinterpret_cast<qreal*>(_v) = _t->squircleExponent(); break;
        default: break;
        }
    }
}

const QMetaObject *ThemeMetrics::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ThemeMetrics::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ThemeMetricsE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ThemeMetrics::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
