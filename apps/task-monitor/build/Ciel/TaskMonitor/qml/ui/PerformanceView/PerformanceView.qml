import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import Ciel.Ui
import Ciel.TaskMonitor 1.0

Item {
    id: root

    ProcessTableModel {
        id: monitor
    }

    property int activeTabIndex: 0
    property var tabsList: []

    function rebuildTabs() {
        let tabs = [
            {
                type: "cpu",
                title: "CPU",
                color: Theme.accent
            },
            {
                type: "ram",
                title: "Memory",
                color: "#7E57C2"
            },
            {
                type: "disk",
                title: "Disk",
                color: "#26A69A"
            },
            {
                type: "net",
                title: "Network",
                color: "#EC407A"
            }
        ];

        for (let i = 0; i < monitor.gpuCount; ++i) {
            tabs.push({
                type: "gpu",
                gpuIndex: i,
                title: monitor.gpuCount > 1 ? ("GPU " + i) : "GPU",
                color: "#FFA726"
            });
        }
        root.tabsList = tabs;
    }

    function ensureTabVisible(idx) {
        if (!tabRow || idx < 0 || idx >= tabRow.children.length)
            return;
        let item = tabRow.children[idx];
        if (!item || item.width === undefined)
            return;
        if (item.x < tabBarScroll.contentX) {
            tabBarScroll.contentX = item.x;
        } else if (item.x + item.width > tabBarScroll.contentX + tabBarScroll.width) {
            tabBarScroll.contentX = item.x + item.width - tabBarScroll.width;
        }
    }

    Component.onCompleted: rebuildTabs()

    Connections {
        target: monitor
        function onGpusChanged() {
            root.rebuildTabs();
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 14
        spacing: 14

        Flickable {
            id: tabBarScroll
            Layout.fillWidth: true
            Layout.preferredHeight: 74
            contentWidth: tabRow.width
            contentHeight: height
            clip: true
            boundsBehavior: Flickable.StopAtBounds
            flickableDirection: Flickable.HorizontalFlick

            WheelHandler {
                acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad
                onWheel: event => {
                    let delta = event.angleDelta.y !== 0 ? event.angleDelta.y : event.angleDelta.x;
                    tabBarScroll.contentX = Math.max(0, Math.min(tabBarScroll.contentWidth - tabBarScroll.width, tabBarScroll.contentX - delta));
                }
            }

            Row {
                id: tabRow
                height: tabBarScroll.height
                spacing: 10

                Repeater {
                    model: root.tabsList

                    delegate: Item {
                        id: tabItem
                        required property int index
                        required property var modelData

                        readonly property bool isSelected: root.activeTabIndex === index
                        readonly property bool isHovered: tabMouseArea.containsMouse

                        width: 168
                        height: tabRow.height

                        scale: tabMouseArea.pressed ? 0.98 : 1.0
                        transformOrigin: Item.Center

                        Behavior on scale {
                            CielSpring {
                                damping: 4.85
                                spring: 8.0
                                mass: 4.0
                                epsilon: 0.01
                            }
                        }

                        CielSquircle {
                            id: tabBase
                            anchors.fill: parent
                            color: tabItem.isHovered ? Theme.surface : Theme.background
                            borderWidth: tabItem.isSelected ? 1.5 : 1
                            borderColor: tabItem.isSelected ? tabItem.modelData.color : Theme.border

                            Behavior on borderColor {
                                ColorAnimation {
                                    duration: 140
                                    easing.type: Easing.OutQuad
                                }
                            }

                            Behavior on color {
                                ColorAnimation {
                                    duration: 100
                                }
                            }
                        }

                        Item {
                            anchors.fill: parent
                            anchors.margins: 8

                            Column {
                                anchors.left: parent.left
                                anchors.right: miniGraph.left
                                anchors.rightMargin: 8
                                anchors.verticalCenter: parent.verticalCenter
                                spacing: 3

                                Text {
                                    width: parent.width
                                    text: tabItem.modelData.title
                                    font.pixelSize: 12
                                    font.weight: tabItem.isSelected ? Font.DemiBold : Font.Normal
                                    color: tabItem.isSelected ? tabItem.modelData.color : Theme.textSecondary
                                    elide: Text.ElideRight

                                    Behavior on color {
                                        ColorAnimation {
                                            duration: 120
                                        }
                                    }
                                }

                                Text {
                                    width: parent.width
                                    font.pixelSize: 11
                                    font.weight: Font.DemiBold
                                    color: Theme.textPrimary
                                    elide: Text.ElideRight
                                    text: {
                                        if (tabItem.modelData.type === "cpu") {
                                            return monitor.systemCpuUsage.toFixed(1) + "%";
                                        }
                                        if (tabItem.modelData.type === "ram") {
                                            return (monitor.systemRamUsedBytes / (1024 * 1024 * 1024)).toFixed(1) + " GB (" + monitor.systemRamUsagePercent.toFixed(0) + "%)";
                                        }
                                        if (tabItem.modelData.type === "disk") {
                                            return monitor.systemDiskSpeed;
                                        }
                                        if (tabItem.modelData.type === "net") {
                                            return monitor.systemNetSpeed;
                                        }
                                        if (tabItem.modelData.type === "gpu") {
                                            return monitor.gpuUsage(tabItem.modelData.gpuIndex).toFixed(1) + "%";
                                        }
                                        return "";
                                    }
                                }
                            }

                            CielGraphView {
                                id: miniGraph
                                anchors.right: parent.right
                                anchors.top: parent.top
                                anchors.bottom: parent.bottom
                                width: 54
                                strokeColor: tabItem.modelData.color
                                strokeWidth: 1.4
                                fillOpacityTop: tabItem.isSelected ? 0.16 : 0.06
                                maxValue: 1.0
                                minValue: 0.0
                                values: {
                                    if (monitor.historyRevision < 0)
                                        return [];
                                    if (tabItem.modelData.type === "cpu")
                                        return monitor.cpuHistory;
                                    if (tabItem.modelData.type === "ram")
                                        return monitor.ramHistory;
                                    if (tabItem.modelData.type === "disk")
                                        return monitor.diskHistory;
                                    if (tabItem.modelData.type === "net")
                                        return monitor.netHistory;
                                    if (tabItem.modelData.type === "gpu")
                                        return monitor.gpuHistory(tabItem.modelData.gpuIndex);
                                    return [];
                                }
                            }
                        }

                        MouseArea {
                            id: tabMouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                root.activeTabIndex = tabItem.index;
                                root.ensureTabVisible(tabItem.index);
                            }
                        }
                    }
                }
            }
        }

        Item {
            id: contentContainer
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            Repeater {
                model: root.tabsList

                Loader {
                    id: pageLoader
                    anchors.fill: parent
                    readonly property bool isCurrent: index === root.activeTabIndex
                    active: isCurrent
                    visible: opacity > 0.001
                    transformOrigin: Item.Center

                    opacity: isCurrent ? 1.0 : 0.0
                    scale: isCurrent ? 1.0 : 0.985

                    layer.enabled: pageOpacityAnim.running || pageScaleSpring.running
                    layer.smooth: true

                    Behavior on opacity {
                        NumberAnimation {
                            id: pageOpacityAnim
                            duration: 140
                            easing.type: Easing.OutQuad
                        }
                    }

                    Behavior on scale {
                        CielSpring {
                            id: pageScaleSpring
                            damping: 4.85
                            spring: 8.0
                            mass: 4.6
                            epsilon: 0.001
                        }
                    }

                    sourceComponent: {
                        if (modelData.type === "cpu")
                            return cpuPageComponent;
                        if (modelData.type === "ram")
                            return ramPageComponent;
                        if (modelData.type === "disk")
                            return diskPageComponent;
                        if (modelData.type === "net")
                            return netPageComponent;
                        return gpuPageComponent;
                    }

                    property var tabContext: modelData
                }
            }
        }
    }

    Component {
        id: cpuPageComponent

        CielScrollView {
            id: cpuScroll
            anchors.fill: parent
            contentWidth: availableWidth
            contentHeight: cpuContent.implicitHeight + 14

            ColumnLayout {
                id: cpuContent
                width: cpuScroll.availableWidth
                spacing: 14

                CielSquircle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 140
                    color: "transparent"
                    borderWidth: 1
                    borderColor: Theme.border

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 14
                        spacing: 8

                        RowLayout {
                            Layout.fillWidth: true

                            Text {
                                text: "Total CPU Utilization"
                                font.pixelSize: 13
                                font.weight: Font.DemiBold
                                color: Theme.textSecondary
                            }

                            Item {
                                Layout.fillWidth: true
                            }

                            Text {
                                text: monitor.systemCpuUsage.toFixed(1) + "%"
                                font.pixelSize: 14
                                font.weight: Font.Bold
                                color: Theme.textPrimary
                            }
                        }

                        CielGraphView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            strokeColor: Theme.accent
                            strokeWidth: 2.2
                            maxValue: 1.0
                            minValue: 0.0
                            values: monitor.cpuHistory
                        }
                    }
                }

                GridLayout {
                    Layout.fillWidth: true
                    columns: Math.max(2, Math.min(4, Math.floor(cpuScroll.availableWidth / 220)))
                    columnSpacing: 10
                    rowSpacing: 10

                    Repeater {
                        model: monitor.coreCount

                        delegate: CielSquircle {
                            required property int index

                            Layout.fillWidth: true
                            Layout.preferredHeight: 88
                            color: "transparent"
                            borderWidth: 1
                            borderColor: Theme.border

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 10
                                spacing: 4

                                RowLayout {
                                    Layout.fillWidth: true

                                    Text {
                                        text: "CPU " + (index + 1)
                                        font.pixelSize: 11
                                        font.weight: Font.DemiBold
                                        color: Theme.textSecondary
                                    }

                                    Item {
                                        Layout.fillWidth: true
                                    }

                                    Text {
                                        text: {
                                            const usage = monitor.perCoreUsage[index];
                                            return (usage !== undefined ? usage.toFixed(1) : "0.0") + "%";
                                        }
                                        font.pixelSize: 11
                                        font.weight: Font.Medium
                                        color: Theme.textPrimary
                                    }
                                }

                                CielGraphView {
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    strokeColor: Theme.accent
                                    strokeWidth: 1.6
                                    fillOpacityTop: 0.18
                                    maxValue: 1.0
                                    minValue: 0.0
                                    values: monitor.historyRevision >= 0 ? monitor.coreHistory(index) : []
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Component {
        id: ramPageComponent

        CielScrollView {
            id: ramScroll
            anchors.fill: parent
            contentWidth: availableWidth
            contentHeight: ramContent.implicitHeight + 14

            ColumnLayout {
                id: ramContent
                width: ramScroll.availableWidth
                spacing: 14

                CielSquircle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 180
                    color: "transparent"
                    borderWidth: 1
                    borderColor: Theme.border

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 14
                        spacing: 8

                        RowLayout {
                            Layout.fillWidth: true

                            Text {
                                text: "Memory Usage History"
                                font.pixelSize: 13
                                font.weight: Font.DemiBold
                                color: Theme.textSecondary
                            }

                            Item {
                                Layout.fillWidth: true
                            }

                            Text {
                                text: monitor.formatBytes(monitor.systemRamUsedBytes) + " / " + monitor.formatBytes(monitor.systemRamTotalBytes) + " (" + monitor.systemRamUsagePercent.toFixed(1) + "%)"
                                font.pixelSize: 14
                                font.weight: Font.Bold
                                color: Theme.textPrimary
                            }
                        }

                        CielGraphView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            strokeColor: "#7E57C2"
                            strokeWidth: 2.2
                            fillOpacityTop: 0.22
                            maxValue: 1.0
                            minValue: 0.0
                            values: monitor.ramHistory
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10

                    CielSquircle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 90
                        color: "transparent"
                        borderWidth: 1
                        borderColor: Theme.border

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 4

                            Text {
                                text: "In Use"
                                font.pixelSize: 12
                                color: Theme.textSecondary
                            }

                            Text {
                                text: monitor.formatBytes(monitor.systemRamUsedBytes)
                                font.pixelSize: 18
                                font.weight: Font.Bold
                                color: Theme.textPrimary
                            }
                        }
                    }

                    CielSquircle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 90
                        color: "transparent"
                        borderWidth: 1
                        borderColor: Theme.border

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 4

                            Text {
                                text: "Available"
                                font.pixelSize: 12
                                color: Theme.textSecondary
                            }

                            Text {
                                text: monitor.formatBytes(monitor.systemRamAvailBytes)
                                font.pixelSize: 18
                                font.weight: Font.Bold
                                color: Theme.textPrimary
                            }
                        }
                    }

                    CielSquircle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 90
                        color: "transparent"
                        borderWidth: 1
                        borderColor: Theme.border

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 4

                            Text {
                                text: "Swap"
                                font.pixelSize: 12
                                color: Theme.textSecondary
                            }

                            Text {
                                text: monitor.formatBytes(monitor.systemSwapUsedBytes) + " / " + monitor.formatBytes(monitor.systemSwapTotalBytes)
                                font.pixelSize: 18
                                font.weight: Font.Bold
                                color: Theme.textPrimary
                            }
                        }
                    }
                }
            }
        }
    }

    Component {
        id: diskPageComponent

        CielScrollView {
            id: diskScroll
            anchors.fill: parent
            contentWidth: availableWidth
            contentHeight: diskContent.implicitHeight + 14

            ColumnLayout {
                id: diskContent
                width: diskScroll.availableWidth
                spacing: 14

                CielSquircle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 180
                    color: "transparent"
                    borderWidth: 1
                    borderColor: Theme.border

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 14
                        spacing: 8

                        RowLayout {
                            Layout.fillWidth: true

                            Text {
                                text: "Disk Transfer Activity"
                                font.pixelSize: 13
                                font.weight: Font.DemiBold
                                color: Theme.textSecondary
                            }

                            Item {
                                Layout.fillWidth: true
                            }

                            Text {
                                text: monitor.systemDiskSpeed
                                font.pixelSize: 14
                                font.weight: Font.Bold
                                color: Theme.textPrimary
                            }
                        }

                        CielGraphView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            strokeColor: "#26A69A"
                            strokeWidth: 2.2
                            fillOpacityTop: 0.22
                            maxValue: 1.0
                            minValue: 0.0
                            values: monitor.diskHistory
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10

                    CielSquircle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 90
                        color: "transparent"
                        borderWidth: 1
                        borderColor: Theme.border

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 4

                            Text {
                                text: "Read Speed"
                                font.pixelSize: 12
                                color: Theme.textSecondary
                            }

                            Text {
                                text: monitor.formatSpeed(monitor.systemDiskReadBytesSec)
                                font.pixelSize: 18
                                font.weight: Font.Bold
                                color: Theme.textPrimary
                            }
                        }
                    }

                    CielSquircle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 90
                        color: "transparent"
                        borderWidth: 1
                        borderColor: Theme.border

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 4

                            Text {
                                text: "Write Speed"
                                font.pixelSize: 12
                                color: Theme.textSecondary
                            }

                            Text {
                                text: monitor.formatSpeed(monitor.systemDiskWriteBytesSec)
                                font.pixelSize: 18
                                font.weight: Font.Bold
                                color: Theme.textPrimary
                            }
                        }
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 8
                    visible: monitor.diskCount > 0

                    Text {
                        text: "Mounted Filesystems"
                        font.pixelSize: 13
                        font.weight: Font.DemiBold
                        color: Theme.textSecondary
                    }

                    Repeater {
                        model: monitor.diskCount

                        delegate: CielSquircle {
                            required property int index

                            Layout.fillWidth: true
                            Layout.preferredHeight: 64
                            color: "transparent"
                            borderWidth: 1
                            borderColor: Theme.border

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 12

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    spacing: 2

                                    Text {
                                        text: monitor.diskMount(index)
                                        font.pixelSize: 13
                                        font.weight: Font.DemiBold
                                        color: Theme.textPrimary
                                    }

                                    Text {
                                        text: monitor.diskFsType(index)
                                        font.pixelSize: 11
                                        color: Theme.textSecondary
                                    }
                                }

                                Text {
                                    text: monitor.formatBytes(monitor.diskUsedBytes(index)) + " / " + monitor.formatBytes(monitor.diskTotalBytes(index))
                                    font.pixelSize: 12
                                    font.weight: Font.Medium
                                    color: Theme.textPrimary
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Component {
        id: netPageComponent

        CielScrollView {
            id: netScroll
            anchors.fill: parent
            contentWidth: availableWidth
            contentHeight: netContent.implicitHeight + 14

            ColumnLayout {
                id: netContent
                width: netScroll.availableWidth
                spacing: 14

                CielSquircle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 180
                    color: "transparent"
                    borderWidth: 1
                    borderColor: Theme.border

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 14
                        spacing: 8

                        RowLayout {
                            Layout.fillWidth: true

                            Text {
                                text: "Network Activity"
                                font.pixelSize: 13
                                font.weight: Font.DemiBold
                                color: Theme.textSecondary
                            }

                            Item {
                                Layout.fillWidth: true
                            }

                            Text {
                                text: monitor.systemNetSpeed
                                font.pixelSize: 14
                                font.weight: Font.Bold
                                color: Theme.textPrimary
                            }
                        }

                        CielGraphView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            strokeColor: "#EC407A"
                            strokeWidth: 2.2
                            fillOpacityTop: 0.22
                            maxValue: 1.0
                            minValue: 0.0
                            values: monitor.netHistory
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10

                    CielSquircle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 90
                        color: "transparent"
                        borderWidth: 1
                        borderColor: Theme.border

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 4

                            Text {
                                text: "Receive (Rx)"
                                font.pixelSize: 12
                                color: Theme.textSecondary
                            }

                            Text {
                                text: monitor.formatSpeed(monitor.systemNetRxBytesSec)
                                font.pixelSize: 18
                                font.weight: Font.Bold
                                color: Theme.textPrimary
                            }
                        }
                    }

                    CielSquircle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 90
                        color: "transparent"
                        borderWidth: 1
                        borderColor: Theme.border

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 4

                            Text {
                                text: "Send (Tx)"
                                font.pixelSize: 12
                                color: Theme.textSecondary
                            }

                            Text {
                                text: monitor.formatSpeed(monitor.systemNetTxBytesSec)
                                font.pixelSize: 18
                                font.weight: Font.Bold
                                color: Theme.textPrimary
                            }
                        }
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 8
                    visible: monitor.networkCount > 0

                    Text {
                        text: "Network Adapters"
                        font.pixelSize: 13
                        font.weight: Font.DemiBold
                        color: Theme.textSecondary
                    }

                    Repeater {
                        model: monitor.networkCount

                        delegate: CielSquircle {
                            required property int index

                            Layout.fillWidth: true
                            Layout.preferredHeight: 64
                            color: "transparent"
                            borderWidth: 1
                            borderColor: Theme.border

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 12

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    spacing: 2

                                    Text {
                                        text: monitor.networkName(index)
                                        font.pixelSize: 13
                                        font.weight: Font.DemiBold
                                        color: Theme.textPrimary
                                    }

                                    Text {
                                        text: monitor.networkIsWireless(index) ? "Wireless Connection" : "Ethernet Connection"
                                        font.pixelSize: 11
                                        color: Theme.textSecondary
                                    }
                                }

                                ColumnLayout {
                                    spacing: 2

                                    Text {
                                        text: "↓ " + monitor.formatSpeed(monitor.networkRxSpeed(index))
                                        font.pixelSize: 11
                                        font.weight: Font.Medium
                                        color: Theme.textPrimary
                                    }

                                    Text {
                                        text: "↑ " + monitor.formatSpeed(monitor.networkTxSpeed(index))
                                        font.pixelSize: 11
                                        font.weight: Font.Medium
                                        color: Theme.textPrimary
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Component {
        id: gpuPageComponent

        CielScrollView {
            id: gpuScroll
            anchors.fill: parent
            contentWidth: availableWidth
            contentHeight: gpuContent.implicitHeight + 14

            readonly property int gpuIdx: parent.tabContext ? parent.tabContext.gpuIndex : 0

            ColumnLayout {
                id: gpuContent
                width: gpuScroll.availableWidth
                spacing: 14

                CielSquircle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 180
                    color: "transparent"
                    borderWidth: 1
                    borderColor: Theme.border

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 14
                        spacing: 8

                        RowLayout {
                            Layout.fillWidth: true

                            Text {
                                text: {
                                    const name = monitor.gpuName(gpuScroll.gpuIdx);
                                    if (typeof monitor.gpuId === "function") {
                                        const id = monitor.gpuId(gpuScroll.gpuIdx);
                                        return id.length > 0 ? (name + " (" + id + ")") : name;
                                    }
                                    return name;
                                }
                                font.pixelSize: 13
                                font.weight: Font.DemiBold
                                color: Theme.textSecondary
                            }

                            Item {
                                Layout.fillWidth: true
                            }

                            Text {
                                text: monitor.gpuUsage(gpuScroll.gpuIdx).toFixed(1) + "%"
                                font.pixelSize: 14
                                font.weight: Font.Bold
                                color: Theme.textPrimary
                            }
                        }

                        CielGraphView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            strokeColor: "#FFA726"
                            strokeWidth: 2.2
                            fillOpacityTop: 0.22
                            maxValue: 1.0
                            minValue: 0.0
                            values: monitor.historyRevision >= 0 ? monitor.gpuHistory(gpuScroll.gpuIdx) : []
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10

                    CielSquircle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 90
                        color: "transparent"
                        borderWidth: 1
                        borderColor: Theme.border

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 4

                            Text {
                                text: "GPU Memory Used"
                                font.pixelSize: 12
                                color: Theme.textSecondary
                            }

                            Text {
                                text: monitor.formatBytes(monitor.gpuVramUsed(gpuScroll.gpuIdx))
                                font.pixelSize: 18
                                font.weight: Font.Bold
                                color: Theme.textPrimary
                            }
                        }
                    }

                    CielSquircle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 90
                        color: "transparent"
                        borderWidth: 1
                        borderColor: Theme.border

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 4

                            Text {
                                text: "GPU Memory Total"
                                font.pixelSize: 12
                                color: Theme.textSecondary
                            }

                            Text {
                                text: monitor.formatBytes(monitor.gpuVramTotal(gpuScroll.gpuIdx))
                                font.pixelSize: 18
                                font.weight: Font.Bold
                                color: Theme.textPrimary
                            }
                        }
                    }

                    CielSquircle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 90
                        color: "transparent"
                        borderWidth: 1
                        borderColor: Theme.border
                        visible: monitor.gpuTemp(gpuScroll.gpuIdx) > 0.0

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 4

                            Text {
                                text: "Temperature"
                                font.pixelSize: 12
                                color: Theme.textSecondary
                            }

                            Text {
                                text: monitor.gpuTemp(gpuScroll.gpuIdx).toFixed(0) + " °C"
                                font.pixelSize: 18
                                font.weight: Font.Bold
                                color: Theme.textPrimary
                            }
                        }
                    }
                }
            }
        }
    }
}
