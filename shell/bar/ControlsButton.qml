import QtQuick
import QtQuick.Layouts
import Ciel.Ui

Rectangle {
    id: root

    signal clicked

    property color iconColor: Theme.textSecondary
    property real slideOffset: 0.0

    implicitWidth: 34
    implicitHeight: 34
    Layout.preferredWidth: implicitWidth
    Layout.preferredHeight: implicitHeight

    radius: Theme.metrics.radiusSm
    color: mouseArea.containsMouse ? Theme.surfaceHover : Theme.surface

    Behavior on color {
        ColorAnimation {
            duration: 120
            easing.type: Easing.OutQuad
        }
    }

    Item {
        id: iconContainer
        anchors.centerIn: parent
        width: 20
        height: 19

        Item {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: 8.5

            Rectangle {
                x: 0 + (root.slideOffset * 3)
                y: 0
                width: 6.5
                height: 8.5
                color: root.iconColor
                opacity: 0.45
                topLeftRadius: 4
                topRightRadius: 1.5
                bottomLeftRadius: 1.5
                bottomRightRadius: 1.5
            }

            Rectangle {
                x: 8.5 - (root.slideOffset * 3)
                y: 0
                width: 11.5
                height: 8.5
                color: root.iconColor
                topLeftRadius: 1.5
                topRightRadius: 4
                bottomLeftRadius: 1.5
                bottomRightRadius: 1.5
            }
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 8.5

            Rectangle {
                x: 0 + (root.slideOffset * 3)
                y: 0
                width: 11.5
                height: 8.5
                color: root.iconColor
                topLeftRadius: 1.5
                topRightRadius: 1.5
                bottomLeftRadius: 4
                bottomRightRadius: 1.5
            }

            Rectangle {
                x: 13.5 - (root.slideOffset * 3)
                y: 0
                width: 6.5
                height: 8.5
                color: root.iconColor
                opacity: 0.45
                topLeftRadius: 1.5
                topRightRadius: 1.5
                bottomLeftRadius: 1.5
                bottomRightRadius: 4
            }
        }
    }

    ParallelAnimation {
        id: clickAnim

        SequentialAnimation {
            NumberAnimation {
                target: iconContainer
                property: "scale"
                from: 1.0
                to: 0.82
                duration: 80
                easing.type: Easing.OutQuad
            }
            SpringAnimation {
                target: iconContainer
                property: "scale"
                from: 0.82
                to: 1.0
                spring: 4.8
                damping: 0.28
                epsilon: 0.001
            }
        }

        SequentialAnimation {
            NumberAnimation {
                target: root
                property: "slideOffset"
                from: 0.0
                to: 1.0
                duration: 90
                easing.type: Easing.OutQuad
            }
            SpringAnimation {
                target: root
                property: "slideOffset"
                from: 1.0
                to: 0.0
                spring: 4.2
                damping: 0.32
                epsilon: 0.001
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: {
            clickAnim.restart();
            root.clicked();
        }
    }
}
