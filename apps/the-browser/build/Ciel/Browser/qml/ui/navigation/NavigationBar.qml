import QtQuick
import QtQuick.Layouts
import Ciel.Ui
import Ciel.Browser 1.0

Item {
    id: root

    property var activeView: null
    required property BrowserConfig config

    implicitHeight: 44

    CielFocusWrapper {
        anchors.fill: parent

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 12
            anchors.rightMargin: 12
            spacing: 8

            CielIconButton {
                icon: "arrow-left"
                size: Theme.SMALL
                enabled: root.activeView ? root.activeView.canGoBack : false
                opacity: enabled ? 1.0 : 0.35
                onClicked: if (root.activeView)
                    root.activeView.goBack()
            }

            CielIconButton {
                icon: "arrow-right"
                size: Theme.SMALL
                enabled: root.activeView ? root.activeView.canGoForward : false
                opacity: enabled ? 1.0 : 0.35
                onClicked: if (root.activeView)
                    root.activeView.goForward()
            }

            CielIconButton {
                icon: (root.activeView && root.activeView.loading) ? "x" : "arrow-clockwise"
                size: Theme.SMALL
                onClicked: {
                    if (!root.activeView)
                        return;
                    if (root.activeView.loading) {
                        root.activeView.stop();
                    } else {
                        root.activeView.reload();
                    }
                }
            }

            Item {
                Layout.fillWidth: true
                height: 36

                CielSearch {
                    id: omnibox
                    anchors.fill: parent
                    placeholder: "Search or enter address"

                    text: (root.activeView && root.activeView.url.toString() !== "about:blank") ? root.activeView.url.toString() : ""

                    onAccepted: query => {
                        let destination = root.config.resolveQueryOrUrl(query);
                        if (root.activeView) {
                            root.activeView.url = destination;
                        }
                        inputField.focus = false;
                    }
                }
            }
        }
    }
}
