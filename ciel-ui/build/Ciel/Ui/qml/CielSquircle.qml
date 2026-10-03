import QtQuick
import Ciel.Ui

Item {
    id: root

    property color color: Theme.surface
    property color borderColor: "transparent"
    property real borderWidth: 0.0
    property real radius: Theme.metrics.radiusMd
    property real exponent: Theme.metrics.squircleExponent

    default property alias content: mainLayout.data

    implicitWidth: 100
    implicitHeight: 40

    Behavior on color {
        enabled: Theme.transitionMs > 0
        ColorAnimation {
            duration: Theme.transitionMs
            easing.type: Easing.OutCubic
        }
    }

    Behavior on borderColor {
        enabled: Theme.transitionMs > 0
        ColorAnimation {
            duration: Theme.transitionMs
            easing.type: Easing.OutCubic
        }
    }

    ShaderEffect {
        anchors.fill: parent

        readonly property real itemWidth: Math.max(root.width, 1.0)
        readonly property real itemHeight: Math.max(root.height, 1.0)
        readonly property color surfaceColor: root.color
        readonly property color borderColor: root.borderColor
        readonly property real borderWidth: root.borderWidth
        readonly property real r: root.radius
        readonly property real p: root.exponent
        readonly property real aa: 1.0

        fragmentShader: "qrc:/ciel/ui/shaders/shaders/squircle.frag.qsb"
    }

    Item {
        id: mainLayout
        anchors.fill: parent
    }
}
