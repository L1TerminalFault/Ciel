import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import Ciel.Ui
import Ciel.Files

ApplicationWindow {
    id: window
    visible: true

    width: 2040
    height: 820

    title: "Files"
    color: Theme.background

    Connections {
        target: TabManager
        function onCurrentPathChanged() {
            if (TabManager.currentPath) {
                FileListModel.setPath(TabManager.currentPath);
            }
        }
    }

    Component.onCompleted: {
        TabManager.addTab("/");
        FileListModel.setPath(TabManager.currentPath);
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: -2

        CielTabView {
            Layout.fillWidth: true
            Layout.preferredHeight: 36

            model: TabManager
            currentTabId: TabManager.currentTabId

            onTabSelected: id => {
                TabManager.currentTabId = id;
            }

            onTabCloseRequested: id => {
                TabManager.closeTab(id);
            }
            onTabAddRequested: {
                TabManager.addTab(TabManager.currentPath || "/");
            }
        }
        FileView {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
