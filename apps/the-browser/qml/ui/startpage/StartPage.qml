import QtQuick
import QtQuick.Layouts
import QtWebEngine
import Ciel.Ui
import Ciel.Browser 1.0

Item {
    id: root

    required property BrowserConfig config
    required property WebEngineView activeView

    property real entranceProgress: 0.0

    Component.onCompleted: {
        entranceProgress = 1.0;
        searchInput.forceActiveFocus();
    }

    Behavior on entranceProgress {
        CielSpring {
            damping: 0.32
            spring: 5.0
            mass: 1.0
            epsilon: 0.002
        }
    }

    Item {
        id: centerCard
        anchors.centerIn: parent
        width: Math.min(580, parent.width - 48)
        height: layoutContent.implicitHeight

        scale: 0.94 + (root.entranceProgress * 0.06)
        opacity: Math.min(1.0, root.entranceProgress * 1.5)
        y: (1.0 - root.entranceProgress) * 24.0

        ColumnLayout {
            id: layoutContent
            anchors.fill: parent
            spacing: 12

            ColumnLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 6

                CielIcon {
                    Layout.alignment: Qt.AlignHCenter
                    icon: "compass"
                    size: Theme.XLARGE
                    color: Theme.textPrimary
                }

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Search or enter address"
                    font.pixelSize: 20
                    font.weight: Font.DemiBold
                    color: Theme.textPrimary
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: 52

                CielSquircle {
                    anchors.fill: parent
                    color: Theme.surface
                    borderWidth: 1
                    borderColor: Theme.border
                    radius: 12
                }

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 16
                    anchors.rightMargin: 12
                    spacing: 12

                    CielIcon {
                        icon: "magnifying-glass"
                        size: Theme.MEDIUM
                        color: Theme.textSecondary
                        Layout.alignment: Qt.AlignVCenter
                    }

                    TextInput {
                        id: searchInput
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        verticalAlignment: TextInput.AlignVCenter
                        font.pixelSize: 15
                        color: Theme.textPrimary
                        clip: true

                        Text {
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            text: "Type a search query or URL..."
                            font.pixelSize: 15
                            color: Theme.textSecondary
                            visible: !searchInput.text && !searchInput.inputMethodComposing
                        }

                        onAccepted: {
                            if (text.trim().length > 0) {
                                root.activeView.url = root.config.resolveQueryOrUrl(text.trim());
                            }
                        }
                    }

                    CielIconButton {
                        icon: "arrow-right"
                        size: Theme.SMALL
                        visible: searchInput.text.trim().length > 0
                        Layout.alignment: Qt.AlignVCenter
                        onClicked: {
                            if (searchInput.text.trim().length > 0) {
                                root.activeView.url = root.config.resolveQueryOrUrl(searchInput.text.trim());
                            }
                        }
                    }
                }
            }
        }
    }
}
