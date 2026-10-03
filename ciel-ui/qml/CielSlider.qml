import QtQuick
import QtQuick.Layouts
import Ciel.Ui

Item {
    id: root

    property real value: 0.50
    signal sliderMoved(real newValue)

    default property alias iconContent: iconHolder.data

    property real pressOriginValue: 0.50
    property real currentInflate: 0.0

    implicitWidth: 280
    implicitHeight: 42

    transform: Scale {
        origin.x: root.width / 2
        origin.y: root.height / 2
        xScale: 1.0 + (root.currentInflate * 0.063)
        yScale: 1.0 + (root.currentInflate * 0.126)
    }

    SpringAnimation {
        id: snapBackAnim
        target: root
        property: "currentInflate"
        to: 0.0
        spring: 4.6
        damping: 0.28
        epsilon: 0.001
    }

    Rectangle {
        id: track
        anchors.fill: parent
        radius: Theme.metrics.radiusMd
        color: Theme.surface
        border.color: dragArea.containsMouse ? Theme.surfaceHover : Theme.surface
        border.width: 1
        clip: true

        Behavior on border.color {
            ColorAnimation {
                duration: 120
            }
        }

        // Ticks
        Item {
            id: tickContainer
            anchors.fill: parent
            anchors.leftMargin: 20
            anchors.rightMargin: 20

            Repeater {
                model: 9

                delegate: Rectangle {
                    property real fraction: (index + 1) / 10.0
                    x: (tickContainer.width * fraction) - (width / 2)
                    anchors.verticalCenter: parent.verticalCenter
                    width: 1.5
                    height: (index + 1) === 5 ? 8 : 5
                    radius: 0.75
                    color: root.value >= fraction ? Theme.surfaceHover : Theme.surface

                    Behavior on color {
                        ColorAnimation {
                            duration: 80
                        }
                    }
                }
            }
        }

        // Active fill bar
        Rectangle {
            id: fillBar
            x: 0
            y: 0
            height: parent.height
            width: Math.max(parent.height * 0.5, parent.width * root.value)
            color: Theme.surfaceHover

            topLeftRadius: Theme.metrics.radiusMd
            bottomLeftRadius: Theme.metrics.radiusMd
            topRightRadius: 6
            bottomRightRadius: 6

            Rectangle {
                id: thumbNotch
                anchors.right: parent.right
                anchors.rightMargin: 12
                anchors.verticalCenter: parent.verticalCenter
                width: 2.0
                height: 14
                radius: 1.0
                color: Theme.textSecondary
            }
        }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 12
            anchors.rightMargin: 14
            spacing: 10

            Item {
                id: iconHolder
                Layout.preferredWidth: 22
                Layout.preferredHeight: 22
            }

            Item {
                Layout.fillWidth: true
            }

            Text {
                text: Math.round(root.value * 100) + "%"
                color: dragArea.pressed ? Theme.textPrimary : Theme.textSecondary
                font.pixelSize: 12
                font.weight: Font.Medium
                renderType: Text.NativeRendering

                Behavior on color {
                    ColorAnimation {
                        duration: 120
                    }
                }
            }
        }

        MouseArea {
            id: dragArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor

            function updateFromMouse(mouseX) {
                var fraction = Math.max(0.0, Math.min(1.0, mouseX / root.width));
                root.value = fraction;
                root.sliderMoved(fraction);
            }

            onPressed: mouse => {
                snapBackAnim.stop();
                root.pressOriginValue = root.value;
                updateFromMouse(mouse.x);
                root.currentInflate = root.value - root.pressOriginValue;
            }

            onPositionChanged: mouse => {
                if (pressed) {
                    updateFromMouse(mouse.x);
                    root.currentInflate = root.value - root.pressOriginValue;
                }
            }

            onReleased: {
                snapBackAnim.restart();
            }
        }
    }
}
