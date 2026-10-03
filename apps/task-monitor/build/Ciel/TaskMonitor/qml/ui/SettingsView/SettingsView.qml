import QtQuick
import QtQuick.Layouts
import Ciel.Ui

Item {
    id: root

    CielScrollView {
        id: settingsScroll
        anchors.fill: parent
        contentWidth: availableWidth
        contentHeight: contentCol.implicitHeight + 36

        ColumnLayout {
            id: contentCol
            anchors.horizontalCenter: parent.horizontalCenter
            width: Math.min(settingsScroll.availableWidth - 32, 620)
            spacing: 20

            Item {
                Layout.preferredHeight: 6
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4

                Text {
                    text: "Settings"
                    font.pixelSize: 22
                    font.weight: Font.Bold
                    color: Theme.textPrimary
                }

                Text {
                    text: "Adjust how Task Monitor samples and displays system data."
                    font.pixelSize: 13
                    font.weight: Font.Normal
                    color: Theme.textSecondary
                }
            }

            CielSettingsCard {
                title: "General"
                Layout.fillWidth: true

                CielSettingsRow {
                    title: "Appearance"
                    detailText: "Automatic"
                    showChevron: true
                    clickable: true
                    onClicked: console.log("Appearance clicked")
                }

                CielSettingsRow {
                    title: "Default Process Scope"
                    subtitle: "Which processes are shown by default"
                    detailText: "All Processes"
                    showChevron: true
                    clickable: true
                    onClicked: console.log("Scope clicked")
                }

                CielSettingsRow {
                    title: "Group Process Trees"
                    subtitle: "Show child processes nested under their parent app"
                    showDivider: false

                    CielSwitch {
                        checked: true
                    }
                }
            }

            CielSettingsCard {
                title: "Sampling & Performance"
                Layout.fillWidth: true

                CielSettingsRow {
                    title: "Data Refresh Rate"
                    subtitle: "How often CPU, memory, and disk stats update"
                    detailText: "1.0s (Normal)"
                    showChevron: true
                    clickable: true
                    onClicked: console.log("Rate clicked")
                }

                CielSettingsRow {
                    title: "Pause When Minimized"
                    subtitle: "Stop updating stats while the window is hidden, to save power"
                    showDivider: false

                    CielSwitch {
                        checked: true
                    }
                }
            }

            CielSettingsCard {
                title: "Units & Display"
                Layout.fillWidth: true

                CielSettingsRow {
                    title: "Network Data Rate"
                    detailText: "Automatic"
                    showChevron: true
                    clickable: true
                    onClicked: console.log("Net unit clicked")
                }

                CielSettingsRow {
                    title: "Disk Transfer Rate"
                    detailText: "Automatic"
                    showChevron: true
                    clickable: true
                    onClicked: console.log("Disk unit clicked")
                }

                CielSettingsRow {
                    title: "Temperature Scale"
                    detailText: "Celsius (°C)"
                    showChevron: true
                    clickable: true
                    showDivider: false
                    onClicked: console.log("Temp scale clicked")
                }
            }

            Item {
                Layout.preferredHeight: 16
            }
        }
    }
}
