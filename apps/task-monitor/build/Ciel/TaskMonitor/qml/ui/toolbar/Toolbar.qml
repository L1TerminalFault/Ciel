import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Ciel.Ui

Item {
    id: root

    property bool showProcessActions: true

    implicitHeight: 56

    CielFocusWrapper {
        anchors.fill: parent

        RowLayout {
            id: toolbar
            anchors.fill: parent
            anchors.leftMargin: 28
            anchors.rightMargin: 28
            anchors.topMargin: 14
            spacing: 12

            Text {
                text: "Task Monitor"
                font.pixelSize: 15
                font.weight: Font.DemiBold
                color: Theme.textPrimary
            }

            Item {
                Layout.fillWidth: true
            }

            RowLayout {
                id: processActions
                spacing: 12
                visible: opacity > 0.01
                opacity: root.showProcessActions ? 1.0 : 0.0

                Behavior on opacity {
                    NumberAnimation {
                        duration: 150
                        easing.type: Easing.OutCubic
                    }
                }

                CielSearch {
                    placeholder: "Search Processes"
                }

                CielButton {
                    text: "Stop Process"
                }
            }
        }
    }
}
