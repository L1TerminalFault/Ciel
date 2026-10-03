import QtQuick
import Ciel.Ui

Item {
    id: root

    property bool connecting: false
    property bool checked: false
    property color color: Theme.accent
    property real strokeWidth: 1.8

    property real spinAngle: 0.0
    property real flowProgress: 0.0

    implicitWidth: 18
    implicitHeight: 18

    opacity: (connecting || checked) ? 1.0 : 0.0
    scale: checked ? 1.0 : (connecting ? 0.95 : 0.0)

    Behavior on opacity {
        NumberAnimation {
            duration: 120
            easing.type: Easing.OutQuad
        }
    }

    Behavior on scale {
        SpringAnimation {
            spring: 4.6
            damping: 0.32
            epsilon: 0.001
        }
    }

    onConnectingChanged: {
        if (connecting) {
            flowAnim.stop();
            flowProgress = 0.0;
            spinAnim.restart();
        } else {
            spinAnim.stop();
        }
        canvas.requestPaint();
    }

    onCheckedChanged: {
        if (checked) {
            spinAnim.stop();
            flowAnim.restart();
        } else {
            flowAnim.stop();
            flowProgress = 0.0;
        }
        canvas.requestPaint();
    }

    NumberAnimation on spinAngle {
        id: spinAnim
        from: 0.0
        to: Math.PI * 2
        duration: 720
        loops: Animation.Infinite
        running: false
    }

    NumberAnimation {
        id: flowAnim
        target: root
        property: "flowProgress"
        from: 0.0
        to: 1.0
        duration: 340
        easing.type: Easing.OutCubic
    }

    Canvas {
        id: canvas
        anchors.fill: parent

        onPaint: {
            var ctx = getContext("2d");
            ctx.reset();

            if (!root.connecting && !root.checked && root.flowProgress === 0.0)
                return;

            var cx = 9.0;
            var cy = 9.5;
            var r = 5.4;

            var p0 = {
                x: cx - r,
                y: cy
            };
            var p1 = {
                x: 7.2,
                y: 13.3
            };
            var p2 = {
                x: 14.5,
                y: 4.8
            };

            ctx.lineWidth = root.strokeWidth;
            ctx.lineCap = "round";
            ctx.lineJoin = "round";
            ctx.strokeStyle = root.color;

            if (root.connecting) {
                ctx.beginPath();
                ctx.arc(cx, cy, r, root.spinAngle, root.spinAngle + (Math.PI * 1.55));
                ctx.stroke();
                return;
            }

            if (root.checked || root.flowProgress > 0.0) {
                var t = root.flowProgress;

                if (t < 0.45) {
                    var remainingArc = (Math.PI * 1.45) * (1.0 - (t / 0.45));
                    ctx.beginPath();
                    ctx.arc(cx, cy, r, Math.PI + remainingArc, Math.PI, true);
                    ctx.stroke();
                }

                ctx.beginPath();
                ctx.moveTo(p0.x, p0.y);

                var seg1Len = Math.sqrt(Math.pow(p1.x - p0.x, 2) + Math.pow(p1.y - p0.y, 2));
                var seg2Len = Math.sqrt(Math.pow(p2.x - p1.x, 2) + Math.pow(p2.y - p1.y, 2));
                var totalTickLen = seg1Len + seg2Len;

                var headProgress = Math.min(1.0, t * 1.22);
                var curLen = totalTickLen * headProgress;

                if (curLen <= seg1Len) {
                    var r1 = curLen / seg1Len;
                    ctx.lineTo(p0.x + (p1.x - p0.x) * r1, p0.y + (p1.y - p0.y) * r1);
                } else {
                    ctx.lineTo(p1.x, p1.y);
                    var rem = curLen - seg1Len;
                    var r2 = rem / seg2Len;
                    ctx.lineTo(p1.x + (p2.x - p1.x) * r2, p1.y + (p2.y - p1.y) * r2);
                }

                ctx.stroke();
            }
        }

        Connections {
            target: root
            function onSpinAngleChanged() {
                canvas.requestPaint();
            }
            function onFlowProgressChanged() {
                canvas.requestPaint();
            }
            function onColorChanged() {
                canvas.requestPaint();
            }
        }
    }
}
