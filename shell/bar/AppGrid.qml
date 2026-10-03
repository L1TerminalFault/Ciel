import Quickshell
import Quickshell.Io
import QtQuick
import Ciel.Ui

Item {
    id: root

    property string searchFilter: ""
    property var filteredApps: []
    signal appLaunched(var app)

    property real scrollMultiplier: 3.0
    property real flickVelocityMultiplier: 56.0
    property real scrollDeceleration: 950
    property real maxScrollVelocity: 7000
    property real wheelStepSize: 150

    AppCache {
        id: appCache
        onAppsChanged: {
            root.filter();
        }
    }

    function filter() {
        var all = appCache.apps;
        var query = root.searchFilter.trim().toLowerCase();
        var results = [];

        for (var i = 0; i < all.length; ++i) {
            var item = all[i];
            var name = item.name.toLowerCase();
            var id = item.id.toLowerCase();

            if (!query || name.indexOf(query) !== -1 || id.indexOf(query) !== -1) {
                var iconResolved = Quickshell.iconPath(item.icon, true);
                results.push({
                    id: item.id,
                    name: item.name,
                    icon: item.icon,
                    exec: item.exec,
                    hasIcon: iconResolved !== ""
                });
            }
        }

        results.sort(function (a, b) {
            if (a.hasIcon && !b.hasIcon)
                return -1;
            if (!a.hasIcon && b.hasIcon)
                return 1;
            return (a.name || "").localeCompare(b.name || "");
        });

        root.filteredApps = results;
    }

    onSearchFilterChanged: {
        root.filter();
    }
    Component.onCompleted: {
        root.filter();
    }

    Process {
        id: launcherProc
        command: []
        running: false
    }

    GridView {
        id: gridView
        anchors.fill: parent
        clip: true
        cacheBuffer: 80
        cellWidth: Math.floor(width / 5)
        cellHeight: 96
        model: root.filteredApps

        pixelAligned: false
        flickableDirection: Flickable.VerticalFlick
        boundsBehavior: Flickable.DragAndOvershootBounds
        flickDeceleration: root.scrollDeceleration
        maximumFlickVelocity: root.maxScrollVelocity

        Text {
            anchors.centerIn: parent
            visible: root.filteredApps.length === 0
            text: appCache.apps.length === 0 ? "Loading apps..." : "No applications found"
            color: Theme.textSecondary
            font.pixelSize: 13
            renderType: Text.NativeRendering
        }

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.NoButton

            onWheel: wheel => {
                var dy = 0;
                if (wheel.pixelDelta.y !== 0) {
                    dy = wheel.pixelDelta.y * root.scrollMultiplier;
                } else if (wheel.angleDelta.y !== 0) {
                    dy = (wheel.angleDelta.y / 120.0) * root.wheelStepSize;
                }
                gridView.flick(0, dy * root.flickVelocityMultiplier);
            }
        }

        delegate: Item {
            id: itemDelegate
            width: gridView.cellWidth
            height: gridView.cellHeight

            Rectangle {
                anchors.fill: parent
                anchors.margins: 4
                radius: Theme.metrics.radiusMd
                color: itemMouse.containsMouse ? Theme.surfaceHover : Theme.surface

                Behavior on color {
                    ColorAnimation {
                        duration: 70
                        easing.type: Easing.OutQuad
                    }
                }
            }

            Item {
                id: contentWrapper
                anchors.fill: parent
                scale: itemMouse.pressed ? 0.94 : 1.0

                Behavior on scale {
                    NumberAnimation {
                        duration: 80
                        easing.type: Easing.OutQuad
                    }
                }

                Item {
                    id: iconBox
                    width: 44
                    height: 44
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 14
                    clip: true

                    CielSquircle {
                        anchors.fill: parent
                        visible: !modelData.hasIcon || appIconImg.status !== Image.Ready
                        radius: 10
                        color: Theme.surfaceHover

                        Text {
                            anchors.centerIn: parent
                            text: modelData.name ? modelData.name.charAt(0).toUpperCase() : "◇"
                            color: Theme.textSecondary
                            font.pixelSize: 16
                            font.bold: true
                            renderType: Text.NativeRendering
                        }
                    }

                    Image {
                        id: appIconImg
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                        sourceSize: Qt.size(64, 64)
                        asynchronous: true
                        source: Quickshell.iconPath(modelData.icon, true)
                        visible: modelData.hasIcon && status === Image.Ready
                    }
                }

                Text {
                    anchors.top: iconBox.bottom
                    anchors.topMargin: 8
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: 8
                    anchors.rightMargin: 8
                    text: modelData.name || ""
                    color: itemMouse.containsMouse ? Theme.textPrimary : Theme.textSecondary
                    font.pixelSize: 11
                    elide: Text.ElideRight
                    horizontalAlignment: Text.AlignHCenter
                    renderType: Text.NativeRendering

                    Behavior on color {
                        ColorAnimation {
                            duration: 100
                        }
                    }
                }
            }

            MouseArea {
                id: itemMouse
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor

                onClicked: {
                    if (modelData.exec) {
                        launcherProc.command = ["sh", "-c", modelData.exec + " &"];
                        launcherProc.running = true;
                    }
                    root.appLaunched(modelData);
                }
            }
        }
    }
}
