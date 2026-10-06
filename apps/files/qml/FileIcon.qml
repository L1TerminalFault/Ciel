import QtQuick
import QtQuick.Shapes
import QtQuick.Effects

Item {
    id: root

    implicitWidth: 50
    implicitHeight: 64

    readonly property real r: Math.min(width, height) * 0.12
    readonly property real fold: width * 0.32

    MultiEffect {
        source: pageBody
        anchors.fill: pageBody
        shadowEnabled: true
        shadowColor: "#40000000"
        shadowBlur: 0.3
        shadowVerticalOffset: 0
        shadowHorizontalOffset: 0
    }

    Shape {
        id: pageBody
        anchors.fill: parent
        layer.enabled: true
        layer.samples: 4

        ShapePath {
            strokeWidth: 1
            strokeColor: "#00000024"
            fillColor: "#f3f6f9"

            startX: 0
            startY: root.r

            PathQuad {
                x: root.r
                y: 0
                controlX: 0
                controlY: 0
            }
            PathLine {
                x: root.width - root.fold
                y: 0
            }
            PathLine {
                x: root.width
                y: root.fold
            }
            PathLine {
                x: root.width
                y: root.height - root.r
            }
            PathQuad {
                x: root.width - root.r
                y: root.height
                controlX: root.width
                controlY: root.height
            }
            PathLine {
                x: root.r
                y: root.height
            }
            PathQuad {
                x: 0
                y: root.height - root.r
                controlX: 0
                controlY: root.height
            }
            PathLine {
                x: 0
                y: root.r
            }
        }
    }

    Shape {
        id: foldShadow
        anchors.fill: parent
        opacity: 0.16
        layer.enabled: true
        layer.samples: 4

        ShapePath {
            strokeWidth: 0
            strokeColor: "transparent"
            fillColor: "#000000"

            startX: root.width - root.fold
            startY: 0

            PathLine {
                x: root.width - root.fold + 3
                y: root.fold + 3
            }
            PathLine {
                x: root.width
                y: root.fold
            }
            PathLine {
                x: root.width - root.fold
                y: root.fold
            }
            PathLine {
                x: root.width - root.fold
                y: 0
            }
        }
    }

    Shape {
        id: foldFlap
        anchors.fill: parent
        layer.enabled: true
        layer.samples: 4

        ShapePath {
            strokeWidth: 1
            strokeColor: "#00000024"
            fillColor: "#dde1e6"

            startX: root.width - root.fold
            startY: 0

            PathLine {
                x: root.width - root.fold
                y: root.fold - (root.r * 0.45)
            }
            PathQuad {
                x: root.width - root.fold + (root.r * 0.45)
                y: root.fold
                controlX: root.width - root.fold
                controlY: root.fold
            }
            PathLine {
                x: root.width
                y: root.fold
            }
            PathLine {
                x: root.width - root.fold
                y: 0
            }
        }
    }
}
