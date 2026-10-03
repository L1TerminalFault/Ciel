import QtQuick
import QtQuick.Shapes
import Ciel.Ui

Item {
    id: root

    property real value: 0.5
    property color color: Theme.textSecondary

    implicitWidth: 22
    implicitHeight: 22

    Shape {
        id: speakerBody
        anchors.fill: parent
        preferredRendererType: Shape.GeometryRenderer

        ShapePath {
            fillColor: root.color
            strokeColor: "transparent"
            strokeWidth: 0
            joinStyle: ShapePath.RoundJoin

            startX: 2.0
            startY: 7.8

            PathLine {
                x: 5.2
                y: 7.8
            }
            PathLine {
                x: 9.6
                y: 4.2
            }
            PathLine {
                x: 9.6
                y: 17.8
            }
            PathLine {
                x: 5.2
                y: 14.2
            }
            PathLine {
                x: 2.0
                y: 14.2
            }
            PathLine {
                x: 2.0
                y: 7.8
            }
        }
    }

    Shape {
        id: wavesShape
        anchors.left: parent.left
        anchors.leftMargin: 9.6
        anchors.verticalCenter: parent.verticalCenter
        width: 12
        height: 22
        preferredRendererType: Shape.GeometryRenderer

        ShapePath {
            fillColor: "transparent"
            strokeColor: Qt.rgba(root.color.r, root.color.g, root.color.b, Math.min(1.0, Math.max(0.0, root.value / 0.25)))
            strokeWidth: 1.6
            capStyle: ShapePath.RoundCap

            startX: 0
            startY: 11 - 3.8

            PathAngleArc {
                centerX: 0
                centerY: 11
                radiusX: 3.8
                radiusY: 3.8
                startAngle: -55
                sweepAngle: 110
            }
        }

        ShapePath {
            fillColor: "transparent"
            strokeColor: Qt.rgba(root.color.r, root.color.g, root.color.b, Math.min(1.0, Math.max(0.0, (root.value - 0.25) / 0.35)))
            strokeWidth: 1.6
            capStyle: ShapePath.RoundCap

            startX: 0
            startY: 11 - 7.2

            PathAngleArc {
                centerX: 0
                centerY: 11
                radiusX: 7.2
                radiusY: 7.2
                startAngle: -55
                sweepAngle: 110
            }
        }

        ShapePath {
            fillColor: "transparent"
            strokeColor: Qt.rgba(root.color.r, root.color.g, root.color.b, Math.min(1.0, Math.max(0.0, (root.value - 0.60) / 0.40)))
            strokeWidth: 1.6
            capStyle: ShapePath.RoundCap

            startX: 0
            startY: 11 - 10.6

            PathAngleArc {
                centerX: 0
                centerY: 11
                radiusX: 10.6
                radiusY: 10.6
                startAngle: -55
                sweepAngle: 110
            }
        }
    }
}
