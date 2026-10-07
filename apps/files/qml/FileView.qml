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
                                    color: Theme.textPrimary
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
                            onClicked: createFolderPopup.open()
                        }
                        Item {
                            Layout.fillWidth: true
                        }
                        CielIconButton {
                            icon: TabManager.currentSettings.listViewMode ? "squares-four" : "list-dashes"
                            onClicked: TabManager.toggleViewMode()
                        }
                        CielIconButton {
                            id: filterButton
                            icon: "funnel"

                            readonly property int sortDateModified: 0
                            readonly property int sortType: 1
                            readonly property int sortName: 2
                            readonly property int sortSize: 3

                            CielDropDown {
                                id: filterMenu
                                trigger: filterButton
                                placement: "bottom"

                                CielMenuSub {
                                    text: "Sort by"
                                    icon: "arrows-down-up"

                                    CielMenuItem {
                                        text: "Date Modified"
                                        icon: TabManager.currentSettings.sortBy === filterButton.sortDateModified ? "check" : ""
                                        onTriggered: {
                                            TabManager.setSortBy(filterButton.sortDateModified);
                                            filterMenu.close();
                                        }
                                    }

                                    CielMenuItem {
                                        text: "Type"
                                        icon: TabManager.currentSettings.sortBy === filterButton.sortType ? "check" : ""
                                        onTriggered: {
                                            TabManager.setSortBy(filterButton.sortType);
                                            filterMenu.close();
                                        }
                                    }

                                    CielMenuItem {
                                        text: "Name"
                                        icon: TabManager.currentSettings.sortBy === filterButton.sortName ? "check" : ""
                                        onTriggered: {
                                            TabManager.setSortBy(filterButton.sortName);
                                            filterMenu.close();
                                        }
                                    }

                                    CielMenuItem {
                                        text: "Size"
                                        icon: TabManager.currentSettings.sortBy === filterButton.sortSize ? "check" : ""
                                        onTriggered: {
                                            TabManager.setSortBy(filterButton.sortSize);
                                            filterMenu.close();
                                        }
                                    }

                                    CielMenuSeparator {}

                                    CielMenuItem {
                                        text: "Ascending"
                                        icon: TabManager.currentSettings.ascending ? "check" : ""
                                        onTriggered: {
                                            if (!TabManager.currentSettings.ascending)
                                                TabManager.toggleAscending();
                                            filterMenu.close();
                                        }
                                    }

                                    CielMenuItem {
                                        text: "Descending"
                                        icon: !TabManager.currentSettings.ascending ? "check" : ""
                                        onTriggered: {
                                            if (TabManager.currentSettings.ascending)
                                                TabManager.toggleAscending();
                                            filterMenu.close();
                                        }
                                    }
                                }

                                CielMenuSeparator {}

                                CielMenuItem {
                                    text: "Folders First"
                                    icon: TabManager.currentSettings.foldersFirstSorting ? "check" : ""
                                    onTriggered: {
                                        TabManager.toggleFoldersFirst();
                                        filterMenu.close();
                                    }
                                }

                                CielMenuItem {
                                    text: "Show Hidden Files"
                                    icon: TabManager.currentSettings.showHiddenFiles ? "check" : ""
                                    shortcut: "Ctrl+H"
                                    onTriggered: {
                                        TabManager.toggleHiddenFiles();
                                        filterMenu.close();
                                    }
                                }

                                CielMenuItem {
                                    text: "Show Symlinks"
                                    icon: TabManager.currentSettings.showSymlinks ? "check" : ""
                                    onTriggered: {
                                        TabManager.toggleSymlinks();
                                        filterMenu.close();
                                    }
                                }
                            }
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
                            id: viewArea
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            color: Theme.transparent
                            focus: true
                            readonly property int columns: TabManager.currentSettings.listViewMode ? 1 : Math.max(1, Math.floor(width / 130))
                            readonly property int rowsPerPage: Math.max(1, Math.floor(height / (TabManager.currentSettings.listViewMode ? 42 : 120)))
                            readonly property int pageSize: rowsPerPage * columns

                            property string typeAheadBuffer: ""

                            Component.onCompleted: forceActiveFocus()
                            Timer {
                                id: typeAheadTimer
                                interval: 800
                                onTriggered: viewArea.typeAheadBuffer = ""
                            }

                            Keys.onPressed: function (event) {
                                var cur = FileListModel.focusedRow;
                                var count = FileListModel.rowCount();
                                var mods = event.modifiers;

                                switch (event.key) {
                                case Qt.Key_Left:
                                    FileListModel.navigate(cur - 1, mods);
                                    event.accepted = true;
                                    break;
                                case Qt.Key_Right:
                                    FileListModel.navigate(cur + 1, mods);
                                    event.accepted = true;
                                    break;
                                case Qt.Key_Up:
                                    FileListModel.navigate(cur - viewArea.columns, mods);
                                    event.accepted = true;
                                    break;
                                case Qt.Key_Down:
                                    FileListModel.navigate(cur + viewArea.columns, mods);
                                    event.accepted = true;
                                    break;
                                case Qt.Key_Home:
                                    FileListModel.navigate(0, mods);
                                    event.accepted = true;
                                    break;
                                case Qt.Key_End:
                                    FileListModel.navigate(count - 1, mods);
                                    event.accepted = true;
                                    break;
                                case Qt.Key_PageUp:
                                    FileListModel.navigate(cur - viewArea.pageSize, mods);
                                    event.accepted = true;
                                    break;
                                case Qt.Key_PageDown:
                                    FileListModel.navigate(cur + viewArea.pageSize, mods);
                                    event.accepted = true;
                                    break;
                                case Qt.Key_A:
                                    if (event.modifiers & Qt.ControlModifier) {
                                        FileListModel.selectAll();
                                        event.accepted = true;
                                    }
                                    break;
                                case Qt.Key_Escape:
                                    FileListModel.clearSelection();
                                    event.accepted = true;
                                    break;
                                case Qt.Key_Return:
                                case Qt.Key_Enter:
                                    var name = FileListModel.data(FileListModel.index(cur, 0), FileListModel.NameRole);
                                    var isDir = FileListModel.data(FileListModel.index(cur, 0), FileListModel.IsDirRole);
                                    if (isDir) {
                                        TabManager.openFolder(name);
                                    }
                                    event.accepted = true;
                                    break;
                                default:
                                    if (event.text.length > 0 && !event.modifiers) {
                                        viewArea.typeAheadBuffer += event.text.toLowerCase();
                                        typeAheadTimer.restart();
                                        var target = FileListModel.findNextByPrefix(viewArea.typeAheadBuffer);
                                        if (target !== -1) {
                                            FileListModel.navigate(target, 0);
                                        }
                                        event.accepted = true;
                                    }
                                    break;
                                }
                            }
                            Connections {
                                target: FileListModel
                                function onFocusedRowChanged() {
                                    if (loader.item && typeof loader.item.positionViewAtIndex === "function") {
                                        loader.item.positionViewAtIndex(FileListModel.focusedRow, 0);
                                    }
                                }
                            }
                            Loader {
                                id: loader
                                anchors.fill: parent
                                sourceComponent: TabManager.currentSettings.listViewMode ? listViewComponent : gridViewComponent
                            }

                            Component {
                                id: listViewComponent

                                CielListView {
                                    id: fileList
                                    anchors.fill: parent
                                    anchors.topMargin: 4
                                    anchors.bottomMargin: 4
                                    model: FileListModel
                                    TapHandler {
                                        onTapped: function (eventPoint) {
                                            var p = fileList.mapToItem(fileList.contentItem, eventPoint.position.x, eventPoint.position.y);
                                            if (fileList.indexAt(p.x, p.y) === -1) {
                                                FileListModel.clearSelection();
                                                root.forceActiveFocus();
                                            }
                                        }
                                    }
                                    delegate: Item {
                                        id: fileDelegate
                                        width: fileList.width
                                        height: 42

                                        readonly property bool isFocused: FileListModel.focusedRow === index && viewArea.activeFocus
                                        readonly property bool isSelected: model.selected

                                        CielSquircle {
                                            anchors.fill: parent
                                            anchors.leftMargin: 6
                                            anchors.rightMargin: 6
                                            anchors.topMargin: 2
                                            anchors.bottomMargin: 2
                                            borderWidth: 1
                                            borderColor: isFocused ? "#64c5fa" : Theme.transparent

                                            color: isSelected ? "#b4e2fa" : itemHover.hovered ? Theme.background : Theme.surface

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
                                            onClicked: function (mouse) {
                                                viewArea.forceActiveFocus();
                                                FileListModel.handleSelection(index, mouse.modifiers);
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
                                    TapHandler {
                                        onTapped: function (eventPoint) {
                                            var p = fileGrid.mapToItem(fileGrid.contentItem, eventPoint.position.x, eventPoint.position.y);
                                            if (fileGrid.indexAt(p.x, p.y) === -1) {
                                                FileListModel.clearSelection();
                                                root.forceActiveFocus();
                                            }
                                        }
                                    }
                                    delegate: Item {
                                        id: gridDelegate
                                        width: fileGrid.cellWidth
                                        height: fileGrid.cellHeight
                                        readonly property bool isFocused: FileListModel.focusedRow === index && viewArea.activeFocus
                                        readonly property bool isSelected: model.selected

                                        CielSquircle {
                                            anchors.fill: parent
                                            anchors.margins: 6
                                            color: isSelected ? "#b4e2fa" : gridHover.hovered ? Theme.background : Theme.surface
                                            borderWidth: 2
                                            borderColor: isFocused ? "#64c5fa" : "white"

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
                                            onClicked: function (mouse) {
                                                viewArea.forceActiveFocus();
                                                FileListModel.handleSelection(index, mouse.modifiers);
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
        id: createFolderPopup

        contentWidth: 400
        contentHeight: 165
        onOpened: {
            Qt.callLater(function () {
                newFolderNameInput.inputField.forceActiveFocus();
            });
        }

        onClosed: {
            newFolderNameInput.text = "";
        }
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 24
            spacing: 16

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 12

                Text {
                    Layout.fillWidth: true
                    text: "Folder Name"
                    font.pixelSize: 18
                    font.weight: Font.DemiBold
                    color: Theme.textPrimary
                }

                CielSearch {
                    id: newFolderNameInput
                    Layout.fillWidth: true
                    Layout.preferredHeight: 36
                    placeholder: "folderName"
                    showIcons: false
                    isPrimary: true
                    onAccepted: {
                        FileListModel.createFolder();
                        createFolderPopup.close();
                    }
                }
            }
            RowLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignRight
                spacing: 8
                CielButton {
                    text: "Cancel"
                    onClicked: {
                        createFolderPopup.close();
                    }
                }
                CielButton {
                    Layout.alignment: Qt.AlignRight
                    text: "Create"
                    primary: true
                    onClicked: {
                        FileListModel.createFolder(newFolderNameInput.text);
                        createFolderPopup.close();
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
