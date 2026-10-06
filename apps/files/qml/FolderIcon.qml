import QtQuick
import QtQuick.Shapes

Item {
    id: root

    property bool hasFiles: true

    implicitWidth: 82
    implicitHeight: 64

    readonly property real r: Math.min(width, height) * 0.10

    Shape {
        id: backFlap
        anchors.fill: parent
        layer.enabled: true
        layer.samples: 4

        ShapePath {
            strokeWidth: 0
            strokeColor: "transparent"

            fillGradient: LinearGradient {
                x1: 0
                y1: 0
                x2: 0
                y2: root.height * 0.40
                GradientStop {
                    position: 0.0
                    color: "#64c5fa"
                }
                GradientStop {
                    position: 0.8
                    color: "#3aa4f4"
                }
                GradientStop {
                    position: 1.0
                    color: "#1a5ca6"
                }
            }

            startX: 0
            startY: root.height * 0.40

            PathLine {
                x: 0
                y: root.r
            }
            PathQuad {
                x: root.r
                y: 0
                controlX: 0
                controlY: 0
            }
            PathLine {
                x: root.width * 0.30
                y: 0
            }
            PathCubic {
                x: root.width * 0.43
                y: root.height * 0.12
                control1X: root.width * 0.36
                control1Y: 0
                control2X: root.width * 0.38
                control2Y: root.height * 0.12
            }
            PathLine {
                x: root.width - root.r
                y: root.height * 0.12
            }
            PathQuad {
                x: root.width
                y: root.height * 0.12 + root.r
                controlX: root.width
                controlY: root.height * 0.12
            }
            PathLine {
                x: root.width
                y: root.height * 0.40
            }
            PathLine {
                x: 0
                y: root.height * 0.40
            }
        }
    }

    Rectangle {
        id: whiteInsert
        visible: root.hasFiles
        x: root.width * 0.05
        y: root.height * 0.18
        width: root.width * 0.90
        height: root.height * 0.20
        radius: root.r * 0.5
        color: "#ffffff"
    }

    Rectangle {
        id: frontFlap
        x: 0
        y: root.height * 0.22
        width: root.width
        height: root.height * 0.78
        radius: root.r
        color: "#64c5fa"
    }
}
