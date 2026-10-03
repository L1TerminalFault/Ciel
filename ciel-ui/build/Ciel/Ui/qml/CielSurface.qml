import QtQuick
import Ciel.Ui

Item {
    id: root

    property bool interactive: true
    property real radius: Theme.metrics.radiusMd
    property alias color: squircle.color
    property bool hovered: mouseArea.containsMouse
    property bool pressed: mouseArea.pressed

    default property alias content: container.data

    signal clicked

    implicitWidth: 120
    implicitHeight: 44

    // Ensure scaling pivots from the exact center
    transformOrigin: Item.Center

    // Hardware accelerate scale transforms
    layer.enabled: pressed || scaleAnimation.running
    layer.smooth: true

    CielSquircle {
        id: squircle
        anchors.fill: parent
        radius: root.radius

        color: {
            if (!root.interactive)
                return Theme.surface;
            if (root.pressed)
                return Theme.surfacePressed;
            if (root.hovered)
                return Theme.surfaceHover;
            return Theme.surface;
        }
    }

    Item {
        id: container
        anchors.fill: parent
    }

    scale: pressed ? 0.96 : 1.0

    Behavior on scale {
        CielSpring {
            id: scaleAnimation
            spring: 5.0
            damping: 0.70
            mass: 0.8
            epsilon: 0.0005
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: root.interactive
        cursorShape: root.interactive ? Qt.PointingHandCursor : Qt.ArrowCursor
        enabled: root.interactive
        onClicked: root.clicked()
    }
}
