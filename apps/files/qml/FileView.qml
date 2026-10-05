import QtQuick
import QtQuick.Window
import QtQuick.Controls
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

                            ListView {
                                id: fileList
                                anchors.fill: parent
                                clip: true

                                model: FileListModel

                                delegate: Item {
                                    id: fileDelegate
                                    width: fileList.width
                                    height: 36

                                    Rectangle {
                                        anchors.fill: parent
                                        color: itemHover.hovered ? Theme.background : "transparent"

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
                                                // Directory sizes are not computed recursively during listing
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
                                                var separator = TabManager.currentPath.endsWith("/") ? "" : "/";
                                                TabManager.setCurrentPath(TabManager.currentPath + separator + model.name);
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
