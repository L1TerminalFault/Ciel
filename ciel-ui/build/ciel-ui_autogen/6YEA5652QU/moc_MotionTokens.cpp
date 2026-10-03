/****************************************************************************
** Meta object code from reading C++ file 'MotionTokens.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/MotionTokens.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MotionTokens.hpp' doesn't include <QObject>."
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
struct qt_meta_tag_ZN16CielMotionTokensE_t {};
} // unnamed namespace

template <> constexpr inline auto CielMotionTokens::qt_create_metaobjectdata<qt_meta_tag_ZN16CielMotionTokensE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "CielMotionTokens",
        "QML.Element",
        "auto",
        "QML.Singleton",
        "true",
        "responsiveDamping",
        "responsiveEpsilon",
        "responsiveMass",
        "spatialDamping",
        "spatialEpsilon",
        "spatialMass"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'responsiveDamping'
        QtMocHelpers::PropertyData<qreal>(5, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'responsiveEpsilon'
        QtMocHelpers::PropertyData<qreal>(6, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'responsiveMass'
        QtMocHelpers::PropertyData<qreal>(7, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'spatialDamping'
        QtMocHelpers::PropertyData<qreal>(8, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'spatialEpsilon'
        QtMocHelpers::PropertyData<qreal>(9, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'spatialMass'
        QtMocHelpers::PropertyData<qreal>(10, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Constant),
    };
    QtMocHelpers::UintData qt_enums {
    };
    QtMocHelpers::UintData qt_constructors {};
    QtMocHelpers::ClassInfos qt_classinfo({
            {    1,    2 },
            {    3,    4 },
    });
    return QtMocHelpers::metaObjectData<CielMotionTokens, void>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject CielMotionTokens::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16CielMotionTokensE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16CielMotionTokensE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN16CielMotionTokensE_t>.metaTypes,
    nullptr
} };

void CielMotionTokens::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<CielMotionTokens *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<qreal*>(_v) = _t->responsiveDamping(); break;
        case 1: *reinterpret_cast<qreal*>(_v) = _t->responsiveEpsilon(); break;
        case 2: *reinterpret_cast<qreal*>(_v) = _t->responsiveMass(); break;
        case 3: *reinterpret_cast<qreal*>(_v) = _t->spatialDamping(); break;
        case 4: *reinterpret_cast<qreal*>(_v) = _t->spatialEpsilon(); break;
        case 5: *reinterpret_cast<qreal*>(_v) = _t->spatialMass(); break;
        default: break;
        }
    }
}

const QMetaObject *CielMotionTokens::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CielMotionTokens::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16CielMotionTokensE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CielMotionTokens::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
