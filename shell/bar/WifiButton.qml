import QtQuick
import QtQuick.Layouts
import Ciel.Ui

Rectangle {
    id: root

    signal clicked

    property color iconColor: Theme.textSecondary
    property bool interactive: true
    property bool animateOnClick: true

    property real dotY: 0
    property real wave1Y: 0
    property real wave2Y: 0
    property real wave3Y: 0

    property real dotAlpha: 1.0
    property real wave1Alpha: 1.0
    property real wave2Alpha: 1.0
    property real wave3Alpha: 1.0

    function triggerWave() {
        clickAnim.restart();
    }

    implicitWidth: 34
    implicitHeight: 34
    Layout.preferredWidth: implicitWidth
    Layout.preferredHeight: implicitHeight

    radius: Theme.metrics.radiusSm
    color: root.interactive && mouseArea.containsMouse ? Theme.surfaceHover : Theme.surface

    Behavior on color {
        ColorAnimation {
            duration: 140
            easing.type: Easing.OutQuad
        }
    }

    Canvas {
        id: wifiCanvas
        anchors.centerIn: parent
        width: 22
        height: 20
        scale: root.width < 30 ? (root.width / 30) : 1.0

        onPaint: {
            var ctx = getContext("2d");
            ctx.reset();

            var cx = 11;
            var cy = 16;
            var startAngle = -Math.PI * 0.75;
            var endAngle = -Math.PI * 0.25;

            ctx.fillStyle = Qt.rgba(root.iconColor.r, root.iconColor.g, root.iconColor.b, root.dotAlpha);
            ctx.beginPath();
            ctx.arc(cx, cy + root.dotY, 1.6, 0, Math.PI * 2);
            ctx.fill();

            ctx.lineWidth = 1.8;
            ctx.lineCap = "round";

            ctx.strokeStyle = Qt.rgba(root.iconColor.r, root.iconColor.g, root.iconColor.b, root.wave1Alpha);
            ctx.beginPath();
            ctx.arc(cx, cy + root.wave1Y, 5.2, startAngle, endAngle);
            ctx.stroke();

            ctx.strokeStyle = Qt.rgba(root.iconColor.r, root.iconColor.g, root.iconColor.b, root.wave2Alpha);
            ctx.beginPath();
            ctx.arc(cx, cy + root.wave2Y, 9.2, startAngle, endAngle);
            ctx.stroke();

            ctx.strokeStyle = Qt.rgba(root.iconColor.r, root.iconColor.g, root.iconColor.b, root.wave3Alpha);
            ctx.beginPath();
            ctx.arc(cx, cy + root.wave3Y, 13.2, startAngle, endAngle);
            ctx.stroke();
        }

        Connections {
            target: root
            function onDotYChanged() {
                wifiCanvas.requestPaint();
            }
            function onWave1YChanged() {
                wifiCanvas.requestPaint();
            }
            function onWave2YChanged() {
                wifiCanvas.requestPaint();
            }
            function onWave3YChanged() {
                wifiCanvas.requestPaint();
            }
            function onDotAlphaChanged() {
                wifiCanvas.requestPaint();
            }
            function onWave1AlphaChanged() {
                wifiCanvas.requestPaint();
            }
            function onWave2AlphaChanged() {
                wifiCanvas.requestPaint();
            }
            function onWave3AlphaChanged() {
                wifiCanvas.requestPaint();
            }
            function onIconColorChanged() {
                wifiCanvas.requestPaint();
            }
        }
    }

    ParallelAnimation {
        id: clickAnim

        SequentialAnimation {
            ParallelAnimation {
                NumberAnimation {
                    target: root
                    property: "dotY"
                    from: 3
                    to: 0
                    duration: 320
                    easing.type: Easing.OutQuart
                }
                NumberAnimation {
                    target: root
                    property: "dotAlpha"
                    from: 0.2
                    to: 1.0
                    duration: 240
                    easing.type: Easing.OutQuart
                }
            }
        }

        SequentialAnimation {
            PauseAnimation {
                duration: 40
            }
            ParallelAnimation {
                NumberAnimation {
                    target: root
                    property: "wave1Y"
                    from: 3.5
                    to: 0
                    duration: 320
                    easing.type: Easing.OutQuart
                }
                NumberAnimation {
                    target: root
                    property: "wave1Alpha"
                    from: 0.2
                    to: 1.0
                    duration: 220
                    easing.type: Easing.OutQuart
                }
            }
        }

        SequentialAnimation {
            PauseAnimation {
                duration: 80
            }
            ParallelAnimation {
                NumberAnimation {
                    target: root
                    property: "wave2Y"
                    from: 4.0
                    to: 0
                    duration: 360
                    easing.type: Easing.OutQuart
                }
                NumberAnimation {
                    target: root
                    property: "wave2Alpha"
                    from: 0.2
                    to: 1.0
                    duration: 240
                    easing.type: Easing.OutQuart
                }
            }
        }

        SequentialAnimation {
            PauseAnimation {
                duration: 120
            }
            ParallelAnimation {
                NumberAnimation {
                    target: root
                    property: "wave3Y"
                    from: 4.5
                    to: 0
                    duration: 400
                    easing.type: Easing.OutQuart
                }
                NumberAnimation {
                    target: root
                    property: "wave3Alpha"
                    from: 0.2
                    to: 1.0
                    duration: 260
                    easing.type: Easing.OutQuart
                }
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        enabled: root.interactive
        hoverEnabled: root.interactive
        cursorShape: Qt.PointingHandCursor

        onClicked: {
            if (root.animateOnClick) {
                clickAnim.restart();
            }
            root.clicked();
        }
    }
}
