import QtQuick
import Ciel.Ui

Item {
    id: root

    property bool isOpen: false
    default property alias content: container.data

    // Pronounced physical motion parameters
    property real startYOffset: -28.0
    property real startScaleX: 0.90
    property real startScaleY: 0.84

    implicitWidth: container.implicitWidth
    // Reserve vertical headroom so the spring overshoot never clips against the parent bounds
    implicitHeight: container.implicitHeight + 16

    visible: opacity > 0.0

    Item {
        id: animatedWrapper
        width: root.width
        height: root.implicitHeight - 16
        y: 8

        transformOrigin: Item.Center

        transform: Scale {
            id: scaleTransform
            origin.x: animatedWrapper.width / 2
            origin.y: 0
            xScale: root.startScaleX
            yScale: root.startScaleY
        }

        Item {
            id: container
            anchors.fill: parent
        }
    }

    function open() {
        isOpen = true;
    }

    function close() {
        isOpen = false;
    }

    onIsOpenChanged: {
        if (isOpen) {
            closeTransition.stop();
            openTransition.restart();
        } else {
            openTransition.stop();
            closeTransition.restart();
        }
    }

    ParallelAnimation {
        id: openTransition

        // Pronounced vertical spring with clear physical bounce
        SpringAnimation {
            target: animatedWrapper
            property: "y"
            from: root.startYOffset
            to: 8
            spring: 3.6
            damping: 0.58  // Lower damping = clear, visible overshoot
            mass: 1.0
            epsilon: 0.0005
        }

        // Horizontal elastic expansion
        SpringAnimation {
            target: scaleTransform
            property: "xScale"
            from: root.startScaleX
            to: 1.0
            spring: 4.0
            damping: 0.56
            mass: 0.9
            epsilon: 0.0005
        }

        // Vertical elastic expansion
        SpringAnimation {
            target: scaleTransform
            property: "yScale"
            from: root.startScaleY
            to: 1.0
            spring: 3.6
            damping: 0.58
            mass: 1.0
            epsilon: 0.0005
        }

        NumberAnimation {
            target: root
            property: "opacity"
            from: 0.0
            to: 1.0
            duration: 140
            easing.type: Easing.OutQuad
        }
    }

    ParallelAnimation {
        id: closeTransition

        NumberAnimation {
            target: animatedWrapper
            property: "y"
            to: root.startYOffset
            duration: 160
            easing.type: Easing.InQuad
        }

        NumberAnimation {
            target: scaleTransform
            property: "yScale"
            to: root.startScaleY
            duration: 160
            easing.type: Easing.InQuad
        }

        NumberAnimation {
            target: root
            property: "opacity"
            to: 0.0
            duration: 140
            easing.type: Easing.InQuad
        }
    }
}
