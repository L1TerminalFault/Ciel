/****************************************************************************
** Meta object code from reading C++ file 'DownloadsAdaptor.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "DownloadsAdaptor.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DownloadsAdaptor.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN16DownloadsAdaptorE_t {};
} // unnamed namespace

template <> constexpr inline auto DownloadsAdaptor::qt_create_metaobjectdata<qt_meta_tag_ZN16DownloadsAdaptorE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "DownloadsAdaptor",
        "D-Bus Interface",
        "org.ciel.Downloads",
        "D-Bus Introspection",
        "  <interface name=\"org.ciel.Downloads\">\n    <method name=\"Reso"
        "lveDestination\">\n      <arg direction=\"in\" type=\"s\" name=\"f"
        "ilename\"/>\n      <arg direction=\"in\" type=\"s\" name=\"mime_ty"
        "pe\"/>\n      <arg direction=\"out\" type=\"s\" name=\"destination"
        "\"/>\n    </method>\n    <method name=\"GetCategoryInfo\">\n      "
        "<arg direction=\"in\" type=\"s\" name=\"filename\"/>\n      <arg d"
        "irection=\"in\" type=\"s\" name=\"mime_type\"/>\n      <arg direct"
        "ion=\"out\" type=\"a{sv}\" name=\"info\"/>\n      <annotation valu"
        "e=\"QVariantMap\" name=\"org.qtproject.QtDBus.QtTypeName.Out0\"/>\n"
        "    </method>\n    <method name=\"StartDownload\">\n      <arg dir"
        "ection=\"in\" type=\"s\" name=\"url\"/>\n      <arg direction=\"in"
        "\" type=\"s\" name=\"custom_destination\"/>\n      <arg direction="
        "\"out\" type=\"s\" name=\"download_id\"/>\n    </method>\n    <met"
        "hod name=\"PauseDownload\">\n      <arg direction=\"in\" type=\"s\""
        " name=\"download_id\"/>\n    </method>\n    <method name=\"ResumeD"
        "ownload\">\n      <arg direction=\"in\" type=\"s\" name=\"download"
        "_id\"/>\n    </method>\n    <method name=\"CancelDownload\">\n    "
        "  <arg direction=\"in\" type=\"s\" name=\"download_id\"/>\n    </m"
        "ethod>\n    <method name=\"GetActiveDownloads\">\n      <arg direc"
        "tion=\"out\" type=\"a{sv}\" name=\"downloads\"/>\n      <annotatio"
        "n value=\"QVariantMap\" name=\"org.qtproject.QtDBus.QtTypeName.Out"
        "0\"/>\n    </method>\n    <signal name=\"DownloadStarted\">\n     "
        " <arg type=\"s\" name=\"download_id\"/>\n      <arg type=\"s\" nam"
        "e=\"filename\"/>\n      <arg type=\"s\" name=\"destination\"/>\n  "
        "    <arg type=\"x\" name=\"total_bytes\"/>\n      <arg type=\"s\" "
        "name=\"category\"/>\n    </signal>\n    <signal name=\"DownloadPro"
        "gress\">\n      <arg type=\"s\" name=\"download_id\"/>\n      <arg"
        " type=\"x\" name=\"bytes_received\"/>\n      <arg type=\"x\" name="
        "\"total_bytes\"/>\n      <arg type=\"d\" name=\"speed_bytes_sec\"/"
        ">\n      <arg type=\"ad\" name=\"segments\"/>\n      <annotation v"
        "alue=\"QList&lt;double&gt;\" name=\"org.qtproject.QtDBus.QtTypeNam"
        "e.Out4\"/>\n    </signal>\n    <signal name=\"DownloadFinished\">\n"
        "      <arg type=\"s\" name=\"download_id\"/>\n      <arg type=\"s\""
        " name=\"destination\"/>\n      <arg type=\"s\" name=\"category\"/>"
        "\n    </signal>\n    <signal name=\"DownloadFailed\">\n      <arg "
        "type=\"s\" name=\"download_id\"/>\n      <arg type=\"s\" name=\"er"
        "ror_message\"/>\n    </signal>\n  </interface>\n",
        "DownloadFailed",
        "",
        "download_id",
        "error_message",
        "DownloadFinished",
        "destination",
        "category",
        "DownloadProgress",
        "bytes_received",
        "total_bytes",
        "speed_bytes_sec",
        "QList<double>",
        "segments",
        "DownloadStarted",
        "filename",
        "CancelDownload",
        "GetActiveDownloads",
        "QVariantMap",
        "GetCategoryInfo",
        "mime_type",
        "PauseDownload",
        "ResolveDestination",
        "ResumeDownload",
        "StartDownload",
        "url",
        "custom_destination"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'DownloadFailed'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(5, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 7 }, { QMetaType::QString, 8 },
        }}),
        // Signal 'DownloadFinished'
        QtMocHelpers::SignalData<void(const QString &, const QString &, const QString &)>(9, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 7 }, { QMetaType::QString, 10 }, { QMetaType::QString, 11 },
        }}),
        // Signal 'DownloadProgress'
        QtMocHelpers::SignalData<void(const QString &, qlonglong, qlonglong, double, const QList<double> &)>(12, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 7 }, { QMetaType::LongLong, 13 }, { QMetaType::LongLong, 14 }, { QMetaType::Double, 15 },
            { 0x80000000 | 16, 17 },
        }}),
        // Signal 'DownloadStarted'
        QtMocHelpers::SignalData<void(const QString &, const QString &, const QString &, qlonglong, const QString &)>(18, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 7 }, { QMetaType::QString, 19 }, { QMetaType::QString, 10 }, { QMetaType::LongLong, 14 },
            { QMetaType::QString, 11 },
        }}),
        // Slot 'CancelDownload'
        QtMocHelpers::SlotData<void(const QString &)>(20, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 7 },
        }}),
        // Slot 'GetActiveDownloads'
        QtMocHelpers::SlotData<QVariantMap()>(21, 6, QMC::AccessPublic, 0x80000000 | 22),
        // Slot 'GetCategoryInfo'
        QtMocHelpers::SlotData<QVariantMap(const QString &, const QString &)>(23, 6, QMC::AccessPublic, 0x80000000 | 22, {{
            { QMetaType::QString, 19 }, { QMetaType::QString, 24 },
        }}),
        // Slot 'PauseDownload'
        QtMocHelpers::SlotData<void(const QString &)>(25, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 7 },
        }}),
        // Slot 'ResolveDestination'
        QtMocHelpers::SlotData<QString(const QString &, const QString &)>(26, 6, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::QString, 19 }, { QMetaType::QString, 24 },
        }}),
        // Slot 'ResumeDownload'
        QtMocHelpers::SlotData<void(const QString &)>(27, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 7 },
        }}),
        // Slot 'StartDownload'
        QtMocHelpers::SlotData<QString(const QString &, const QString &)>(28, 6, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::QString, 29 }, { QMetaType::QString, 30 },
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
    return QtMocHelpers::metaObjectData<DownloadsAdaptor, qt_meta_tag_ZN16DownloadsAdaptorE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject DownloadsAdaptor::staticMetaObject = { {
    QMetaObject::SuperData::link<QDBusAbstractAdaptor::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16DownloadsAdaptorE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16DownloadsAdaptorE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN16DownloadsAdaptorE_t>.metaTypes,
    nullptr
} };

void DownloadsAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<DownloadsAdaptor *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->DownloadFailed((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 1: _t->DownloadFinished((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 2: _t->DownloadProgress((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qlonglong>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<qlonglong>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<QList<double>>>(_a[5]))); break;
        case 3: _t->DownloadStarted((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<qlonglong>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[5]))); break;
        case 4: _t->CancelDownload((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: { QVariantMap _r = _t->GetActiveDownloads();
            if (_a[0]) *reinterpret_cast<QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 6: { QVariantMap _r = _t->GetCategoryInfo((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast<QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 7: _t->PauseDownload((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: { QString _r = _t->ResolveDestination((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 9: _t->ResumeDownload((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 10: { QString _r = _t->StartDownload((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 4:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<double> >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (DownloadsAdaptor::*)(const QString & , const QString & )>(_a, &DownloadsAdaptor::DownloadFailed, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (DownloadsAdaptor::*)(const QString & , const QString & , const QString & )>(_a, &DownloadsAdaptor::DownloadFinished, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (DownloadsAdaptor::*)(const QString & , qlonglong , qlonglong , double , const QList<double> & )>(_a, &DownloadsAdaptor::DownloadProgress, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (DownloadsAdaptor::*)(const QString & , const QString & , const QString & , qlonglong , const QString & )>(_a, &DownloadsAdaptor::DownloadStarted, 3))
            return;
    }
}

const QMetaObject *DownloadsAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DownloadsAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16DownloadsAdaptorE_t>.strings))
        return static_cast<void*>(this);
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int DownloadsAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void DownloadsAdaptor::DownloadFailed(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}

// SIGNAL 1
void DownloadsAdaptor::DownloadFinished(const QString & _t1, const QString & _t2, const QString & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2, _t3);
}

// SIGNAL 2
void DownloadsAdaptor::DownloadProgress(const QString & _t1, qlonglong _t2, qlonglong _t3, double _t4, const QList<double> & _t5)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2, _t3, _t4, _t5);
}

// SIGNAL 3
void DownloadsAdaptor::DownloadStarted(const QString & _t1, const QString & _t2, const QString & _t3, qlonglong _t4, const QString & _t5)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2, _t3, _t4, _t5);
}
QT_WARNING_POP
