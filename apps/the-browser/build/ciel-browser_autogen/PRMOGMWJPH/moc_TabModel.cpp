/****************************************************************************
** Meta object code from reading C++ file 'TabModel.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/core/TabModel.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TabModel.hpp' doesn't include <QObject>."
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
struct qt_meta_tag_ZN8TabModelE_t {};
} // unnamed namespace

template <> constexpr inline auto TabModel::qt_create_metaobjectdata<qt_meta_tag_ZN8TabModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "TabModel",
        "QML.Element",
        "auto",
        "currentIndexChanged",
        "",
        "countChanged",
        "workspaceIdChanged",
        "addTab",
        "QUrl",
        "url",
        "closeTab",
        "index",
        "updateTitle",
        "title",
        "updateUrl",
        "updateLoading",
        "loading",
        "updateProgress",
        "progress",
        "updateNavigation",
        "canGoBack",
        "canGoForward",
        "moveTab",
        "from",
        "to",
        "togglePin",
        "setPinned",
        "pinned",
        "currentIndex",
        "count",
        "workspaceId"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'currentIndexChanged'
        QtMocHelpers::SignalData<void()>(3, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'countChanged'
        QtMocHelpers::SignalData<void()>(5, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'workspaceIdChanged'
        QtMocHelpers::SignalData<void()>(6, 4, QMC::AccessPublic, QMetaType::Void),
        // Method 'addTab'
        QtMocHelpers::MethodData<void(const QUrl &)>(7, 4, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 9 },
        }}),
        // Method 'addTab'
        QtMocHelpers::MethodData<void()>(7, 4, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void),
        // Method 'closeTab'
        QtMocHelpers::MethodData<void(int)>(10, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 },
        }}),
        // Method 'updateTitle'
        QtMocHelpers::MethodData<void(int, const QString &)>(12, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 }, { QMetaType::QString, 13 },
        }}),
        // Method 'updateUrl'
        QtMocHelpers::MethodData<void(int, const QUrl &)>(14, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 }, { 0x80000000 | 8, 9 },
        }}),
        // Method 'updateLoading'
        QtMocHelpers::MethodData<void(int, bool)>(15, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 }, { QMetaType::Bool, 16 },
        }}),
        // Method 'updateProgress'
        QtMocHelpers::MethodData<void(int, int)>(17, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 }, { QMetaType::Int, 18 },
        }}),
        // Method 'updateNavigation'
        QtMocHelpers::MethodData<void(int, bool, bool)>(19, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 }, { QMetaType::Bool, 20 }, { QMetaType::Bool, 21 },
        }}),
        // Method 'moveTab'
        QtMocHelpers::MethodData<void(int, int)>(22, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 23 }, { QMetaType::Int, 24 },
        }}),
        // Method 'togglePin'
        QtMocHelpers::MethodData<void(int)>(25, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 },
        }}),
        // Method 'setPinned'
        QtMocHelpers::MethodData<void(int, bool)>(26, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 }, { QMetaType::Bool, 27 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'currentIndex'
        QtMocHelpers::PropertyData<int>(28, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'count'
        QtMocHelpers::PropertyData<int>(29, QMetaType::Int, QMC::DefaultPropertyFlags, 1),
        // property 'workspaceId'
        QtMocHelpers::PropertyData<QString>(30, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 2),
    };
    QtMocHelpers::UintData qt_enums {
    };
    QtMocHelpers::UintData qt_constructors {};
    QtMocHelpers::ClassInfos qt_classinfo({
            {    1,    2 },
    });
    return QtMocHelpers::metaObjectData<TabModel, void>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject TabModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractListModel::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8TabModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8TabModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN8TabModelE_t>.metaTypes,
    nullptr
} };

void TabModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<TabModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->currentIndexChanged(); break;
        case 1: _t->countChanged(); break;
        case 2: _t->workspaceIdChanged(); break;
        case 3: _t->addTab((*reinterpret_cast<std::add_pointer_t<QUrl>>(_a[1]))); break;
        case 4: _t->addTab(); break;
        case 5: _t->closeTab((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->updateTitle((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 7: _t->updateUrl((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QUrl>>(_a[2]))); break;
        case 8: _t->updateLoading((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 9: _t->updateProgress((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 10: _t->updateNavigation((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[3]))); break;
        case 11: _t->moveTab((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 12: _t->togglePin((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 13: _t->setPinned((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (TabModel::*)()>(_a, &TabModel::currentIndexChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (TabModel::*)()>(_a, &TabModel::countChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (TabModel::*)()>(_a, &TabModel::workspaceIdChanged, 2))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->currentIndex(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->count(); break;
        case 2: *reinterpret_cast<QString*>(_v) = _t->workspaceId(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setCurrentIndex(*reinterpret_cast<int*>(_v)); break;
        case 2: _t->setWorkspaceId(*reinterpret_cast<QString*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *TabModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TabModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8TabModelE_t>.strings))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int TabModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
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
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void TabModel::currentIndexChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TabModel::countChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TabModel::workspaceIdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
