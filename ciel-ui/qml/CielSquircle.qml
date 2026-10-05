import QtQuick
import Ciel.Ui

Item {
    id: root

    property color color: Theme.surface
    property color borderColor: "transparent"
    property real borderWidth: 0.0
    property real radius: Theme.metrics.radiusMd
    property real exponent: Theme.metrics.squircleExponent

    property bool roundTopLeft: true
    property bool roundTopRight: true
    property bool roundBottomLeft: true
    property bool roundBottomRight: true

    property bool topOnly: false
    property bool bottomOnly: false
    property bool leftOnly: false
    property bool rightOnly: false

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

        readonly property vector4d cornerControl: Qt.vector4d((root.topOnly || root.leftOnly || (!root.bottomOnly && !root.rightOnly && root.roundTopLeft)) ? 1.0 : 0.0, (root.topOnly || root.rightOnly || (!root.bottomOnly && !root.leftOnly && root.roundTopRight)) ? 1.0 : 0.0, (root.bottomOnly || root.leftOnly || (!root.topOnly && !root.rightOnly && root.roundBottomLeft)) ? 1.0 : 0.0, (root.bottomOnly || root.rightOnly || (!root.topOnly && !root.leftOnly && root.roundBottomRight)) ? 1.0 : 0.0)

        fragmentShader: "qrc:/ciel/ui/shaders/shaders/squircle.frag.qsb"
    }

    Item {
        id: mainLayout
        anchors.fill: parent
    }
}
