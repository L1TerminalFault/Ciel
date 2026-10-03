/****************************************************************************
** Meta object code from reading C++ file 'cielgraphview.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/cielgraphview.hpp"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cielgraphview.hpp' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13CielGraphViewE_t {};
} // unnamed namespace

template <> constexpr inline auto CielGraphView::qt_create_metaobjectdata<qt_meta_tag_ZN13CielGraphViewE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "CielGraphView",
        "QML.Element",
        "auto",
        "valuesChanged",
        "",
        "minValueChanged",
        "maxValueChanged",
        "strokeColorChanged",
        "strokeWidthChanged",
        "fillOpacityTopChanged",
        "smoothScrollChanged",
        "scrollProgressChanged",
        "values",
        "QList<qreal>",
        "minValue",
        "maxValue",
        "strokeColor",
        "QColor",
        "strokeWidth",
        "fillOpacityTop",
        "smoothScroll",
        "scrollProgress"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'valuesChanged'
        QtMocHelpers::SignalData<void()>(3, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'minValueChanged'
        QtMocHelpers::SignalData<void()>(5, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'maxValueChanged'
        QtMocHelpers::SignalData<void()>(6, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'strokeColorChanged'
        QtMocHelpers::SignalData<void()>(7, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'strokeWidthChanged'
        QtMocHelpers::SignalData<void()>(8, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'fillOpacityTopChanged'
        QtMocHelpers::SignalData<void()>(9, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'smoothScrollChanged'
        QtMocHelpers::SignalData<void()>(10, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'scrollProgressChanged'
        QtMocHelpers::SignalData<void()>(11, 4, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'values'
        QtMocHelpers::PropertyData<QList<qreal>>(12, 0x80000000 | 13, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet, 0),
        // property 'minValue'
        QtMocHelpers::PropertyData<qreal>(14, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 1),
        // property 'maxValue'
        QtMocHelpers::PropertyData<qreal>(15, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 2),
        // property 'strokeColor'
        QtMocHelpers::PropertyData<QColor>(16, 0x80000000 | 17, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet, 3),
        // property 'strokeWidth'
        QtMocHelpers::PropertyData<qreal>(18, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 4),
        // property 'fillOpacityTop'
        QtMocHelpers::PropertyData<qreal>(19, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 5),
        // property 'smoothScroll'
        QtMocHelpers::PropertyData<bool>(20, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 6),
        // property 'scrollProgress'
        QtMocHelpers::PropertyData<qreal>(21, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 7),
    };
    QtMocHelpers::UintData qt_enums {
    };
    QtMocHelpers::UintData qt_constructors {};
    QtMocHelpers::ClassInfos qt_classinfo({
            {    1,    2 },
    });
    return QtMocHelpers::metaObjectData<CielGraphView, void>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject CielGraphView::staticMetaObject = { {
    QMetaObject::SuperData::link<QQuickItem::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13CielGraphViewE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13CielGraphViewE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13CielGraphViewE_t>.metaTypes,
    nullptr
} };

void CielGraphView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<CielGraphView *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->valuesChanged(); break;
        case 1: _t->minValueChanged(); break;
        case 2: _t->maxValueChanged(); break;
        case 3: _t->strokeColorChanged(); break;
        case 4: _t->strokeWidthChanged(); break;
        case 5: _t->fillOpacityTopChanged(); break;
        case 6: _t->smoothScrollChanged(); break;
        case 7: _t->scrollProgressChanged(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (CielGraphView::*)()>(_a, &CielGraphView::valuesChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (CielGraphView::*)()>(_a, &CielGraphView::minValueChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (CielGraphView::*)()>(_a, &CielGraphView::maxValueChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (CielGraphView::*)()>(_a, &CielGraphView::strokeColorChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (CielGraphView::*)()>(_a, &CielGraphView::strokeWidthChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (CielGraphView::*)()>(_a, &CielGraphView::fillOpacityTopChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (CielGraphView::*)()>(_a, &CielGraphView::smoothScrollChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (CielGraphView::*)()>(_a, &CielGraphView::scrollProgressChanged, 7))
            return;
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<qreal> >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QList<qreal>*>(_v) = _t->values(); break;
        case 1: *reinterpret_cast<qreal*>(_v) = _t->minValue(); break;
        case 2: *reinterpret_cast<qreal*>(_v) = _t->maxValue(); break;
        case 3: *reinterpret_cast<QColor*>(_v) = _t->strokeColor(); break;
        case 4: *reinterpret_cast<qreal*>(_v) = _t->strokeWidth(); break;
        case 5: *reinterpret_cast<qreal*>(_v) = _t->fillOpacityTop(); break;
        case 6: *reinterpret_cast<bool*>(_v) = _t->smoothScroll(); break;
        case 7: *reinterpret_cast<qreal*>(_v) = _t->scrollProgress(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setValues(*reinterpret_cast<QList<qreal>*>(_v)); break;
        case 1: _t->setMinValue(*reinterpret_cast<qreal*>(_v)); break;
        case 2: _t->setMaxValue(*reinterpret_cast<qreal*>(_v)); break;
        case 3: _t->setStrokeColor(*reinterpret_cast<QColor*>(_v)); break;
        case 4: _t->setStrokeWidth(*reinterpret_cast<qreal*>(_v)); break;
        case 5: _t->setFillOpacityTop(*reinterpret_cast<qreal*>(_v)); break;
        case 6: _t->setSmoothScroll(*reinterpret_cast<bool*>(_v)); break;
        case 7: _t->setScrollProgress(*reinterpret_cast<qreal*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *CielGraphView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CielGraphView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13CielGraphViewE_t>.strings))
        return static_cast<void*>(this);
    return QQuickItem::qt_metacast(_clname);
}

int CielGraphView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QQuickItem::qt_metacall(_c, _id, _a);
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
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void CielGraphView::valuesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void CielGraphView::minValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void CielGraphView::maxValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void CielGraphView::strokeColorChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void CielGraphView::strokeWidthChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void CielGraphView::fillOpacityTopChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void CielGraphView::smoothScrollChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void CielGraphView::scrollProgressChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}
QT_WARNING_POP
