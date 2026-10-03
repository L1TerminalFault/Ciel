/****************************************************************************
** Generated QML type registration code
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtQml/qqml.h>
#include <QtQml/qqmlmoduleregistration.h>

#if __has_include(<AppPaths.hpp>)
#  include <AppPaths.hpp>
#endif
#if __has_include(<MotionTokens.hpp>)
#  include <MotionTokens.hpp>
#endif
#if __has_include(<NotificationsModel.hpp>)
#  include <NotificationsModel.hpp>
#endif
#if __has_include(<Theme.hpp>)
#  include <Theme.hpp>
#endif
#if __has_include(<ThemeMetrics.hpp>)
#  include <ThemeMetrics.hpp>
#endif
#if __has_include(<cielgraphview.hpp>)
#  include <cielgraphview.hpp>
#endif


#if !defined(QT_STATIC)
#define Q_QMLTYPE_EXPORT Q_DECL_EXPORT
#else
#define Q_QMLTYPE_EXPORT
#endif
Q_QMLTYPE_EXPORT void qml_register_types_Ciel_Ui()
{
    QT_WARNING_PUSH QT_WARNING_DISABLE_DEPRECATED
    qmlRegisterTypesAndRevisions<AppPaths>("Ciel.Ui", 1);
    qmlRegisterTypesAndRevisions<CielGraphView>("Ciel.Ui", 1);
    qmlRegisterAnonymousType<QQuickItem, 254>("Ciel.Ui", 1);
    qmlRegisterTypesAndRevisions<CielMotionTokens>("Ciel.Ui", 1);
    qmlRegisterTypesAndRevisions<NotificationsModel>("Ciel.Ui", 1);
    QMetaType::fromType<QAbstractItemModel *>().id();
    qmlRegisterEnum<QAbstractItemModel::LayoutChangeHint>("QAbstractItemModel::LayoutChangeHint");
    qmlRegisterEnum<QAbstractItemModel::CheckIndexOption>("QAbstractItemModel::CheckIndexOption");
    QMetaType::fromType<QAbstractListModel *>().id();
    qmlRegisterTypesAndRevisions<Theme>("Ciel.Ui", 1);
    qmlRegisterEnum<Theme::IconSize>("Theme::IconSize");
    qmlRegisterTypesAndRevisions<ThemeMetrics>("Ciel.Ui", 1);
    QT_WARNING_POP
    qmlRegisterModule("Ciel.Ui", 1, 0);
}

static const QQmlModuleRegistration cielUiRegistration("Ciel.Ui", qml_register_types_Ciel_Ui);
