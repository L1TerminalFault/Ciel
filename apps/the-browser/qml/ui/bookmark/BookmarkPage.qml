import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Ciel.Ui
import Ciel.Browser 1.0

Item {
    id: root

    signal openTabRequested(string url)

    property string searchQuery: ""

    ListModel {
        id: bookmarkModel
    }

    function reloadBookmarks() {
        var items = ProfileManager.fetchBookmarks(searchQuery);
        bookmarkModel.clear();
        for (var i = 0; i < items.length; ++i) {
            bookmarkModel.append(items[i]);
        }
    }

    Component.onCompleted: reloadBookmarks()

    onVisibleChanged: {
        if (visible)
            reloadBookmarks();
    }

    Connections {
        target: ProfileManager
        function onBookmarksChanged() {
            if (root.visible && !bookmarkList.busyClosing)
                root.reloadBookmarks();
        }
        function onActiveProfileChanged() {
            root.reloadBookmarks();
        }
    }

    Item {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: 32
        anchors.bottomMargin: 24
        width: Math.min(740, parent.width - 64)

        CielFocusWrapper {
            anchors.fill: parent

            ColumnLayout {
                anchors.fill: parent
                spacing: 18

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 16

                    Text {
                        text: "Bookmarks"
                        font.pixelSize: 22
                        font.weight: Font.DemiBold
                        color: Theme.textPrimary
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    CielSearch {
                        id: searchBar
                        Layout.preferredWidth: 240
                        placeholder: "Search bookmarks…"
                        onTextChanged: {
                            root.searchQuery = text;
                            root.reloadBookmarks();
                        }
                    }
                }

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Item {
                        anchors.centerIn: parent
                        visible: bookmarkModel.count === 0

                        ColumnLayout {
                            anchors.centerIn: parent
                            spacing: 8

                            CielIcon {
                                icon: "bookmark-simple"
                                size: Theme.MEDIUM
                                color: Theme.textSecondary
                                opacity: 0.35
                                Layout.alignment: Qt.AlignHCenter
                            }

                            Text {
                                text: root.searchQuery.length > 0
                                      ? "No matching bookmarks found"
                                      : "No bookmarks yet. Press Ctrl+D on any page."
                                font.pixelSize: 13
                                color: Theme.textSecondary
                                opacity: 0.8
                                Layout.alignment: Qt.AlignHCenter
                            }
                        }
                    }

                    CielScrollView {
                        anchors.fill: parent
                        visible: bookmarkModel.count > 0

                        ListView {
                            id: bookmarkList
                            anchors.fill: parent
                            spacing: 6
                            clip: true
                            model: bookmarkModel

                            property bool busyClosing: false

                            delegate: Item {
                                id: bmDelegate
                                width: bookmarkList.width
                                height: Math.max(0, 61 * closeDimension)

                                readonly property bool hovered: rowMouse.containsMouse

                                property real closeYScale: 1.0
                                property real closeXScale: 1.0
                                property real closeOpacity: 1.0
                                property real closeDimension: 1.0
                                property bool isClosing: false

                                function requestClose() {
                                    if (isClosing)
                                        return;
                                    isClosing = true;
                                    bookmarkList.busyClosing = true;
                                    closeAnimation.restart();
                                }

                                ParallelAnimation {
                                    id: closeAnimation

                                    NumberAnimation {
                                        target: bmDelegate
                                        property: "closeYScale"
                                        to: 0.0
                                        duration: 200
                                        easing.type: Easing.InQuad
                                    }
                                    NumberAnimation {
                                        target: bmDelegate
                                        property: "closeXScale"
                                        to: 0.2
                                        duration: 200
                                        easing.type: Easing.InQuad
                                    }
                                    SequentialAnimation {
                                        PauseAnimation { duration: 45 }
                                        NumberAnimation {
                                            target: bmDelegate
                                            property: "closeDimension"
                                            to: 0.0
                                            duration: 175
                                            easing.type: Easing.InOutQuad
                                        }
                                    }
                                    NumberAnimation {
                                        target: bmDelegate
                                        property: "closeOpacity"
                                        to: 0.0
                                        duration: 180
                                        easing.type: Easing.OutQuad
                                    }

                                    onFinished: {
                                        var targetId = model.id;
                                        bookmarkModel.remove(index);
                                        ProfileManager.deleteBookmark(targetId);
                                        bookmarkList.busyClosing = false;
                                    }
                                }

                                Item {
                                    id: visualContent
                                    anchors.fill: parent
                                    opacity: bmDelegate.isClosing ? bmDelegate.closeOpacity : 1.0

                                    transform: Scale {
                                        origin.x: visualContent.width / 2
                                        origin.y: visualContent.height / 2
                                        xScale: bmDelegate.isClosing ? bmDelegate.closeXScale : 1.0
                                        yScale: bmDelegate.isClosing ? bmDelegate.closeYScale : 1.0
                                    }

                                    CielSquircle {
                                        anchors.fill: parent
                                        color: Theme.surface
                                        borderWidth: 1
                                        borderColor: Theme.border
                                    }

                                    RowLayout {
                                        anchors.fill: parent
                                        anchors.leftMargin: 14
                                        anchors.rightMargin: 14
                                        spacing: 12

                                        CielIcon {
                                            icon: "bookmark-simple"
                                            size: Theme.SMALL
                                            color: bmDelegate.hovered ? Theme.accent : Theme.textSecondary
                                            opacity: bmDelegate.hovered ? 1.0 : 0.6
                                            Layout.alignment: Qt.AlignVCenter

                                            Behavior on color {
                                                ColorAnimation { duration: 120 }
                                            }
                                            Behavior on opacity {
                                                NumberAnimation { duration: 100 }
                                            }
                                        }

                                        ColumnLayout {
                                            Layout.fillWidth: true
                                            spacing: 2
                                            Layout.alignment: Qt.AlignVCenter

                                            Text {
                                                text: model.title || model.url
                                                font.pixelSize: 13
                                                font.weight: Font.Medium
                                                color: Theme.textPrimary
                                                elide: Text.ElideRight
                                                Layout.fillWidth: true
                                            }

                                            Text {
                                                text: model.url
                                                font.pixelSize: 11
                                                color: Theme.textSecondary
                                                elide: Text.ElideRight
                                                Layout.maximumWidth: Math.min(380, bmDelegate.width * 0.55)
                                            }
                                        }

                                        Text {
                                            text: {
                                                var d = new Date(model.timestamp * 1000);
                                                return d.toLocaleDateString(Qt.locale(), Locale.ShortFormat);
                                            }
                                            font.pixelSize: 11
                                            color: Theme.textSecondary
                                            opacity: 0.7
                                            Layout.alignment: Qt.AlignVCenter
                                        }

                                        CielIconButton {
                                            icon: "trash"
                                            Layout.alignment: Qt.AlignVCenter
                                            onClicked: bmDelegate.requestClose()
                                        }
                                    }

                                    MouseArea {
                                        id: rowMouse
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        cursorShape: Qt.PointingHandCursor
                                        z: -1
                                        onClicked: {
                                            if (!bmDelegate.isClosing)
                                                root.openTabRequested(model.url);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
