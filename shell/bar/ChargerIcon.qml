import QtQuick
import QtQuick.Shapes
import Ciel.Ui

Item {
    id: root

    property bool active: false
    property color color: "#FFFFFF"

    implicitWidth: 9
    implicitHeight: 11.5

    scale: active ? 1.0 : 0.0
    opacity: active ? 1.0 : 0.0

    Behavior on scale {
        SpringAnimation {
            spring: 5.0
            damping: 0.28
            epsilon: 0.001
        }
    }

    Behavior on opacity {
        NumberAnimation {
            duration: 100
            easing.type: Easing.OutQuad
        }
    }

    Shape {
        id: boltShape
        anchors.fill: parent
        preferredRendererType: Shape.GeometryRenderer

        ShapePath {
            fillColor: root.color
            strokeColor: "transparent"
            strokeWidth: 0

            startX: 5.2
            startY: 0.5

            PathLine {
                x: 1.1
                y: 6.0
            }
            PathLine {
                x: 4.4
                y: 6.0
            }
            PathLine {
                x: 3.1
                y: 11.0
            }
            PathLine {
                x: 8.2
                y: 4.6
            }
            PathLine {
                x: 5.0
                y: 4.6
            }
            PathLine {
                x: 6.2
                y: 0.5
            }
            PathLine {
                x: 5.2
                y: 0.5
            }
        }
    }
}
