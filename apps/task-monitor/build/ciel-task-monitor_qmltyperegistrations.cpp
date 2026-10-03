/****************************************************************************
** Generated QML type registration code
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtQml/qqml.h>
#include <QtQml/qqmlmoduleregistration.h>

#if __has_include(<BootPerformanceModel.hpp>)
#  include <BootPerformanceModel.hpp>
#endif
#if __has_include(<ProcessTableModel.hpp>)
#  include <ProcessTableModel.hpp>
#endif
#if __has_include(<StartupAppsModel.hpp>)
#  include <StartupAppsModel.hpp>
#endif


#if !defined(QT_STATIC)
#define Q_QMLTYPE_EXPORT Q_DECL_EXPORT
#else
#define Q_QMLTYPE_EXPORT
#endif
Q_QMLTYPE_EXPORT void qml_register_types_Ciel_TaskMonitor()
{
    QT_WARNING_PUSH QT_WARNING_DISABLE_DEPRECATED
    qmlRegisterTypesAndRevisions<BootPerformanceModel>("Ciel.TaskMonitor", 1);
    qmlRegisterTypesAndRevisions<ProcessTableModel>("Ciel.TaskMonitor", 1);
    QMetaType::fromType<QAbstractItemModel *>().id();
    qmlRegisterEnum<QAbstractItemModel::LayoutChangeHint>("QAbstractItemModel::LayoutChangeHint");
    qmlRegisterEnum<QAbstractItemModel::CheckIndexOption>("QAbstractItemModel::CheckIndexOption");
    QMetaType::fromType<QAbstractListModel *>().id();
    QMetaType::fromType<QAbstractTableModel *>().id();
    qmlRegisterTypesAndRevisions<StartupAppsModel>("Ciel.TaskMonitor", 1);
    qmlRegisterEnum<StartupAppsModel::ScopeFilter>("StartupAppsModel::ScopeFilter");
    QT_WARNING_POP
    qmlRegisterModule("Ciel.TaskMonitor", 1, 0);
}

static const QQmlModuleRegistration cielTaskMonitorRegistration("Ciel.TaskMonitor", qml_register_types_Ciel_TaskMonitor);
