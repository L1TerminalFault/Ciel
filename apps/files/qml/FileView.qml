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

    property int navigationIconSize: Theme.SMALL

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
                            anchors.fill: parent
                            anchors.margins: 8
                            model: [
                                {
                                    title: "something"
                                },
                                {
                                    title: "some"
                                }
                            ]
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
                    Layout.preferredWidth: 300
                    Layout.margins: 8

                    Text {
                        text: "sidebar"
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
}
