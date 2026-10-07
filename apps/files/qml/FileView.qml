import QtQuick
import QtQuick.Window
import QtQuick.Controls as T
import QtQuick.Layouts
import Ciel.Ui
import Ciel.Files

Item {
    id: root
    width: parent ? parent.width : 0
    height: parent ? parent.height : 0
    property var breadcrumbModel: []
    property bool editingPath: false
    function updateBreadcrumb() {
        const parts = TabManager.currentPath.split("/").filter(p => p !== "");

        let result = [];
        let path = "";

        // Root
        result.push({
            title: "/",
            path: "/"
        });

        for (const part of parts) {
            path += "/" + part;

            result.push({
                title: part,
                path: path
            });
        }

        breadcrumbModel = result;
    }
    Component.onCompleted: updateBreadcrumb()
    Connections {
        target: TabManager
        function onCurrentPathChanged() {
            updateBreadcrumb();
        }
    }
    property int navigationIconSize: Theme.SMALL
    ListModel {
        id: placesModel

        ListElement {
            name: "Home"
            iconName: "house-simple"
            location: "home"
        }

        ListElement {
            name: "Desktop"
            iconName: "desktop"
            location: "desktop"
        }

        ListElement {
            name: "Documents"
            iconName: "file-text"
            location: "documents"
        }

        ListElement {
            name: "Downloads"
            iconName: "download-simple"
            location: "downloads"
        }

        ListElement {
            name: "Pictures"
            iconName: "images"
            location: "pictures"
        }

        ListElement {
            name: "Music"
            iconName: "music-notes"
            location: "music"
        }

        ListElement {
            name: "Videos"
            iconName: "video-camera"
            location: "videos"
        }

        ListElement {
            name: "Trash"
            iconName: "trash"
            location: "trash"
        }
    }
    Rectangle {
        anchors.fill: parent
        color: Theme.surface
    }

    CielFocusWrapper {
        anchors.fill: parent

        ColumnLayout {
            anchors.fill: parent
            spacing: 0

            RowLayout {
                Layout.fillWidth: true
                Layout.fillHeight: false
                Layout.margins: 13
                Layout.preferredHeight: 36
                spacing: 20

                RowLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: false
                    spacing: 12

                    CielIconButton {
                        icon: "arrow-left"
                        iconSize: root.navigationIconSize
                    }

                    CielIconButton {
                        icon: "arrow-right"
                        iconSize: root.navigationIconSize
                    }

                    CielIconButton {
                        icon: "arrow-up"
                        iconSize: root.navigationIconSize
                        onClicked: TabManager.goUp()
                    }

                    CielIconButton {
                        icon: "arrows-clockwise"
                        iconSize: root.navigationIconSize
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: 8

                    CielSquircle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: Theme.background
                        CielBreadCrumb {
                            visible: !editingPath
                            anchors.fill: parent
                            anchors.margins: 8
                            model: root.breadcrumbModel
                        }
                        TextInput {
                            id: pathInput
                            anchors.fill: parent
                            anchors.margins: 8
                            verticalAlignment: TextInput.AlignVCenter
                            visible: editingPath
                            color: Theme.textPrimary
                            selectByMouse: true
                            text: TabManager.currentPath
                            focus: editingPath
                            onAccepted: {
                                if (TabManager.setCurrentPath(text)) {
                                    root.editingPath = false;
                                } else {
                                    pathErrorPopup.title = "Invalid path";
                                    pathErrorPopup.message = "The specified directory does not exist.";
                                    pathErrorPopup.open();
                                }
                            }

                            onActiveFocusChanged: {
                                if (!activeFocus)
                                    root.editingPath = false;
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            enabled: !editingPath

                            onClicked: {
                                pathInput.text = TabManager.currentPath;
                                root.editingPath = true;
                                pathInput.forceActiveFocus();
                            }
                        }
                    }

                    CielSearch {
                        Layout.fillHeight: true
                        isPrimary: true
                    }
                }
            }

            CielSeparator {}

            RowLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true
                spacing: 0

                ColumnLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: false
                    Layout.preferredWidth: 200
                    Layout.margins: 24
                    Layout.alignment: Qt.AlignTop
                    spacing: 8

                    Text {
                        text: "Quick Access"
                        font.pixelSize: 12
                        font.weight: Font.Medium
                    }

                    ListView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        spacing: 4
                        model: placesModel

                        delegate: CielSquircle {
                            id: delegateRoot

                            function locationPath() {
                                switch (location) {
                                case "home":
                                    return AppPaths.home();
                                case "desktop":
                                    return AppPaths.desktop();
                                case "documents":
                                    return AppPaths.documents();
                                case "downloads":
                                    return AppPaths.downloads();
                                case "pictures":
                                    return AppPaths.pictures();
                                case "music":
                                    return AppPaths.music();
                                case "videos":
                                    return AppPaths.videos();
                                case "trash":
                                    return AppPaths.trash();
                                default:
                                    return "";
                                }
                            }
                            width: ListView.view.width
                            height: 36

                            RowLayout {
                                anchors.fill: parent
                                anchors.topMargin: 2
                                anchors.bottomMargin: 2
                                anchors.leftMargin: 12
                                anchors.rightMargin: 12
                                spacing: 8

                                CielIcon {
                                    icon: iconName
                                    size: 20
                                }

                                Text {
                                    text: name
                                    font.pixelSize: 14
                                    font.weight: Font.Medium
                                }

                                Item {
                                    Layout.fillWidth: true
                                }
                            }

                            HoverHandler {
                                id: hoverHandler
                            }

                            color: hoverHandler.hovered || TabManager.currentPath == delegateRoot.locationPath() ? Theme.background : Theme.surface

                            TapHandler {
                                onTapped: TabManager.setCurrentPath(delegateRoot.locationPath())
                            }
                        }
                    }
                }

                CielSeparator {
                    vertical: true
                }

                ColumnLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    spacing: 0

                    RowLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: false
                        Layout.preferredHeight: 32
                        Layout.leftMargin: 8
                        Layout.rightMargin: 8
                        Layout.topMargin: 4
                        Layout.bottomMargin: 5

                        CielIconButton {
                            icon: "folder-plus"
                        }
                        Item {
                            Layout.fillWidth: true
                        }
                        CielIconButton {
                            icon: TabManager.currentSettings.listViewMode ? "squares-four" : "list-dashes"
                            onClicked: TabManager.toggleViewMode()
                        }
                    }

                    CielSeparator {}

                    ColumnLayout {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.rightMargin: 16
                        Layout.bottomMargin: 16
                        spacing: 0

                        CielSquircle {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            color: Theme.transparent

                            Loader {
                                anchors.fill: parent
                                sourceComponent: TabManager.currentSettings.listViewMode ? listViewComponent : gridViewComponent
                            }

                            Component {
                                id: listViewComponent

                                CielListView {
                                    id: fileList
                                    anchors.fill: parent
                                    model: FileListModel

                                    delegate: Item {
                                        id: fileDelegate
                                        width: fileList.width
                                        height: 42

                                        CielSquircle {
                                            anchors.fill: parent
                                            anchors.leftMargin: 6
                                            anchors.rightMargin: 6
                                            anchors.topMargin: 2
                                            anchors.bottomMargin: 2
                                            color: itemHover.hovered ? Theme.background : Theme.surface

                                            HoverHandler {
                                                id: itemHover
                                            }

                                            RowLayout {
                                                anchors.fill: parent
                                                anchors.leftMargin: 16
                                                anchors.rightMargin: 16
                                                spacing: 12

                                                CielIcon {
                                                    icon: model.isDir ? "folder" : "file"
                                                }

                                                Text {
                                                    text: model.name
                                                    color: Theme.textPrimary
                                                    Layout.fillWidth: true
                                                    elide: Text.ElideRight
                                                }

                                                Text {
                                                    text: model.isDir ? "" : (model.size + " B")
                                                    color: Theme.textSecondary
                                                    Layout.preferredWidth: 80
                                                }

                                                Text {
                                                    text: model.modified
                                                    color: Theme.textSecondary
                                                    Layout.preferredWidth: 120
                                                }
                                            }
                                        }

                                        MouseArea {
                                            anchors.fill: parent
                                            onDoubleClicked: {
                                                if (model.isDir) {
                                                    TabManager.openFolder(model.name);
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            Component {
                                id: gridViewComponent

                                CielGridView {
                                    id: fileGrid
                                    anchors.fill: parent
                                    cellWidth: 130
                                    cellHeight: 120
                                    model: FileListModel

                                    delegate: Item {
                                        id: gridDelegate
                                        width: fileGrid.cellWidth
                                        height: fileGrid.cellHeight

                                        CielSquircle {
                                            anchors.fill: parent
                                            anchors.margins: 4
                                            color: gridHover.hovered ? Theme.background : Theme.surface

                                            HoverHandler {
                                                id: gridHover
                                            }

                                            ColumnLayout {
                                                anchors.fill: parent
                                                anchors.margins: 8
                                                spacing: 8

                                                Item {
                                                    Layout.fillHeight: true
                                                }

                                                FileIcon {
                                                    visible: !model.isDir
                                                    Layout.alignment: Qt.AlignHCenter
                                                }

                                                FolderIcon {
                                                    visible: model.isDir
                                                    Layout.alignment: Qt.AlignHCenter
                                                }

                                                Text {
                                                    text: model.name
                                                    color: Theme.textPrimary
                                                    Layout.fillWidth: true
                                                    horizontalAlignment: Text.AlignHCenter
                                                    elide: Text.ElideMiddle
                                                    maximumLineCount: 2
                                                    wrapMode: Text.WrapAnywhere
                                                }

                                                Item {
                                                    Layout.fillHeight: true
                                                }
                                            }
                                        }

                                        MouseArea {
                                            anchors.fill: parent
                                            onDoubleClicked: {
                                                if (model.isDir) {
                                                    TabManager.openFolder(model.name);
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
    }
    CielPopup {
        id: pathErrorPopup

        property string title: ""
        property string message: ""

        contentWidth: 400
        contentHeight: 180

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 24
            spacing: 16

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 8

                Text {
                    Layout.fillWidth: true
                    text: pathErrorPopup.title
                    font.pixelSize: 18
                    font.weight: Font.DemiBold
                    color: Theme.textPrimary
                }

                Text {
                    Layout.fillWidth: true
                    text: pathErrorPopup.message
                    color: Theme.textSecondary
                    wrapMode: Text.WordWrap
                }
            }

            Item {
                Layout.fillHeight: true
            }

            CielButton {
                Layout.alignment: Qt.AlignRight
                text: "OK"
                onClicked: pathErrorPopup.close()
            }
        }
    }
}
