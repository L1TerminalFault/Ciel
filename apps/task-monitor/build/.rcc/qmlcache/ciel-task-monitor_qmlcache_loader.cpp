#include <QtQml/qqmlprivate.h>
#include <QtCore/qdir.h>
#include <QtCore/qurl.h>
#include <QtCore/qhash.h>
#include <QtCore/qstring.h>

namespace QmlCacheGeneratedCode {
namespace _qt_qml_Ciel_TaskMonitor_qml_Main_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_Ciel_TaskMonitor_qml_ui_toolbar_Toolbar_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_Ciel_TaskMonitor_qml_ui_ProcessView_ProcessView_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_Ciel_TaskMonitor_qml_ui_PerformanceView_PerformanceView_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_Ciel_TaskMonitor_qml_ui_StartupView_StartupView_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_Ciel_TaskMonitor_qml_ui_BootTimeView_BootTimeView_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_Ciel_TaskMonitor_qml_ui_SettingsView_SettingsView_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}

}
namespace {
struct Registry {
    Registry();
    ~Registry();
    QHash<QString, const QQmlPrivate::CachedQmlUnit*> resourcePathToCachedUnit;
    static const QQmlPrivate::CachedQmlUnit *lookupCachedUnit(const QUrl &url);
};

Q_GLOBAL_STATIC(Registry, unitRegistry)


Registry::Registry() {
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/Ciel/TaskMonitor/qml/Main.qml"), &QmlCacheGeneratedCode::_qt_qml_Ciel_TaskMonitor_qml_Main_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/Ciel/TaskMonitor/qml/ui/toolbar/Toolbar.qml"), &QmlCacheGeneratedCode::_qt_qml_Ciel_TaskMonitor_qml_ui_toolbar_Toolbar_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/Ciel/TaskMonitor/qml/ui/ProcessView/ProcessView.qml"), &QmlCacheGeneratedCode::_qt_qml_Ciel_TaskMonitor_qml_ui_ProcessView_ProcessView_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/Ciel/TaskMonitor/qml/ui/PerformanceView/PerformanceView.qml"), &QmlCacheGeneratedCode::_qt_qml_Ciel_TaskMonitor_qml_ui_PerformanceView_PerformanceView_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/Ciel/TaskMonitor/qml/ui/StartupView/StartupView.qml"), &QmlCacheGeneratedCode::_qt_qml_Ciel_TaskMonitor_qml_ui_StartupView_StartupView_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/Ciel/TaskMonitor/qml/ui/BootTimeView/BootTimeView.qml"), &QmlCacheGeneratedCode::_qt_qml_Ciel_TaskMonitor_qml_ui_BootTimeView_BootTimeView_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/Ciel/TaskMonitor/qml/ui/SettingsView/SettingsView.qml"), &QmlCacheGeneratedCode::_qt_qml_Ciel_TaskMonitor_qml_ui_SettingsView_SettingsView_qml::unit);
    QQmlPrivate::RegisterQmlUnitCacheHook registration;
    registration.structVersion = 0;
    registration.lookupCachedQmlUnit = &lookupCachedUnit;
    QQmlPrivate::qmlregister(QQmlPrivate::QmlUnitCacheHookRegistration, &registration);
}

Registry::~Registry() {
    QQmlPrivate::qmlunregister(QQmlPrivate::QmlUnitCacheHookRegistration, quintptr(&lookupCachedUnit));
}

const QQmlPrivate::CachedQmlUnit *Registry::lookupCachedUnit(const QUrl &url) {
    if (url.scheme() != QLatin1String("qrc"))
        return nullptr;
    QString resourcePath = QDir::cleanPath(url.path());
    if (resourcePath.isEmpty())
        return nullptr;
    if (!resourcePath.startsWith(QLatin1Char('/')))
        resourcePath.prepend(QLatin1Char('/'));
    return unitRegistry()->resourcePathToCachedUnit.value(resourcePath, nullptr);
}
}
int QT_MANGLE_NAMESPACE(qInitResources_qmlcache_ciel_task_monitor)() {
    ::unitRegistry();
    return 1;
}
Q_CONSTRUCTOR_FUNCTION(QT_MANGLE_NAMESPACE(qInitResources_qmlcache_ciel_task_monitor))
int QT_MANGLE_NAMESPACE(qCleanupResources_qmlcache_ciel_task_monitor)() {
    return 1;
}
