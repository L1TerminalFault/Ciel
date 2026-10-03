/****************************************************************************
** Meta object code from reading C++ file 'BrowserConfig.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/core/BrowserConfig.hpp"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BrowserConfig.hpp' doesn't include <QObject>."
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
struct qt_meta_tag_ZN14DownloadsModelE_t {};
} // unnamed namespace

template <> constexpr inline auto DownloadsModel::qt_create_metaobjectdata<qt_meta_tag_ZN14DownloadsModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "DownloadsModel",
        "QML.Element",
        "auto",
        "countChanged",
        "",
        "count"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'countChanged'
        QtMocHelpers::SignalData<void()>(3, 4, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'count'
        QtMocHelpers::PropertyData<int>(5, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
    };
    QtMocHelpers::UintData qt_enums {
    };
    QtMocHelpers::UintData qt_constructors {};
    QtMocHelpers::ClassInfos qt_classinfo({
            {    1,    2 },
    });
    return QtMocHelpers::metaObjectData<DownloadsModel, void>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject DownloadsModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractListModel::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14DownloadsModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14DownloadsModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14DownloadsModelE_t>.metaTypes,
    nullptr
} };

void DownloadsModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<DownloadsModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->countChanged(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (DownloadsModel::*)()>(_a, &DownloadsModel::countChanged, 0))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->count(); break;
        default: break;
        }
    }
}

const QMetaObject *DownloadsModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DownloadsModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14DownloadsModelE_t>.strings))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int DownloadsModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void DownloadsModel::countChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
namespace {
struct qt_meta_tag_ZN13BrowserConfigE_t {};
} // unnamed namespace

template <> constexpr inline auto BrowserConfig::qt_create_metaobjectdata<qt_meta_tag_ZN13BrowserConfigE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "BrowserConfig",
        "QML.Element",
        "auto",
        "newTabUrlChanged",
        "",
        "homeUrlChanged",
        "searchTemplateChanged",
        "onDownloadStarted",
        "id",
        "filename",
        "destination",
        "total_bytes",
        "category",
        "onDownloadProgress",
        "bytes_received",
        "speed_bytes_sec",
        "QList<double>",
        "segments",
        "onDownloadFinished",
        "onDownloadFailed",
        "error_message",
        "resolveQueryOrUrl",
        "QUrl",
        "input",
        "startDownload",
        "url",
        "mimeType",
        "pauseDownload",
        "resumeDownload",
        "cancelDownload",
        "retryDownload",
        "openFolder",
        "path",
        "clearFinished",
        "newTabUrl",
        "homeUrl",
        "searchTemplate",
        "downloadsModel",
        "DownloadsModel*"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'newTabUrlChanged'
        QtMocHelpers::SignalData<void()>(3, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'homeUrlChanged'
        QtMocHelpers::SignalData<void()>(5, 4, QMC::AccessPublic, QMetaType::Void),
        // Signal 'searchTemplateChanged'
        QtMocHelpers::SignalData<void()>(6, 4, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onDownloadStarted'
        QtMocHelpers::SlotData<void(const QString &, const QString &, const QString &, qint64, const QString &)>(7, 4, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 8 }, { QMetaType::QString, 9 }, { QMetaType::QString, 10 }, { QMetaType::LongLong, 11 },
            { QMetaType::QString, 12 },
        }}),
        // Slot 'onDownloadProgress'
        QtMocHelpers::SlotData<void(const QString &, qint64, qint64, double, const QList<double> &)>(13, 4, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 8 }, { QMetaType::LongLong, 14 }, { QMetaType::LongLong, 11 }, { QMetaType::Double, 15 },
            { 0x80000000 | 16, 17 },
        }}),
        // Slot 'onDownloadFinished'
        QtMocHelpers::SlotData<void(const QString &, const QString &, const QString &)>(18, 4, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 8 }, { QMetaType::QString, 10 }, { QMetaType::QString, 12 },
        }}),
        // Slot 'onDownloadFailed'
        QtMocHelpers::SlotData<void(const QString &, const QString &)>(19, 4, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 8 }, { QMetaType::QString, 20 },
        }}),
        // Method 'resolveQueryOrUrl'
        QtMocHelpers::MethodData<QUrl(const QString &) const>(21, 4, QMC::AccessPublic, 0x80000000 | 22, {{
            { QMetaType::QString, 23 },
        }}),
        // Method 'startDownload'
        QtMocHelpers::MethodData<void(const QUrl &, const QString &, const QString &)>(24, 4, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 22, 25 }, { QMetaType::QString, 9 }, { QMetaType::QString, 26 },
        }}),
        // Method 'startDownload'
        QtMocHelpers::MethodData<void(const QUrl &, const QString &)>(24, 4, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { 0x80000000 | 22, 25 }, { QMetaType::QString, 9 },
        }}),
        // Method 'startDownload'
        QtMocHelpers::MethodData<void(const QUrl &)>(24, 4, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { 0x80000000 | 22, 25 },
        }}),
        // Method 'pauseDownload'
        QtMocHelpers::MethodData<void(const QString &)>(27, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 8 },
        }}),
        // Method 'resumeDownload'
        QtMocHelpers::MethodData<void(const QString &)>(28, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 8 },
        }}),
        // Method 'cancelDownload'
        QtMocHelpers::MethodData<void(const QString &)>(29, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 8 },
        }}),
        // Method 'retryDownload'
        QtMocHelpers::MethodData<void(const QString &)>(30, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 8 },
        }}),
        // Method 'openFolder'
        QtMocHelpers::MethodData<void(const QString &)>(31, 4, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 32 },
        }}),
        // Method 'clearFinished'
        QtMocHelpers::MethodData<void()>(33, 4, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'newTabUrl'
        QtMocHelpers::PropertyData<QUrl>(34, 0x80000000 | 22, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet, 0),
        // property 'homeUrl'
        QtMocHelpers::PropertyData<QUrl>(35, 0x80000000 | 22, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet, 1),
        // property 'searchTemplate'
        QtMocHelpers::PropertyData<QString>(36, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 2),
        // property 'downloadsModel'
        QtMocHelpers::PropertyData<DownloadsModel*>(37, 0x80000000 | 38, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
    };
    QtMocHelpers::UintData qt_enums {
    };
    QtMocHelpers::UintData qt_constructors {};
    QtMocHelpers::ClassInfos qt_classinfo({
            {    1,    2 },
    });
    return QtMocHelpers::metaObjectData<BrowserConfig, void>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject BrowserConfig::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13BrowserConfigE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13BrowserConfigE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13BrowserConfigE_t>.metaTypes,
    nullptr
} };

void BrowserConfig::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<BrowserConfig *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->newTabUrlChanged(); break;
        case 1: _t->homeUrlChanged(); break;
        case 2: _t->searchTemplateChanged(); break;
        case 3: _t->onDownloadStarted((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<qint64>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[5]))); break;
        case 4: _t->onDownloadProgress((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qint64>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<qint64>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<QList<double>>>(_a[5]))); break;
        case 5: _t->onDownloadFinished((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 6: _t->onDownloadFailed((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 7: { QUrl _r = _t->resolveQueryOrUrl((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QUrl*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->startDownload((*reinterpret_cast<std::add_pointer_t<QUrl>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 9: _t->startDownload((*reinterpret_cast<std::add_pointer_t<QUrl>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 10: _t->startDownload((*reinterpret_cast<std::add_pointer_t<QUrl>>(_a[1]))); break;
        case 11: _t->pauseDownload((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 12: _t->resumeDownload((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 13: _t->cancelDownload((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 14: _t->retryDownload((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 15: _t->openFolder((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 16: _t->clearFinished(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 4:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<double> >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (BrowserConfig::*)()>(_a, &BrowserConfig::newTabUrlChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (BrowserConfig::*)()>(_a, &BrowserConfig::homeUrlChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (BrowserConfig::*)()>(_a, &BrowserConfig::searchTemplateChanged, 2))
            return;
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< DownloadsModel* >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QUrl*>(_v) = _t->newTabUrl(); break;
        case 1: *reinterpret_cast<QUrl*>(_v) = _t->homeUrl(); break;
        case 2: *reinterpret_cast<QString*>(_v) = _t->searchTemplate(); break;
        case 3: *reinterpret_cast<DownloadsModel**>(_v) = _t->downloadsModel(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setNewTabUrl(*reinterpret_cast<QUrl*>(_v)); break;
        case 1: _t->setHomeUrl(*reinterpret_cast<QUrl*>(_v)); break;
        case 2: _t->setSearchTemplate(*reinterpret_cast<QString*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *BrowserConfig::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BrowserConfig::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13BrowserConfigE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int BrowserConfig::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void BrowserConfig::newTabUrlChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void BrowserConfig::homeUrlChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void BrowserConfig::searchTemplateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
