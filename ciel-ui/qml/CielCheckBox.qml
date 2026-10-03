import QtQuick
import QtQuick.Layouts
import Ciel.Ui

Item {
    id: root

    property string text: ""
    property string description: ""
    property bool checked: false
    property bool enabled: true

    signal toggled(bool checked)

    implicitWidth: rowLayout.implicitWidth
    implicitHeight: Math.max(24, rowLayout.implicitHeight)

    readonly property bool hovered: mouseArea.containsMouse && root.enabled

    RowLayout {
        id: rowLayout
        anchors.fill: parent
        spacing: 12

        Item {
            id: boxContainer
            Layout.preferredWidth: 20
            Layout.preferredHeight: 20
            Layout.alignment: Qt.AlignTop

            property real boxScale: 1.0

            Behavior on boxScale {
                CielSpring {
                    damping: 0.32
                    spring: 6.5
                    mass: 0.8
                    epsilon: 0.001
                }
            }

            CielSquircle {
                id: boxBg
                anchors.fill: parent
                color: root.checked ? Theme.accent : "transparent"
                borderWidth: 1
                borderColor: root.checked ? Theme.accent : Theme.border
                scale: boxContainer.boxScale

                Behavior on color {
                    ColorAnimation {
                        duration: 120
                    }
                }

                Behavior on borderColor {
                    ColorAnimation {
                        duration: 120
                    }
                }

                CielIcon {
                    anchors.centerIn: parent
                    icon: "check"
                    size: Theme.XSMALL
                    color: "#FFFFFF"
                    visible: root.checked
                }
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 2
            Layout.alignment: Qt.AlignVCenter

            Text {
                text: root.text
                font.pixelSize: 13
                font.weight: Font.Medium
                color: root.enabled ? Theme.textPrimary : Theme.textSecondary
                elide: Text.ElideRight
                Layout.fillWidth: true
            }

            Text {
                text: root.description
                font.pixelSize: 11
                color: Theme.textSecondary
                visible: root.description.length > 0
                wrapMode: Text.Wrap
                Layout.fillWidth: true
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: root.enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
        onPressed: {
            if (root.enabled) {
                boxContainer.boxScale = 0.86;
            }
        }
        onReleased: {
            boxContainer.boxScale = 1.0;
        }
        onCanceled: {
            boxContainer.boxScale = 1.0;
        }
        onClicked: {
            if (root.enabled) {
                root.checked = !root.checked;
                root.toggled(root.checked);
            }
        }
    }
}
