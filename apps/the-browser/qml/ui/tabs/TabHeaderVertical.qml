import QtQuick
import QtQuick.Layouts
import Ciel.Ui
import Ciel.Browser 1.0

Item {
    id: root

    property bool collapsed: false
    property var activeView: null
    required property BrowserConfig config
    signal collapseToggled

    clip: true
    implicitHeight: collapsed ? 116 : 94

    property real bloomProgress: collapsed ? 1.0 : 0.0

    function cleanDisplayUrl(raw) {
        if (!raw || raw === "about:blank" || raw === "")
            return "";
        if (raw.startsWith("ciel://") || raw.startsWith("about:"))
            return raw;
        var s = raw.replace(/^[a-zA-Z]+:\/\//, "");
        s = s.replace(/^www\./, "");
        var slash = s.indexOf("/");
        return slash !== -1 ? s.substring(0, slash) : s;
    }

    Rectangle {
        anchors.fill: parent
        color: Theme.background
        z: -1
    }

    Behavior on implicitHeight {
        CielSpring {
            damping: 3.0
            spring: 10.2
            mass: 3.2
            epsilon: 0.002
        }
    }

    Behavior on bloomProgress {
        CielSpring {
            damping: 4.5
            spring: 6.0
            mass: 3.5
            epsilon: 0.005
        }
    }

    TextEdit {
        id: clipHelper
        visible: false
    }

    Item {
        id: bloomExpandedDeck
        anchors.fill: parent
        visible: opacity > 0.0
        opacity: Math.max(0.0, 1.0 - root.bloomProgress * 2.0)
        scale: 1.0 - (root.bloomProgress * 0.10)
        transformOrigin: Item.Center

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 6
            spacing: 6

            RowLayout {
                Layout.fillWidth: true
                spacing: 4

                CielIconButton {
                    icon: "arrow-left"
                    size: Theme.SMALL
                    opacity: root.activeView && root.activeView.canGoBack ? 1.0 : 0.35
                    onClicked: {
                        if (root.activeView && root.activeView.canGoBack)
                            root.activeView.goBack();
                    }
                }

                CielIconButton {
                    icon: "arrow-right"
                    size: Theme.SMALL
                    opacity: root.activeView && root.activeView.canGoForward ? 1.0 : 0.35
                    onClicked: {
                        if (root.activeView && root.activeView.canGoForward)
                            root.activeView.goForward();
                    }
                }

                CielIconButton {
                    icon: root.activeView && root.activeView.loading ? "x" : "arrow-clockwise"
                    size: Theme.SMALL
                    onClicked: {
                        if (!root.activeView)
                            return;
                        if (root.activeView.loading)
                            root.activeView.stop();
                        else
                            root.activeView.reload();
                    }
                }

                Item {
                    Layout.fillWidth: true
                }

                CielIconButton {
                    icon: "sidebar-simple"
                    size: Theme.SMALL
                    onClicked: root.collapseToggled()
                }
            }

            CielSearch {
                id: omniboxVertical
                Layout.fillWidth: true
                Layout.preferredHeight: 32
                placeholder: "Search or enter URL"

                readonly property string fullUrlStr: (root.activeView && root.activeView.url) ? root.activeView.url.toString() : ""

                text: {
                    if (omniboxVertical.isInputActive) {
                        return fullUrlStr === "about:blank" ? "" : fullUrlStr;
                    }
                    return root.cleanDisplayUrl(fullUrlStr);
                }

                onAccepted: query => {
                    if (root.activeView) {
                        root.activeView.url = root.config.resolveQueryOrUrl(query);
                        omniboxVertical.inputField.focus = false;
                    }
                }

                preContent: CielIconButton {
                    id: linkBtnVertical
                    property bool copied: false
                    icon: copied ? "check" : "link"
                    size: Theme.XSMALL
                    iconColor: copied ? Theme.accent : Theme.textSecondary

                    Timer {
                        id: copyResetTimerVertical
                        interval: 1500
                        onTriggered: linkBtnVertical.copied = false
                    }

                    onClicked: {
                        if (root.activeView && root.activeView.url) {
                            clipHelper.text = root.activeView.url.toString();
                            clipHelper.selectAll();
                            clipHelper.copy();
                            linkBtnVertical.copied = true;
                            copyResetTimerVertical.restart();
                        }
                    }
                }

                postContent: RowLayout {
                    spacing: 2

                    CielIconButton {
                        id: starBtn
                        property bool bookmarked: (root.activeView && root.activeView.url) ? ProfileManager.isBookmarked(root.activeView.url.toString()) : false
                        icon: bookmarked ? "star-fill" : "star"
                        size: Theme.XSMALL
                        iconColor: bookmarked ? Theme.accent : Theme.textSecondary

                        Connections {
                            target: ProfileManager
                            function onBookmarksChanged() {
                                if (root.activeView && root.activeView.url) {
                                    starBtn.bookmarked = ProfileManager.isBookmarked(root.activeView.url.toString());
                                }
                            }
                        }

                        onClicked: {
                            if (root.activeView && root.activeView.url) {
                                var u = root.activeView.url.toString();
                                var t = root.activeView.title ? root.activeView.title : u;
                                starBtn.bookmarked = ProfileManager.toggleBookmark(u, t);
                            }
                        }
                    }

                    CielIconButton {
                        icon: "faders-horizontal"
                        size: Theme.XSMALL
                    }
                }
            }
        }
    }

    Item {
        id: bloomCollapsedSpine
        anchors.fill: parent
        visible: opacity > 0.0
        opacity: Math.max(0.0, (root.bloomProgress - 0.3) * 1.42)
        scale: 0.90 + (root.bloomProgress * 0.10)
        transformOrigin: Item.Center

        ColumnLayout {
            anchors.fill: parent
            anchors.topMargin: 6
            anchors.bottomMargin: 6
            spacing: 4

            CielIconButton {
                Layout.alignment: Qt.AlignHCenter
                icon: "sidebar-simple"
                size: Theme.SMALL
                onClicked: root.collapseToggled()
            }

            CielIconButton {
                Layout.alignment: Qt.AlignHCenter
                icon: "magnifying-glass"
                size: Theme.SMALL
                onClicked: root.collapseToggled()
            }

            CielIconButton {
                Layout.alignment: Qt.AlignHCenter
                icon: root.activeView && root.activeView.loading ? "x" : "arrow-clockwise"
                size: Theme.SMALL
                onClicked: {
                    if (!root.activeView)
                        return;
                    if (root.activeView.loading)
                        root.activeView.stop();
                    else
                        root.activeView.reload();
                }
            }
        }
    }
}
