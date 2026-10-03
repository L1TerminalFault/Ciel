import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Ciel.Ui

ApplicationWindow {
    id: window
    width: 620
    height: 420
    visible: true
    title: "Ciel Workspace"

    // Coordinated background crossfade via D-Bus Theme.transitionMs
    color: Theme.background
    Behavior on color {
        enabled: Theme.transitionMs > 0
        ColorAnimation {
            duration: Theme.transitionMs
            easing.type: Easing.OutCubic
        }
    }

    ColumnLayout {
        anchors.centerIn: parent
        spacing: Theme.metrics.spacingLg

        // Header Card with G2 Curvature & AppPaths verification
        CielSurface {
            Layout.preferredWidth: 460
            Layout.preferredHeight: 110
            interactive: false
            radius: Theme.metrics.radiusLg

            ColumnLayout {
                anchors.centerIn: parent
                spacing: Theme.metrics.spacingSm

                Text {
                    text: "Ciel System State"
                    font.pixelSize: 18
                    font.weight: Font.Bold
                    color: Theme.textPrimary
                    Layout.alignment: Qt.AlignHCenter
                    Behavior on color {
                        ColorAnimation {
                            duration: Theme.transitionMs
                        }
                    }
                }

                Text {
                    text: "Config: " + AppPaths.configDir("shell")
                    font.family: "monospace"
                    font.pixelSize: 11
                    color: Theme.textSecondary
                    Layout.alignment: Qt.AlignHCenter
                    Behavior on color {
                        ColorAnimation {
                            duration: Theme.transitionMs
                        }
                    }
                }

                Text {
                    text: "Active Accent: " + Theme.accent
                    font.family: "monospace"
                    font.pixelSize: 11
                    color: Theme.accent
                    Layout.alignment: Qt.AlignHCenter
                    Behavior on color {
                        ColorAnimation {
                            duration: Theme.transitionMs
                        }
                    }
                }
            }
        }

        // Live Action Controls (Calls daemon methods over D-Bus IPC)
        RowLayout {
            spacing: Theme.metrics.spacingMd
            Layout.alignment: Qt.AlignHCenter

            CielButton {
                text: Theme.isDark ? "Light Mode" : "Dark Mode"
                primary: true
                onClicked: Theme.toggleMode()
            }

            CielButton {
                text: "Blue"
                onClicked: Theme.setAccent("#0A84FF")
            }

            CielButton {
                text: "Purple"
                onClicked: Theme.setAccent("#AF52DE")
            }

            CielButton {
                text: "Emerald"
                onClicked: Theme.setAccent("#34C759")
            }
        }

        // Interactive spring test surface
        CielSurface {
            Layout.preferredWidth: 460
            Layout.preferredHeight: 50
            radius: Theme.metrics.radiusMd
            Layout.alignment: Qt.AlignHCenter

            Text {
                anchors.centerIn: parent
                text: "Interactive Spring Surface"
                font.pixelSize: 13
                font.weight: Font.Medium
                color: Theme.textPrimary
                Behavior on color {
                    ColorAnimation {
                        duration: Theme.transitionMs
                    }
                }
            }
        }
    }
}
