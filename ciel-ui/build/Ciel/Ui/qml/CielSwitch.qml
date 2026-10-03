import QtQuick
import Ciel.Ui

Item {
    id: root

    property bool checked: false
    signal toggled(bool checked)

    implicitWidth: 42
    implicitHeight: 24

    readonly property real paddingSize: 2.0
    readonly property real thumbDiameter: height - (paddingSize * 2)

    readonly property real minX: root.paddingSize
    readonly property real maxX: track.width - root.thumbDiameter - root.paddingSize

    Rectangle {
        id: track
        anchors.fill: parent
        radius: height / 2

        color: {
            if (!root.enabled) {
                return Theme.surface;
            }
            if (root.checked) {
                return mouseArea.containsMouse ? Qt.lighter(Theme.accent, 1.06) : Theme.accent;
            }
            return mouseArea.containsMouse ? Qt.lighter(Theme.surface, 1.15) : Theme.surface;
        }

        border.width: 1
        border.color: root.checked ? Qt.darker(Theme.accent, 1.1) : Theme.border

        Behavior on color {
            ColorAnimation {
                duration: 150
                easing.type: Easing.OutQuad
            }
        }

        Behavior on border.color {
            ColorAnimation {
                duration: 150
                easing.type: Easing.OutQuad
            }
        }

        Rectangle {
            id: thumb
            height: root.thumbDiameter
            radius: height / 2
            anchors.verticalCenter: parent.verticalCenter

            width: mouseArea.pressed ? (root.thumbDiameter + 3) : root.thumbDiameter

            x: root.checked ? (root.maxX - (mouseArea.pressed ? 3 : 0)) : root.minX

            color: root.checked ? "#FFFFFF" : Theme.textPrimary

            Behavior on x {
                CielSpring {
                    damping: 0.38
                    spring: 4.2
                    mass: 1.0
                    epsilon: 0.25
                }
            }

            Behavior on width {
                CielSpring {
                    damping: 0.40
                    spring: 4.5
                    mass: 1.0
                    epsilon: 0.25
                }
            }

            Behavior on color {
                ColorAnimation {
                    duration: 150
                    easing.type: Easing.OutQuad
                }
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: root.enabled ? Qt.PointingHandCursor : Qt.ArrowCursor

        onClicked: {
            if (!root.enabled)
                return;
            root.checked = !root.checked;
            root.toggled(root.checked);
        }
    }
}
