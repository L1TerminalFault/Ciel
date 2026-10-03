import Quickshell
import Quickshell.Wayland
import Quickshell.Io
import QtQuick
import QtQuick.Layouts
import Quickshell.Widgets
import Ciel.Ui

PanelWindow {
    id: bar

    property int paddingHorizontal: 12
    property int paddingVertical: 8

    anchors.top: true
    margins.top: 5
    implicitWidth: content.implicitWidth + (paddingHorizontal * 2)
    implicitHeight: content.implicitHeight + (paddingVertical * 2)
    color: "transparent"

    WlrLayershell.keyboardFocus: appLauncher.isOpen ? WlrLayershell.OnDemand : WlrLayershell.None

    function appIcon(appId) {
        if (!appId)
            return "";
        const entry = DesktopEntries.heuristicLookup(appId);
        if (!entry)
            return "";
        return Quickshell.iconPath(entry.icon, true);
    }

    property var activeWorkspaces: []
    property string hyprBuffer: ""

    function parseHyprClients(text) {
        try {
            var clients = JSON.parse(text);
            var wsMap = {};
            var maxWs = 1;
            var focusedWs = 1;

            for (var i = 0; i < clients.length; ++i) {
                var c = clients[i];
                var wsId = c.workspace ? c.workspace.id : null;
                if (wsId === null || wsId < 1 || wsId > 5)
                    continue;
                if (wsId > maxWs)
                    maxWs = wsId;

                if (!wsMap[wsId]) {
                    wsMap[wsId] = {
                        workspaceId: wsId,
                        apps: [],
                        isActive: false,
                        isEmpty: false
                    };
                }

                var appId = c.initialClass || c.class || "";
                var isFocused = c.focusHistoryID === 0;

                wsMap[wsId].apps.push({
                    appId: appId,
                    title: c.title || "",
                    address: c.address || "",
                    isFocused: isFocused
                });

                if (isFocused) {
                    wsMap[wsId].isActive = true;
                    focusedWs = wsId;
                }
            }

            if (focusedWs > maxWs && focusedWs <= 5) {
                maxWs = focusedWs;
            }

            var list = [];
            for (var w = 1; w <= maxWs; ++w) {
                if (wsMap[w] && wsMap[w].apps.length > 0) {
                    wsMap[w].apps.sort(function (a, b) {
                        if (a.isFocused && !b.isFocused)
                            return -1;
                        if (!a.isFocused && b.isFocused)
                            return 1;
                        return 0;
                    });
                    list.push(wsMap[w]);
                } else {
                    list.push({
                        workspaceId: w,
                        apps: [],
                        isActive: (w === focusedWs),
                        isEmpty: true
                    });
                }
            }

            bar.activeWorkspaces = list;
        } catch (e) {
            fallbackGrouping();
        }
    }

    function fallbackGrouping() {
        var raw = ToplevelManager.toplevels;
        var map = {};
        for (var i = 0; i < raw.length; ++i) {
            var tl = raw[i];
            var key = tl.appId || String(i);
            if (!map[key]) {
                map[key] = {
                    workspaceId: 1,
                    apps: [],
                    isActive: false,
                    isEmpty: false
                };
            }
            map[key].apps.push({
                appId: tl.appId,
                title: tl.title,
                address: "",
                isFocused: tl.activated,
                toplevel: tl
            });
            if (tl.activated) {
                map[key].isActive = true;
            }
        }
        var arr = [];
        for (var k in map) {
            arr.push(map[k]);
        }
        bar.activeWorkspaces = arr;
    }

    function switchToWorkspace(wsId, address, toplevel) {
        if (address) {
            dispatchProc.command = ["hyprctl", "--batch", "dispatch workspace " + wsId + " ; dispatch focuswindow address:" + address];
            dispatchProc.running = true;
        } else {
            dispatchProc.command = ["hyprctl", "dispatch", "workspace", String(wsId)];
            dispatchProc.running = true;
        }

        if (toplevel && toplevel.activate) {
            toplevel.activate();
        }
    }

    Process {
        id: dispatchProc
        command: []
        running: false
    }

    Process {
        id: hyprWatcher
        command: ["hyprctl", "clients", "-j"]
        running: false

        stdout: SplitParser {
            onRead: data => {
                bar.hyprBuffer += data + "\n";
            }
        }

        onExited: (code, status) => {
            if (code === 0 && bar.hyprBuffer.trim() !== "") {
                bar.parseHyprClients(bar.hyprBuffer);
            }
            bar.hyprBuffer = "";
        }
    }

    Timer {
        interval: 350
        running: true
        repeat: true
        onTriggered: {
            if (!hyprWatcher.running) {
                bar.hyprBuffer = "";
                hyprWatcher.running = true;
            }
        }
    }

    Instantiator {
        model: ToplevelManager.toplevels
        onObjectAdded: {
            if (!hyprWatcher.running) {
                bar.hyprBuffer = "";
                hyprWatcher.running = true;
            }
        }
        onObjectRemoved: {
            if (!hyprWatcher.running) {
                bar.hyprBuffer = "";
                hyprWatcher.running = true;
            }
        }
        delegate: QtObject {
            property bool act: modelData ? modelData.activated : false
            onActChanged: {
                if (!hyprWatcher.running) {
                    bar.hyprBuffer = "";
                    hyprWatcher.running = true;
                }
            }
        }
    }

    component Separator: Rectangle {
        implicitWidth: 1
        implicitHeight: 14
        Layout.preferredWidth: implicitWidth
        Layout.preferredHeight: implicitHeight
        color: Theme.surfaceHover
    }

    // Bar chassis: pure G2 continuous squircle backed by CielTheme
    CielSquircle {
        anchors.fill: parent
        radius: parent.height * 0.38
        color: Theme.surface
        borderColor: Theme.surfaceHover
        borderWidth: 1
    }

    RowLayout {
        id: content
        anchors.centerIn: parent
        spacing: Theme.metrics.spacingSm

        WinButton {
            id: winButton
            onClicked: appLauncher.toggle()
        }

        Separator {}

        Item {
            id: iconStrip

            implicitWidth: iconRow.implicitWidth
            implicitHeight: 34
            Layout.preferredWidth: implicitWidth
            Layout.preferredHeight: implicitHeight

            Row {
                id: iconRow
                anchors.verticalCenter: parent.verticalCenter
                spacing: Theme.metrics.spacingSm

                Repeater {
                    model: bar.activeWorkspaces

                    delegate: Item {
                        id: delegateItem
                        required property var modelData

                        property bool isEmpty: modelData.isEmpty
                        property int count: modelData.apps.length
                        property int extraOffset: isEmpty ? 0 : Math.min(count - 1, 2) * 5

                        implicitWidth: 34 + extraOffset
                        implicitHeight: 34
                        width: implicitWidth
                        height: implicitHeight

                        property real itemCenterX: x + (width / 2)
                        property real distance: Math.abs(dockHoverArea.mouseX - itemCenterX)
                        property real normalizedDist: Math.min(distance / 72, 1.0)
                        property real influence: dockHoverArea.containsMouse ? 0.5 * (1.0 + Math.cos(Math.PI * normalizedDist)) : 0

                        Item {
                            id: scalableContent
                            anchors.fill: parent
                            transformOrigin: Item.Center
                            scale: 1.0 + (delegateItem.influence * 0.15)

                            Behavior on scale {
                                SpringAnimation {
                                    spring: 2.8
                                    damping: 0.42
                                    epsilon: 0.001
                                }
                            }

                            Rectangle {
                                anchors.fill: parent
                                radius: 7
                                color: Theme.textPrimary
                                opacity: delegateItem.influence * 0.08

                                Behavior on opacity {
                                    NumberAnimation {
                                        duration: 130
                                        easing.type: Easing.OutQuad
                                    }
                                }
                            }

                            // Empty workspace dot
                            Rectangle {
                                anchors.centerIn: parent
                                visible: delegateItem.isEmpty
                                width: 6
                                height: 6
                                radius: 3
                                color: delegateItem.modelData.isActive ? Theme.textPrimary : Theme.textSecondary
                                opacity: delegateItem.modelData.isActive ? 0.9 : 0.4

                                Behavior on color {
                                    ColorAnimation {
                                        duration: 180
                                        easing.type: Easing.OutQuad
                                    }
                                }
                            }

                            // Active app icon stack
                            Item {
                                anchors.centerIn: parent
                                visible: !delegateItem.isEmpty
                                width: 26 + delegateItem.extraOffset
                                height: 26

                                Repeater {
                                    model: Math.min(delegateItem.count, 3)

                                    delegate: Item {
                                        z: -index
                                        width: 26
                                        height: 26
                                        x: index * 5
                                        y: -(index * 1.5)
                                        scale: 1.0 - (index * 0.08)
                                        opacity: index === 0 ? 1.0 : (index === 1 ? 0.65 : 0.38)

                                        Image {
                                            id: appImg
                                            anchors.fill: parent
                                            fillMode: Image.PreserveAspectFit
                                            sourceSize: Qt.size(64, 64)
                                            source: appIcon(delegateItem.modelData.apps[index].appId)

                                            Timer {
                                                interval: 200
                                                running: appImg.source == ""
                                                repeat: true
                                                onTriggered: {
                                                    var s = appIcon(delegateItem.modelData.apps[index].appId);
                                                    if (s !== "") {
                                                        appImg.source = s;
                                                        stop();
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        // Active workspace bottom pill indicator
                        Rectangle {
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: -1
                            width: 3.5
                            height: 3.5
                            radius: 1.75
                            color: Theme.accent
                            opacity: delegateItem.modelData.isActive ? 0.9 : 0.0

                            Behavior on opacity {
                                NumberAnimation {
                                    duration: 200
                                }
                            }
                        }
                    }
                }
            }

            MouseArea {
                id: dockHoverArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor

                onClicked: mouse => {
                    let target = iconRow.childAt(mouse.x, mouse.y);
                    if (!target) {
                        for (let i = 0; i < iconRow.children.length; ++i) {
                            let child = iconRow.children[i];
                            if (child && child.modelData && Math.abs(mouse.x - (child.x + child.width / 2)) < (child.width / 2 + 4)) {
                                target = child;
                                break;
                            }
                        }
                    }

                    if (target && target.modelData) {
                        var ws = target.modelData;
                        var topApp = ws.apps.length > 0 ? ws.apps[0] : null;
                        bar.switchToWorkspace(ws.workspaceId, topApp ? topApp.address : "", topApp ? topApp.toplevel : null);
                    }
                }
            }
        }

        Separator {}

        // Clock
        Text {
            id: clock
            color: Theme.textSecondary
            text: Qt.formatTime(new Date(), "HH:mm")
            font.pixelSize: 13
            font.weight: Font.Medium
            renderType: Text.NativeRendering

            Timer {
                interval: 1000
                running: true
                repeat: true
                onTriggered: {
                    clock.text = Qt.formatTime(new Date(), "HH:mm");
                }
            }
        }

        // Live notification counter badge (from NotificationsModel)
        Rectangle {
            visible: NotificationsModel.unreadCount > 0
            implicitWidth: notifCount.implicitWidth + 8
            implicitHeight: 16
            radius: 8
            color: Theme.accent

            Text {
                id: notifCount
                anchors.centerIn: parent
                text: NotificationsModel.unreadCount
                font.pixelSize: 10
                font.weight: Font.Bold
                color: "#FFFFFF"
                renderType: Text.NativeRendering
            }
        }

        WifiButton {
            id: wifiButton
            onClicked: wifiPopup.toggle()
        }

        BatteryIndicator {
            id: batteryIndicator
        }

        Separator {}

        ControlsButton {
            id: controlsButton
            onClicked: controlsPopup.toggle()
        }
    }

    WifiPopup {
        id: wifiPopup
        targetWindow: bar
        anchorRect: Qt.rect(content.x + wifiButton.x, content.y + wifiButton.y + 6, wifiButton.width, wifiButton.height)
    }

    AppLauncherPopup {
        id: appLauncher
        targetWindow: bar
        anchorRect: Qt.rect(0, content.y + 6, bar.width, content.height)
    }

    ControlsPopup {
        id: controlsPopup
        targetWindow: bar
        anchorRect: Qt.rect(content.x + controlsButton.x, content.y + controlsButton.y + 6, controlsButton.width, controlsButton.height)
    }
}
