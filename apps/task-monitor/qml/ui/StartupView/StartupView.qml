import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Ciel.Ui
import Ciel.TaskMonitor 1.0

Item {
    id: root

    StartupAppsModel {
        id: userAppsModel
        scopeFilter: StartupAppsModel.UserScope
    }

    StartupAppsModel {
        id: systemAppsModel
        scopeFilter: StartupAppsModel.SystemScope
    }

    CielScrollView {
        id: mainScroll
        anchors.fill: parent
        contentWidth: availableWidth
        contentHeight: scrollContent.implicitHeight + 36

        ColumnLayout {
            id: scrollContent
            width: mainScroll.availableWidth
            spacing: 24

            Item {
                Layout.preferredHeight: 2
            }

            RowLayout {
                Layout.fillWidth: true

                ColumnLayout {
                    spacing: 2

                    Text {
                        text: "Login Items"
                        font.pixelSize: 18
                        font.weight: Font.Bold
                        color: Theme.textPrimary
                    }

                    Text {
                        text: "Manage applications and background tasks that launch automatically"
                        font.pixelSize: 12
                        color: Theme.textSecondary
                    }
                }

                Item {
                    Layout.fillWidth: true
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 8

                Text {
                    text: "Open at Login"
                    font.pixelSize: 13
                    font.weight: Font.DemiBold
                    color: Theme.textPrimary
                    Layout.leftMargin: 2
                }

                CielSquircle {
                    Layout.fillWidth: true
                    implicitHeight: userCol.implicitHeight + 2
                    Layout.preferredHeight: implicitHeight
                    color: Theme.surface
                    borderWidth: 1
                    borderColor: Theme.border

                    ColumnLayout {
                        id: userCol
                        anchors.fill: parent
                        anchors.margins: 1
                        spacing: 0

                        Repeater {
                            model: userAppsModel

                            delegate: Item {
                                id: userDelegate
                                required property int index
                                required property string appName
                                required property string appComment
                                required property string appExec
                                required property string appIcon
                                required property bool appEnabled

                                Layout.fillWidth: true
                                height: 54

                                Rectangle {
                                    anchors.fill: parent
                                    anchors.bottomMargin: userDelegate.index < userAppsModel.count - 1 ? 1 : 0
                                    color: Theme.background
                                    opacity: userMouse.containsMouse ? 0.6 : 0.0

                                    Behavior on opacity {
                                        NumberAnimation {
                                            duration: 80
                                            easing.type: Easing.OutQuad
                                        }
                                    }
                                }

                                MouseArea {
                                    id: userMouse
                                    anchors.fill: parent
                                    hoverEnabled: true
                                }

                                RowLayout {
                                    anchors.fill: parent
                                    anchors.leftMargin: 14
                                    anchors.rightMargin: 14
                                    spacing: 12

                                    CielSquircle {
                                        Layout.preferredWidth: 32
                                        Layout.preferredHeight: 32
                                        color: Theme.background
                                        borderWidth: 1
                                        borderColor: Theme.border

                                        CielIcon {
                                            anchors.centerIn: parent
                                            icon: "app-window"
                                            size: 18
                                            color: Theme.textPrimary
                                        }
                                    }

                                    ColumnLayout {
                                        Layout.fillWidth: true
                                        spacing: 2
                                        Layout.alignment: Qt.AlignVCenter

                                        Text {
                                            text: userDelegate.appName
                                            font.pixelSize: 13
                                            font.weight: Font.Medium
                                            color: Theme.textPrimary
                                            elide: Text.ElideRight
                                            Layout.fillWidth: true
                                        }

                                        Text {
                                            text: userDelegate.appComment.length > 0 ? userDelegate.appComment : userDelegate.appExec
                                            font.pixelSize: 11
                                            color: Theme.textSecondary
                                            elide: Text.ElideRight
                                            Layout.fillWidth: true
                                        }
                                    }

                                    CielSwitch {
                                        checked: userDelegate.appEnabled
                                        onToggled: userAppsModel.toggleEnabled(userDelegate.index)
                                    }
                                }

                                Rectangle {
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    anchors.bottom: parent.bottom
                                    height: 1
                                    color: Theme.border
                                    visible: userDelegate.index < userAppsModel.count - 1
                                }
                            }
                        }

                        Item {
                            Layout.fillWidth: true
                            height: 48
                            visible: userAppsModel.count === 0

                            Text {
                                anchors.centerIn: parent
                                text: "No user-specific startup applications"
                                font.pixelSize: 12
                                color: Theme.textSecondary
                            }
                        }
                    }
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 8

                Text {
                    text: "Allow in the Background"
                    font.pixelSize: 13
                    font.weight: Font.DemiBold
                    color: Theme.textPrimary
                    Layout.leftMargin: 2
                }

                CielSquircle {
                    Layout.fillWidth: true
                    implicitHeight: sysCol.implicitHeight + 2
                    Layout.preferredHeight: implicitHeight
                    color: Theme.surface
                    borderWidth: 1
                    borderColor: Theme.border

                    ColumnLayout {
                        id: sysCol
                        anchors.fill: parent
                        anchors.margins: 1
                        spacing: 0

                        Repeater {
                            model: systemAppsModel

                            delegate: Item {
                                id: sysDelegate
                                required property int index
                                required property string appName
                                required property string appComment
                                required property string appExec
                                required property string appIcon
                                required property bool appEnabled

                                Layout.fillWidth: true
                                height: 54

                                Rectangle {
                                    anchors.fill: parent
                                    anchors.bottomMargin: sysDelegate.index < systemAppsModel.count - 1 ? 1 : 0
                                    color: Theme.background
                                    opacity: sysMouse.containsMouse ? 0.6 : 0.0

                                    Behavior on opacity {
                                        NumberAnimation {
                                            duration: 80
                                            easing.type: Easing.OutQuad
                                        }
                                    }
                                }

                                MouseArea {
                                    id: sysMouse
                                    anchors.fill: parent
                                    hoverEnabled: true
                                }

                                RowLayout {
                                    anchors.fill: parent
                                    anchors.leftMargin: 14
                                    anchors.rightMargin: 14
                                    spacing: 12

                                    CielSquircle {
                                        Layout.preferredWidth: 32
                                        Layout.preferredHeight: 32
                                        color: Theme.background
                                        borderWidth: 1
                                        borderColor: Theme.border

                                        CielIcon {
                                            anchors.centerIn: parent
                                            icon: "app-window"
                                            size: 18
                                            color: Theme.textPrimary
                                        }
                                    }

                                    ColumnLayout {
                                        Layout.fillWidth: true
                                        spacing: 2
                                        Layout.alignment: Qt.AlignVCenter

                                        Text {
                                            text: sysDelegate.appName
                                            font.pixelSize: 13
                                            font.weight: Font.Medium
                                            color: Theme.textPrimary
                                            elide: Text.ElideRight
                                            Layout.fillWidth: true
                                        }

                                        Text {
                                            text: sysDelegate.appComment.length > 0 ? sysDelegate.appComment : sysDelegate.appExec
                                            font.pixelSize: 11
                                            color: Theme.textSecondary
                                            elide: Text.ElideRight
                                            Layout.fillWidth: true
                                        }
                                    }

                                    CielSwitch {
                                        checked: sysDelegate.appEnabled
                                        onToggled: systemAppsModel.toggleEnabled(sysDelegate.index)
                                    }
                                }

                                Rectangle {
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    anchors.bottom: parent.bottom
                                    height: 1
                                    color: Theme.border
                                    visible: sysDelegate.index < systemAppsModel.count - 1
                                }
                            }
                        }

                        Item {
                            Layout.fillWidth: true
                            height: 48
                            visible: systemAppsModel.count === 0

                            Text {
                                anchors.centerIn: parent
                                text: "No system background tasks found"
                                font.pixelSize: 12
                                color: Theme.textSecondary
                            }
                        }
                    }
                }
            }

            Item {
                Layout.preferredHeight: 12
            }
        }
    }
}
