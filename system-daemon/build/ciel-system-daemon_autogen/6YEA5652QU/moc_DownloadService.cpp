/****************************************************************************
** Meta object code from reading C++ file 'DownloadService.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/DownloadService.hpp"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DownloadService.hpp' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13SegmentWorkerE_t {};
} // unnamed namespace

template <> constexpr inline auto SegmentWorker::qt_create_metaobjectdata<qt_meta_tag_ZN13SegmentWorkerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "SegmentWorker",
        "progressUpdated",
        "",
        "finished",
        "index",
        "failed",
        "error"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'progressUpdated'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'finished'
        QtMocHelpers::SignalData<void(int)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 4 },
        }}),
        // Signal 'failed'
        QtMocHelpers::SignalData<void(int, const QString &)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 4 }, { QMetaType::QString, 6 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<SegmentWorker, qt_meta_tag_ZN13SegmentWorkerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject SegmentWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13SegmentWorkerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13SegmentWorkerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13SegmentWorkerE_t>.metaTypes,
    nullptr
} };

void SegmentWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SegmentWorker *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->progressUpdated(); break;
        case 1: _t->finished((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->failed((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (SegmentWorker::*)()>(_a, &SegmentWorker::progressUpdated, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (SegmentWorker::*)(int )>(_a, &SegmentWorker::finished, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (SegmentWorker::*)(int , const QString & )>(_a, &SegmentWorker::failed, 2))
            return;
    }
}

const QMetaObject *SegmentWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SegmentWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13SegmentWorkerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SegmentWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void SegmentWorker::progressUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SegmentWorker::finished(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void SegmentWorker::failed(int _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2);
}
namespace {
struct qt_meta_tag_ZN12DownloadTaskE_t {};
} // unnamed namespace

template <> constexpr inline auto DownloadTask::qt_create_metaobjectdata<qt_meta_tag_ZN12DownloadTaskE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "DownloadTask",
        "taskFinished",
        "",
        "id",
        "taskFailed",
        "error"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'taskFinished'
        QtMocHelpers::SignalData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'taskFailed'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 5 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<DownloadTask, qt_meta_tag_ZN12DownloadTaskE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject DownloadTask::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12DownloadTaskE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12DownloadTaskE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12DownloadTaskE_t>.metaTypes,
    nullptr
} };

void DownloadTask::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<DownloadTask *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->taskFinished((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->taskFailed((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (DownloadTask::*)(const QString & )>(_a, &DownloadTask::taskFinished, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (DownloadTask::*)(const QString & , const QString & )>(_a, &DownloadTask::taskFailed, 1))
            return;
    }
}

const QMetaObject *DownloadTask::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DownloadTask::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12DownloadTaskE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DownloadTask::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void DownloadTask::taskFinished(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void DownloadTask::taskFailed(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}
namespace {
struct qt_meta_tag_ZN15DownloadServiceE_t {};
} // unnamed namespace

template <> constexpr inline auto DownloadService::qt_create_metaobjectdata<qt_meta_tag_ZN15DownloadServiceE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "DownloadService",
        "DownloadStarted",
        "",
        "download_id",
        "filename",
        "destination",
        "total_bytes",
        "category",
        "DownloadProgress",
        "bytes_received",
        "speed_bytes_sec",
        "QList<double>",
        "segments",
        "DownloadFinished",
        "DownloadFailed",
        "error_message",
        "onTick",
        "onTaskFinished",
        "id",
        "onTaskFailed",
        "error",
        "ResolveDestination",
        "mime_type",
        "GetCategoryInfo",
        "QVariantMap",
        "StartDownload",
        "url",
        "custom_destination",
        "PauseDownload",
        "ResumeDownload",
        "CancelDownload",
        "GetActiveDownloads"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'DownloadStarted'
        QtMocHelpers::SignalData<void(const QString &, const QString &, const QString &, qint64, const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 4 }, { QMetaType::QString, 5 }, { QMetaType::LongLong, 6 },
            { QMetaType::QString, 7 },
        }}),
        // Signal 'DownloadProgress'
        QtMocHelpers::SignalData<void(const QString &, qint64, qint64, double, const QList<double> &)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::LongLong, 9 }, { QMetaType::LongLong, 6 }, { QMetaType::Double, 10 },
            { 0x80000000 | 11, 12 },
        }}),
        // Signal 'DownloadFinished'
        QtMocHelpers::SignalData<void(const QString &, const QString &, const QString &)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 5 }, { QMetaType::QString, 7 },
        }}),
        // Signal 'DownloadFailed'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 15 },
        }}),
        // Slot 'onTick'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onTaskFinished'
        QtMocHelpers::SlotData<void(const QString &)>(17, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 18 },
        }}),
        // Slot 'onTaskFailed'
        QtMocHelpers::SlotData<void(const QString &, const QString &)>(19, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 18 }, { QMetaType::QString, 20 },
        }}),
        // Method 'ResolveDestination'
        QtMocHelpers::MethodData<QString(const QString &, const QString &)>(21, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::QString, 4 }, { QMetaType::QString, 22 },
        }}),
        // Method 'GetCategoryInfo'
        QtMocHelpers::MethodData<QVariantMap(const QString &, const QString &)>(23, 2, QMC::AccessPublic, 0x80000000 | 24, {{
            { QMetaType::QString, 4 }, { QMetaType::QString, 22 },
        }}),
        // Method 'StartDownload'
        QtMocHelpers::MethodData<QString(const QString &, const QString &)>(25, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::QString, 26 }, { QMetaType::QString, 27 },
        }}),
        // Method 'PauseDownload'
        QtMocHelpers::MethodData<void(const QString &)>(28, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Method 'ResumeDownload'
        QtMocHelpers::MethodData<void(const QString &)>(29, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Method 'CancelDownload'
        QtMocHelpers::MethodData<void(const QString &)>(30, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Method 'GetActiveDownloads'
        QtMocHelpers::MethodData<QVariantMap()>(31, 2, QMC::AccessPublic, 0x80000000 | 24),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<DownloadService, qt_meta_tag_ZN15DownloadServiceE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject DownloadService::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15DownloadServiceE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15DownloadServiceE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15DownloadServiceE_t>.metaTypes,
    nullptr
} };

void DownloadService::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<DownloadService *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->DownloadStarted((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<qint64>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[5]))); break;
        case 1: _t->DownloadProgress((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qint64>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<qint64>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<QList<double>>>(_a[5]))); break;
        case 2: _t->DownloadFinished((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 3: _t->DownloadFailed((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 4: _t->onTick(); break;
        case 5: _t->onTaskFinished((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->onTaskFailed((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 7: { QString _r = _t->ResolveDestination((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 8: { QVariantMap _r = _t->GetCategoryInfo((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast<QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 9: { QString _r = _t->StartDownload((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 10: _t->PauseDownload((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 11: _t->ResumeDownload((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 12: _t->CancelDownload((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 13: { QVariantMap _r = _t->GetActiveDownloads();
            if (_a[0]) *reinterpret_cast<QVariantMap*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 4:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<double> >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (DownloadService::*)(const QString & , const QString & , const QString & , qint64 , const QString & )>(_a, &DownloadService::DownloadStarted, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (DownloadService::*)(const QString & , qint64 , qint64 , double , const QList<double> & )>(_a, &DownloadService::DownloadProgress, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (DownloadService::*)(const QString & , const QString & , const QString & )>(_a, &DownloadService::DownloadFinished, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (DownloadService::*)(const QString & , const QString & )>(_a, &DownloadService::DownloadFailed, 3))
            return;
    }
}

const QMetaObject *DownloadService::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DownloadService::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15DownloadServiceE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DownloadService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void DownloadService::DownloadStarted(const QString & _t1, const QString & _t2, const QString & _t3, qint64 _t4, const QString & _t5)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3, _t4, _t5);
}

// SIGNAL 1
void DownloadService::DownloadProgress(const QString & _t1, qint64 _t2, qint64 _t3, double _t4, const QList<double> & _t5)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2, _t3, _t4, _t5);
}

// SIGNAL 2
void DownloadService::DownloadFinished(const QString & _t1, const QString & _t2, const QString & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2, _t3);
}

// SIGNAL 3
void DownloadService::DownloadFailed(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2);
}
QT_WARNING_POP
