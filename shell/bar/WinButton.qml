import QtQuick
import QtQuick.Layouts
import Ciel.Ui

Rectangle {
    id: root

    signal clicked

    property color iconColor: Theme.textSecondary
    property real currentAngle: 0
    property real pinch: 0

    implicitWidth: 34
    implicitHeight: 34
    Layout.preferredWidth: implicitWidth
    Layout.preferredHeight: implicitHeight

    radius: Theme.metrics.radiusSm
    color: mouseArea.containsMouse ? Theme.surfaceHover : Theme.surface

    Behavior on color {
        ColorAnimation {
            duration: 140
            easing.type: Easing.OutQuad
        }
    }

    Item {
        id: gridContainer
        anchors.centerIn: parent
        width: 20
        height: 20
        rotation: root.currentAngle
        transformOrigin: Item.Center

        Rectangle {
            x: 0 + root.pinch
            y: 0 + root.pinch
            width: 9
            height: 9
            color: root.iconColor
            topLeftRadius: 4
            topRightRadius: 1.5
            bottomLeftRadius: 1.5
            bottomRightRadius: 1.5
        }

        Rectangle {
            x: 11 - root.pinch
            y: 0 + root.pinch
            width: 9
            height: 9
            color: root.iconColor
            topLeftRadius: 1.5
            topRightRadius: 4
            bottomLeftRadius: 1.5
            bottomRightRadius: 1.5
        }

        Rectangle {
            x: 0 + root.pinch
            y: 11 - root.pinch
            width: 9
            height: 9
            color: root.iconColor
            topLeftRadius: 1.5
            topRightRadius: 1.5
            bottomLeftRadius: 4
            bottomRightRadius: 1.5
        }

        Rectangle {
            x: 11 - root.pinch
            y: 11 - root.pinch
            width: 9
            height: 9
            color: root.iconColor
            topLeftRadius: 1.5
            topRightRadius: 1.5
            bottomLeftRadius: 1.5
            bottomRightRadius: 4
        }
    }

    ParallelAnimation {
        id: clickAnim

        NumberAnimation {
            target: root
            property: "currentAngle"
            from: root.currentAngle
            to: root.currentAngle + 90
            duration: 380
            easing.type: Easing.OutCubic
        }

        SequentialAnimation {
            NumberAnimation {
                target: root
                property: "pinch"
                from: 0
                to: 1.8
                duration: 120
                easing.type: Easing.OutQuad
            }
            NumberAnimation {
                target: root
                property: "pinch"
                from: 1.8
                to: 0
                duration: 260
                easing.type: Easing.OutBack
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
