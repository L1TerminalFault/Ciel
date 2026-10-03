/****************************************************************************
** Generated QML type registration code
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtQml/qqml.h>
#include <QtQml/qqmlmoduleregistration.h>

#if __has_include(<BrowserConfig.hpp>)
#  include <BrowserConfig.hpp>
#endif
#if __has_include(<ProfileManager.hpp>)
#  include <ProfileManager.hpp>
#endif
#if __has_include(<SwipeGestureFilter.hpp>)
#  include <SwipeGestureFilter.hpp>
#endif
#if __has_include(<TabModel.hpp>)
#  include <TabModel.hpp>
#endif
#if __has_include(<WorkspaceModel.hpp>)
#  include <WorkspaceModel.hpp>
#endif


#if !defined(QT_STATIC)
#define Q_QMLTYPE_EXPORT Q_DECL_EXPORT
#else
#define Q_QMLTYPE_EXPORT
#endif
Q_QMLTYPE_EXPORT void qml_register_types_Ciel_Browser()
{
    QT_WARNING_PUSH QT_WARNING_DISABLE_DEPRECATED
    qmlRegisterTypesAndRevisions<BrowserConfig>("Ciel.Browser", 1);
    qmlRegisterTypesAndRevisions<DownloadsModel>("Ciel.Browser", 1);
    qmlRegisterTypesAndRevisions<ProfileManager>("Ciel.Browser", 1);
    QMetaType::fromType<QAbstractItemModel *>().id();
    qmlRegisterEnum<QAbstractItemModel::LayoutChangeHint>("QAbstractItemModel::LayoutChangeHint");
    qmlRegisterEnum<QAbstractItemModel::CheckIndexOption>("QAbstractItemModel::CheckIndexOption");
    QMetaType::fromType<QAbstractListModel *>().id();
    qmlRegisterTypesAndRevisions<SwipeGestureFilter>("Ciel.Browser", 1);
    qmlRegisterTypesAndRevisions<TabModel>("Ciel.Browser", 1);
    qmlRegisterTypesAndRevisions<WorkspaceModel>("Ciel.Browser", 1);
    QT_WARNING_POP
    qmlRegisterModule("Ciel.Browser", 1, 0);
}

static const QQmlModuleRegistration cielBrowserRegistration("Ciel.Browser", qml_register_types_Ciel_Browser);
