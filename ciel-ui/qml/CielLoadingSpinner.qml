import QtQuick
import Ciel.Ui

Item {
    id: root

    property bool finished: true
    property color color: Theme.textPrimary
    property real orbitRadius: 6.8
    property real finalSize: 4.5
    property real minDotSize: 2.0
    property real maxDotSize: 5.5

    property real currentOrbit: 0.0
    property real centerPopScale: 1.0
    property real wavePhase: 0.0

    implicitWidth: 26
    implicitHeight: 26

    onFinishedChanged: {
        if (!finished) {
            collapseAnim.stop();
            centerPopScale = 1.0;
            expandAnim.restart();
        } else {
            expandAnim.stop();
            collapseAnim.restart();
        }
    }

    SpringAnimation {
        id: expandAnim
        target: root
        property: "currentOrbit"
        from: 0.0
        to: root.orbitRadius
        spring: 4.0
        damping: 0.42
        epsilon: 0.001
    }

    ParallelAnimation {
        id: collapseAnim

        SpringAnimation {
            target: root
            property: "currentOrbit"
            from: root.orbitRadius
            to: 0.0
            spring: 3.5
            damping: 0.45
            epsilon: 0.001
        }

        SequentialAnimation {
            PauseAnimation {
                duration: 270
            }

            NumberAnimation {
                target: root
                property: "centerPopScale"
                from: 1.0
                to: 1.38
                duration: 120
                easing.type: Easing.OutQuad
            }

            SpringAnimation {
                target: root
                property: "centerPopScale"
                from: 1.38
                to: 1.0
                spring: 3.4
                damping: 0.40
                epsilon: 0.001
            }
        }
    }

    NumberAnimation on wavePhase {
        from: 0.0
        to: Math.PI * 2
        duration: 950
        loops: Animation.Infinite
        running: !root.finished
    }

    Item {
        anchors.fill: parent

        Repeater {
            model: 6

            delegate: Rectangle {
                id: dot

                property real angle: (index / 6.0) * Math.PI * 2
                property real waveFactor: 0.5 * (1.0 + Math.cos(angle - root.wavePhase))
                property real dynamicSize: root.minDotSize + ((root.maxDotSize - root.minDotSize) * Math.pow(waveFactor, 1.8))

                property real progress: Math.min(1.0, Math.max(0.0, root.currentOrbit / root.orbitRadius))
                property real baseDisplaySize: (root.finalSize * (1.0 - progress)) + (dynamicSize * progress)
                property real popMultiplier: 1.0 + ((root.centerPopScale - 1.0) * (1.0 - progress))

                width: Math.max(1.2, baseDisplaySize * popMultiplier)
                height: width
                radius: width / 2
                color: root.color
                opacity: 0.35 + (waveFactor * 0.65)

                x: (parent.width / 2) + (Math.cos(angle) * root.currentOrbit) - (width / 2)
                y: (parent.height / 2) + (Math.sin(angle) * root.currentOrbit) - (height / 2)
            }
        }
    }
}
