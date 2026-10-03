/****************************************************************************
** Meta object code from reading C++ file 'ProfileManager.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/core/ProfileManager.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ProfileManager.hpp' doesn't include <QObject>."
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
struct qt_meta_tag_ZN14ProfileManagerE_t {};
} // unnamed namespace

template <> constexpr inline auto ProfileManager::qt_create_metaobjectdata<qt_meta_tag_ZN14ProfileManagerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ProfileManager",
        "QML.Element",
        "auto",
        "QML.Singleton",
        "true",
        "activeProfileChanged",
        "",
        "visitRecorded",
        "url",
        "title",
        "workspaceId",
        "bookmarksChanged",
        "switchProfile",
        "profileId",
        "recordVisit",
        "fetchHistory",
        "QVariantList",
        "search",
        "deleteHistoryItem",
        "id",
        "clearHistory",
        "rangeIndex",
        "isBookmarked",
        "toggleBookmark",
        "activeProfileId",
        "currentSessionId",
        "profilePath"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'activeProfileChanged'
        QtMocHelpers::SignalData<void()>(5, 6, QMC::AccessPublic, QMetaType::Void),
        // Signal 'visitRecorded'
        QtMocHelpers::SignalData<void(const QString &, const QString &, const QString &)>(7, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 8 }, { QMetaType::QString, 9 }, { QMetaType::QString, 10 },
        }}),
        // Signal 'bookmarksChanged'
        QtMocHelpers::SignalData<void()>(11, 6, QMC::AccessPublic, QMetaType::Void),
        // Method 'switchProfile'
        QtMocHelpers::MethodData<void(const QString &)>(12, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 13 },
        }}),
        // Method 'recordVisit'
        QtMocHelpers::MethodData<void(const QString &, const QString &, const QString &)>(14, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 8 }, { QMetaType::QString, 9 }, { QMetaType::QString, 10 },
        }}),
        // Method 'fetchHistory'
        QtMocHelpers::MethodData<QVariantList(const QString &)>(15, 6, QMC::AccessPublic, 0x80000000 | 16, {{
            { QMetaType::QString, 17 },
        }}),
        // Method 'fetchHistory'
        QtMocHelpers::MethodData<QVariantList()>(15, 6, QMC::AccessPublic | QMC::MethodCloned, 0x80000000 | 16),
        // Method 'deleteHistoryItem'
        QtMocHelpers::MethodData<void(qint64)>(18, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::LongLong, 19 },
        }}),
        // Method 'clearHistory'
        QtMocHelpers::MethodData<void(int)>(20, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 21 },
        }}),
        // Method 'isBookmarked'
        QtMocHelpers::MethodData<bool(const QString &)>(22, 6, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 8 },
        }}),
        // Method 'toggleBookmark'
        QtMocHelpers::MethodData<bool(const QString &, const QString &)>(23, 6, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 8 }, { QMetaType::QString, 9 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'activeProfileId'
        QtMocHelpers::PropertyData<QString>(24, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'currentSessionId'
        QtMocHelpers::PropertyData<QString>(25, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'profilePath'
        QtMocHelpers::PropertyData<QString>(26, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
    };
    QtMocHelpers::UintData qt_enums {
    };
    QtMocHelpers::UintData qt_constructors {};
    QtMocHelpers::ClassInfos qt_classinfo({
            {    1,    2 },
            {    3,    4 },
    });
    return QtMocHelpers::metaObjectData<ProfileManager, void>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject ProfileManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ProfileManagerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ProfileManagerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14ProfileManagerE_t>.metaTypes,
    nullptr
} };

void ProfileManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ProfileManager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->activeProfileChanged(); break;
        case 1: _t->visitRecorded((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 2: _t->bookmarksChanged(); break;
        case 3: _t->switchProfile((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->recordVisit((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 5: { QVariantList _r = _t->fetchHistory((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 6: { QVariantList _r = _t->fetchHistory();
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 7: _t->deleteHistoryItem((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1]))); break;
        case 8: _t->clearHistory((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 9: { bool _r = _t->isBookmarked((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 10: { bool _r = _t->toggleBookmark((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ProfileManager::*)()>(_a, &ProfileManager::activeProfileChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProfileManager::*)(const QString & , const QString & , const QString & )>(_a, &ProfileManager::visitRecorded, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProfileManager::*)()>(_a, &ProfileManager::bookmarksChanged, 2))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->activeProfileId(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->currentSessionId(); break;
        case 2: *reinterpret_cast<QString*>(_v) = _t->profilePath(); break;
        default: break;
        }
    }
}

const QMetaObject *ProfileManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProfileManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ProfileManagerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ProfileManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
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
void ProfileManager::activeProfileChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ProfileManager::visitRecorded(const QString & _t1, const QString & _t2, const QString & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2, _t3);
}

// SIGNAL 2
void ProfileManager::bookmarksChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
