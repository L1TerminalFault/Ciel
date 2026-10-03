import Quickshell
import Quickshell.Wayland
import QtQuick
import Ciel.Ui

PanelWindow {
    id: overlay

    anchors {
        top: true
        right: true
    }

    margins {
        top: 20
        right: 12
    }

    WlrLayershell.layer: WlrLayer.Overlay
    WlrLayershell.keyboardFocus: WlrKeyboardFocus.None

    implicitWidth: 420
    implicitHeight: 800
    color: "transparent"

    visible: NotificationsModel.unreadCount > 0

    mask: Region {
        item: container
    }

    Column {
        id: container
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        width: 390
        spacing: 6

        // Pure vertical cascading pushdown
        move: Transition {
            id: moveTrans

            SequentialAnimation {
                PauseAnimation {
                    duration: Math.min(270, moveTrans.ViewTransition.index * 75)
                }

                SpringAnimation {
                    properties: "y"
                    spring: 2.4
                    damping: 0.30
                    mass: 1.15
                    epsilon: 0.001
                }
            }
        }

        Repeater {
            model: NotificationsModel

            delegate: NotificationToast {
                width: container.width
                notificationId: model.id
                appName: model.appName
                summary: model.summary
                body: model.body
            }
        }
    }
}
