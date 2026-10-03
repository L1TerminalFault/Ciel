import QtQuick
import QtQuick.Layouts
import Ciel.Ui

Item {
    id: root

    property alias icon: iconItem.icon
    property alias iconRotation: iconItem.rotation
    property color iconColor: Theme.textPrimary
    property int size: Theme.MEDIUM
    property alias iconSize: root.size
    property bool active: false
    property bool primary: false

    readonly property real buttonSize: {
        switch (root.size) {
        case Theme.XXSMALL:
            return 18;
        case Theme.XSMALL:
            return 24;
        case Theme.SMALL:
            return 30;
        case Theme.MEDIUM:
            return 36;
        case Theme.LARGE:
            return 44;
        case Theme.XLARGE:
            return 56;
        default:
            return 36;
        }
    }

    readonly property real buttonRadius: {
        switch (root.size) {
        case Theme.XXSMALL:
            return 4;
        case Theme.XSMALL:
            return 5;
        case Theme.SMALL:
            return 7;
        case Theme.MEDIUM:
            return 8;
        case Theme.LARGE:
            return 10;
        case Theme.XLARGE:
            return 12;
        default:
            return 8;
        }
    }

    signal clicked

    implicitWidth: buttonSize
    implicitHeight: buttonSize
    width: implicitWidth
    height: implicitHeight

    scale: mouseArea.pressed ? 0.90 : 1.0
    transformOrigin: Item.Center

    Behavior on scale {
        CielSpring {
            damping: 0.28
            spring: 4.8
            mass: 1.0
            epsilon: 0.001
        }
    }

    CielSquircle {
        id: bg
        anchors.fill: parent
        radius: root.buttonRadius
        color: root.primary ? Theme.surface : Theme.surface

        opacity: {
            if (root.active)
                return 0.85;
            if (mouseArea.pressed)
                return 0.65;
            if (mouseArea.containsMouse)
                return 0.38;
            return 0.0;
        }

        Behavior on opacity {
            NumberAnimation {
                duration: 90
                easing.type: Easing.OutQuad
            }
        }
    }

    CielIcon {
        id: iconItem
        anchors.centerIn: parent
        size: root.size
        color: root.iconColor
        transformOrigin: Item.Center

        Behavior on color {
            ColorAnimation {
                duration: 120
            }
        }

        Behavior on rotation {
            CielSpring {
                damping: 4.0
                spring: 8.0
                mass: 2.0
                epsilon: 0.3
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: root.clicked()
    }
}
