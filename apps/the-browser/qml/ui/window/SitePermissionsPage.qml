import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Ciel.Ui
import Ciel.Browser 1.0

Item {
    id: root

    property string searchQuery: ""
    property string stateFilter: ""      

    function featureName(k) {
        return ({ microphone: "Microphone", camera: "Camera",
                  location: "Location", notifications: "Notifications" })[k] || k
    }
    function featureIcon(k) {
        return ({ microphone: "microphone", camera: "camera",
                  location: "map-pin", notifications: "bell" })[k] || "shield-check"
    }

    ListModel { id: permModel }

    function reloadPermissions() {
        var items = SitePermissionManager.list(searchQuery, stateFilter)
        permModel.clear()
        for (var i = 0; i < items.length; ++i)
            permModel.append(items[i])
    }

    Component.onCompleted: reloadPermissions()
    onVisibleChanged: if (visible) reloadPermissions()
    onStateFilterChanged: reloadPermissions()

    Connections {
        target: SitePermissionManager
        function onPermissionsChanged() {
            if (root.visible && !permList.busyClosing)
                root.reloadPermissions()
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
                        text: "Site Permissions"
                        font.pixelSize: 22
                        font.weight: Font.DemiBold
                        color: Theme.textPrimary
                    }

                    Item { Layout.fillWidth: true }

                    CielButton {
                        text: "Reset all"
                        enabled: permModel.count > 0 || root.searchQuery !== "" || root.stateFilter !== ""
                        onClicked: resetPopup.open()
                    }

                    CielSearch {
                        Layout.preferredWidth: 220
                        placeholder: "Search sites…"
                        onTextChanged: {
                            root.searchQuery = text
                            root.reloadPermissions()
                        }
                    }
                }

                RowLayout {
                    spacing: 8

                    CielButton {
                        text: "All"
                        primary: root.stateFilter === ""
                        onClicked: root.stateFilter = ""
                    }
                    CielButton {
                        text: "Ask"
                        primary: root.stateFilter === "ask"
                        onClicked: root.stateFilter = "ask"
                    }
                    CielButton {
                        text: "Granted"
                        primary: root.stateFilter === "granted"
                        onClicked: root.stateFilter = "granted"
                    }
                    CielButton {
                        text: "Denied"
                        primary: root.stateFilter === "denied"
                        onClicked: root.stateFilter = "denied"
                    }
                }

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Item {
                        anchors.centerIn: parent
                        visible: permModel.count === 0

                        ColumnLayout {
                            anchors.centerIn: parent
                            spacing: 8

                            CielIcon {
                                icon: "shield-check"
                                size: Theme.MEDIUM
                                color: Theme.textSecondary
                                opacity: 0.35
                                Layout.alignment: Qt.AlignHCenter
                            }

                            Text {
                                text: (root.searchQuery !== "" || root.stateFilter !== "")
                                      ? "No matching permissions"
                                      : "No saved permissions yet. Sites you allow or block will appear here."
                                font.pixelSize: 13
                                color: Theme.textSecondary
                                opacity: 0.8
                                Layout.alignment: Qt.AlignHCenter
                            }
                        }
                    }

                    CielScrollView {
                        anchors.fill: parent
                        visible: permModel.count > 0

                        ListView {
                            id: permList
                            anchors.fill: parent
                            spacing: 6
                            clip: true
                            model: permModel

                            property bool busyClosing: false

                            delegate: Item {
                                id: permDelegate
                                width: permList.width
                                height: Math.max(0, 61 * closeDimension)

                                readonly property string decision: model.decision
                                readonly property bool granted: decision === "granted"
                                readonly property color tone: decision === "granted" ? Theme.accent
                                                            : (decision === "denied" ? "#EF4444" : Theme.textSecondary)

                                property real closeScale: 1.0
                                property real closeOpacity: 1.0
                                property real closeDimension: 1.0
                                property bool isClosing: false

                                function requestClose() {
                                    if (isClosing)
                                        return
                                    isClosing = true
                                    permList.busyClosing = true
                                    closeAnimation.restart()
                                }

                                ParallelAnimation {
                                    id: closeAnimation
                                    NumberAnimation { target: permDelegate; property: "closeScale"; to: 0.2; duration: 200; easing.type: Easing.InQuad }
                                    SequentialAnimation {
                                        PauseAnimation { duration: 45 }
                                        NumberAnimation { target: permDelegate; property: "closeDimension"; to: 0.0; duration: 175; easing.type: Easing.InOutQuad }
                                    }
                                    NumberAnimation { target: permDelegate; property: "closeOpacity"; to: 0.0; duration: 180; easing.type: Easing.OutQuad }
                                    onFinished: {
                                        var o = model.origin, f = model.feature
                                        permModel.remove(index)
                                        SitePermissionManager.removePermission(o, f)
                                        permList.busyClosing = false
                                    }
                                }

                                Item {
                                    id: content
                                    anchors.fill: parent
                                    opacity: permDelegate.isClosing ? permDelegate.closeOpacity : 1.0
                                    transform: Scale {
                                        origin.x: content.width / 2
                                        origin.y: content.height / 2
                                        xScale: permDelegate.isClosing ? permDelegate.closeScale : 1.0
                                        yScale: permDelegate.isClosing ? permDelegate.closeScale : 1.0
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
                                        anchors.rightMargin: 10
                                        spacing: 12

                                        CielIcon {
                                            icon: root.featureIcon(model.feature)
                                            size: Theme.SMALL
                                            color: permDelegate.tone
                                            Layout.alignment: Qt.AlignVCenter
                                        }

                                        ColumnLayout {
                                            Layout.fillWidth: true
                                            spacing: 2
                                            Layout.alignment: Qt.AlignVCenter

                                            Text {
                                                text: model.origin.replace(/^https:\/\//, "")
                                                font.pixelSize: 13
                                                font.weight: Font.Medium
                                                color: Theme.textPrimary
                                                elide: Text.ElideRight
                                                Layout.fillWidth: true
                                            }

                                            Text {
                                                text: root.featureName(model.feature) + " · "
                                                      + new Date(model.updated_at * 1000).toLocaleDateString(Qt.locale(), Locale.ShortFormat)
                                                font.pixelSize: 11
                                                color: Theme.textSecondary
                                                elide: Text.ElideRight
                                                Layout.fillWidth: true
                                            }
                                        }

                                        CielButton {
                                            text: "Ask"
                                            primary: permDelegate.decision === "ask"
                                            Layout.alignment: Qt.AlignVCenter
                                            onClicked: SitePermissionManager.setPermission(model.origin, model.feature, "ask")
                                        }
                                        CielButton {
                                            text: "Allow"
                                            primary: permDelegate.decision === "granted"
                                            Layout.alignment: Qt.AlignVCenter
                                            onClicked: SitePermissionManager.setPermission(model.origin, model.feature, "granted")
                                        }
                                        CielButton {
                                            text: "Block"
                                            primary: permDelegate.decision === "denied"
                                            Layout.alignment: Qt.AlignVCenter
                                            onClicked: SitePermissionManager.setPermission(model.origin, model.feature, "denied")
                                        }

                                        CielIconButton {
                                            icon: "trash"
                                            Layout.alignment: Qt.AlignVCenter
                                            onClicked: permDelegate.requestClose()
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
        id: resetPopup
        contentWidth: 380
        contentHeight: 190

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 24
            spacing: 14

            Text {
                text: "Reset all site permissions?"
                font.pixelSize: 17
                font.weight: Font.DemiBold
                color: Theme.textPrimary
            }

            Text {
                text: "Every site will ask again the next time it needs the camera, microphone, location or notifications."
                font.pixelSize: 13
                color: Theme.textSecondary
                wrapMode: Text.Wrap
                Layout.fillWidth: true
            }

            Item { Layout.fillHeight: true }

            RowLayout {
                Layout.fillWidth: true
                spacing: 10
                Item { Layout.fillWidth: true }

                CielButton { text: "Cancel"; onClicked: resetPopup.close() }
                CielButton {
                    text: "Reset"
                    destructive: true
                    onClicked: {
                        SitePermissionManager.clearAll()
                        resetPopup.close()
                    }
                }
            }
        }
    }
}
