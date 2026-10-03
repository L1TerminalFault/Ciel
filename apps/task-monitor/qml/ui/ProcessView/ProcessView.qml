import QtQuick
import QtQuick.Controls as T
import QtQuick.Layouts
import QtQuick.Effects
import Ciel.Ui
import Ciel.TaskMonitor 1.0

Item {
    id: root

    property alias filterText: processModel.filterText
    property int activeSortColumn: processModel.sortColumn
    property int activeSortOrder: processModel.sortOrder

    property var columnHeaders: ["PID", "Process Name", "CPU", "Memory", "Disk", "Network"]
    property var columnWidths: [70, 240, 85, 95, 95, 95]

    property real rowHeight: 32
    property real headerHeight: rowHeight * 2
    property real cellPadding: 8
    property real radius: 12

    anchors.fill: parent

    ProcessTableModel {
        id: processModel
    }

    function getColumnWidth(col) {
        if (columnWidths && col < columnWidths.length) {
            return columnWidths[col];
        }
        return 100;
    }

    function setColumnWidth(col, newWidth) {
        let widths = columnWidths.slice();
        widths[col] = Math.max(50, newWidth);
        columnWidths = widths;
    }

    function getHeaderMetric(index) {
        switch (index) {
        case 2:
            return Math.round(processModel.systemCpuUsage) + "%";
        case 3:
            return Math.round(processModel.systemRamUsagePercent) + "%";
        case 4:
            return processModel.systemDiskSpeed;
        case 5:
            return processModel.systemNetSpeed;
        default:
            return "";
        }
    }

    layer.enabled: true
    layer.effect: MultiEffect {
        maskEnabled: true
        maskSource: ShaderEffectSource {
            sourceItem: squircleMask
            hideSource: true
        }
    }

    CielSquircle {
        id: squircleMask
        width: root.width
        height: root.height
        visible: false
        color: "white"
    }

    Item {
        id: headerContainer
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: root.headerHeight
        clip: true

        Row {
            id: headerRow
            x: -internalScroll.contentX
            height: parent.height

            Repeater {
                model: root.columnHeaders.length

                Item {
                    id: headerCell
                    width: root.getColumnWidth(index)
                    height: headerContainer.height

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (root.activeSortColumn === index) {
                                processModel.sortByColumn(index, root.activeSortOrder === Qt.DescendingOrder ? Qt.AscendingOrder : Qt.DescendingOrder);
                            } else {
                                processModel.sortByColumn(index, Qt.DescendingOrder);
                            }
                        }
                    }

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.leftMargin: root.cellPadding
                        anchors.rightMargin: root.cellPadding
                        anchors.topMargin: 4
                        anchors.bottomMargin: 4
                        spacing: 1

                        Text {
                            text: root.getHeaderMetric(index)
                            visible: text !== ""
                            color: Theme.textPrimary
                            font.pixelSize: 14
                            font.weight: Font.DemiBold
                            verticalAlignment: Text.AlignVCenter
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 4

                            Text {
                                Layout.fillWidth: true
                                verticalAlignment: Text.AlignVCenter
                                color: root.activeSortColumn === index ? Theme.textPrimary : Theme.textSecondary
                                font.pixelSize: 12
                                font.weight: root.activeSortColumn === index ? Font.Bold : Font.Normal
                                text: root.columnHeaders[index]
                                elide: Text.ElideRight
                            }

                            CielIcon {
                                icon: "caret-down-fill"
                                size: 12
                                color: Theme.accent
                                visible: root.activeSortColumn === index
                                opacity: root.activeSortColumn === index ? 1.0 : 0.0
                                rotation: root.activeSortOrder === Qt.DescendingOrder ? 0 : 180

                                Behavior on rotation {
                                    NumberAnimation {
                                        duration: 200
                                        easing.type: Easing.OutCubic
                                    }
                                }

                                Behavior on opacity {
                                    NumberAnimation {
                                        duration: 200
                                        easing.type: Easing.OutCubic
                                    }
                                }
                            }
                        }
                    }

                    Rectangle {
                        id: activeIndicator
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        height: 2
                        color: Theme.accent
                        opacity: root.activeSortColumn === index ? 1.0 : 0.0

                        Behavior on opacity {
                            NumberAnimation {
                                duration: 180
                                easing.type: Easing.OutQuad
                            }
                        }
                    }

                    Rectangle {
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        width: 1
                        color: Theme.border
                        visible: index < (root.columnHeaders.length - 1)
                    }

                    MouseArea {
                        id: resizeHandle
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        width: 12
                        anchors.rightMargin: -6
                        cursorShape: Qt.SplitHCursor
                        visible: index < (root.columnHeaders.length - 1)

                        property real startWidth: 0
                        property real startRootX: 0

                        onPressed: mouse => {
                            startWidth = root.getColumnWidth(index);
                            startRootX = mapToItem(root, mouse.x, 0).x;
                        }

                        onPositionChanged: mouse => {
                            if (pressed) {
                                let currentRootX = mapToItem(root, mouse.x, 0).x;
                                let delta = currentRootX - startRootX;
                                root.setColumnWidth(index, startWidth + delta);
                            }
                        }
                    }
                }
            }
        }

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 1
            color: Theme.border
        }
    }

    CielScrollView {
        id: internalScroll
        anchors.top: headerContainer.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        contentWidth: Math.max(availableWidth, headerRow.width)
        contentHeight: processListCol.implicitHeight

        Column {
            id: processListCol
            width: Math.max(internalScroll.availableWidth, headerRow.width)

            Repeater {
                model: processModel

                delegate: Column {
                    id: rowGroupContainer
                    width: processListCol.width

                    property bool isExpanded: false
                    property var subItems: (model && model.subProcesses !== undefined) ? model.subProcesses : []
                    property int subCount: (model && model.childCount !== undefined) ? model.childCount : 0

                    Rectangle {
                        id: mainRow
                        width: parent.width
                        height: root.rowHeight
                        color: Theme.background

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: (rowGroupContainer.subCount > 1) ? Qt.PointingHandCursor : Qt.ArrowCursor
                            onClicked: {
                                if (rowGroupContainer.subCount > 1) {
                                    rowGroupContainer.isExpanded = !rowGroupContainer.isExpanded;
                                }
                            }
                        }

                        Rectangle {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.bottom: parent.bottom
                            height: 1
                            color: Theme.border
                        }

                        Row {
                            anchors.fill: parent

                            Item {
                                width: root.getColumnWidth(0)
                                height: parent.height

                                Text {
                                    text: (model && model.pid > 0) ? model.pid : ""
                                    anchors.fill: parent
                                    anchors.leftMargin: root.cellPadding
                                    anchors.rightMargin: root.cellPadding
                                    verticalAlignment: Text.AlignVCenter
                                    color: Theme.textPrimary
                                    font.pixelSize: 12
                                    elide: Text.ElideRight
                                }

                                Rectangle {
                                    anchors.right: parent.right
                                    anchors.top: parent.top
                                    anchors.bottom: parent.bottom
                                    width: 1
                                    color: Theme.border
                                }
                            }

                            Item {
                                width: root.getColumnWidth(1)
                                height: parent.height

                                Text {
                                    text: (model && model.procName !== undefined) ? model.procName : ""
                                    anchors.fill: parent
                                    anchors.leftMargin: root.cellPadding
                                    anchors.rightMargin: root.cellPadding
                                    verticalAlignment: Text.AlignVCenter
                                    color: Theme.textPrimary
                                    font.pixelSize: 13
                                    font.weight: rowGroupContainer.subCount > 1 ? Font.Medium : Font.Normal
                                    elide: Text.ElideRight
                                }

                                Rectangle {
                                    anchors.right: parent.right
                                    anchors.top: parent.top
                                    anchors.bottom: parent.bottom
                                    width: 1
                                    color: Theme.border
                                }
                            }

                            Item {
                                width: root.getColumnWidth(2)
                                height: parent.height

                                Text {
                                    text: (model && model.cpu !== undefined) ? model.cpu : ""
                                    anchors.fill: parent
                                    anchors.leftMargin: root.cellPadding
                                    anchors.rightMargin: root.cellPadding
                                    verticalAlignment: Text.AlignVCenter
                                    color: Theme.textPrimary
                                    font.pixelSize: 13
                                    font.weight: rowGroupContainer.subCount > 1 ? Font.Medium : Font.Normal
                                    elide: Text.ElideRight
                                }

                                Rectangle {
                                    anchors.right: parent.right
                                    anchors.top: parent.top
                                    anchors.bottom: parent.bottom
                                    width: 1
                                    color: Theme.border
                                }
                            }

                            Item {
                                width: root.getColumnWidth(3)
                                height: parent.height

                                Text {
                                    text: (model && model.ram !== undefined) ? model.ram : ""
                                    anchors.fill: parent
                                    anchors.leftMargin: root.cellPadding
                                    anchors.rightMargin: root.cellPadding
                                    verticalAlignment: Text.AlignVCenter
                                    color: Theme.textPrimary
                                    font.pixelSize: 13
                                    font.weight: rowGroupContainer.subCount > 1 ? Font.Medium : Font.Normal
                                    elide: Text.ElideRight
                                }

                                Rectangle {
                                    anchors.right: parent.right
                                    anchors.top: parent.top
                                    anchors.bottom: parent.bottom
                                    width: 1
                                    color: Theme.border
                                }
                            }

                            Item {
                                width: root.getColumnWidth(4)
                                height: parent.height

                                Text {
                                    text: (model && model.disk !== undefined) ? model.disk : ""
                                    anchors.fill: parent
                                    anchors.leftMargin: root.cellPadding
                                    anchors.rightMargin: root.cellPadding
                                    verticalAlignment: Text.AlignVCenter
                                    color: Theme.textPrimary
                                    font.pixelSize: 13
                                    font.weight: rowGroupContainer.subCount > 1 ? Font.Medium : Font.Normal
                                    elide: Text.ElideRight
                                }

                                Rectangle {
                                    anchors.right: parent.right
                                    anchors.top: parent.top
                                    anchors.bottom: parent.bottom
                                    width: 1
                                    color: Theme.border
                                }
                            }

                            Item {
                                width: root.getColumnWidth(5)
                                height: parent.height

                                Text {
                                    text: (model && model.net !== undefined) ? model.net : ""
                                    anchors.fill: parent
                                    anchors.leftMargin: root.cellPadding
                                    anchors.rightMargin: root.cellPadding
                                    verticalAlignment: Text.AlignVCenter
                                    color: Theme.textPrimary
                                    font.pixelSize: 13
                                    font.weight: rowGroupContainer.subCount > 1 ? Font.Medium : Font.Normal
                                    elide: Text.ElideRight
                                }
                            }
                        }
                    }

                    Item {
                        id: subRowsWrapper
                        width: parent.width
                        height: rowGroupContainer.isExpanded ? (rowGroupContainer.subCount * root.rowHeight) : 0
                        clip: true
                        visible: height > 0

                        Behavior on height {
                            CielSpring {
                                spring: 5.0
                                damping: 0.5
                                mass: 1.0
                                epsilon: 0.005
                            }
                        }

                        Column {
                            width: parent.width

                            Repeater {
                                model: rowGroupContainer.subItems

                                Rectangle {
                                    width: parent.width
                                    height: root.rowHeight
                                    color: Theme.surface

                                    Rectangle {
                                        anchors.left: parent.left
                                        anchors.right: parent.right
                                        anchors.bottom: parent.bottom
                                        height: 1
                                        color: Theme.border
                                    }

                                    Row {
                                        anchors.fill: parent

                                        Item {
                                            width: root.getColumnWidth(0)
                                            height: parent.height

                                            Text {
                                                text: (modelData && modelData.pid > 0) ? modelData.pid : ""
                                                anchors.fill: parent
                                                anchors.leftMargin: root.cellPadding
                                                anchors.rightMargin: root.cellPadding
                                                verticalAlignment: Text.AlignVCenter
                                                color: Theme.textPrimary
                                                font.pixelSize: 12
                                                elide: Text.ElideRight
                                            }

                                            Rectangle {
                                                anchors.right: parent.right
                                                anchors.top: parent.top
                                                anchors.bottom: parent.bottom
                                                width: 1
                                                color: Theme.border
                                            }
                                        }

                                        Item {
                                            width: root.getColumnWidth(1)
                                            height: parent.height

                                            Text {
                                                text: (modelData && modelData.name !== undefined) ? modelData.name : ""
                                                anchors.fill: parent
                                                anchors.leftMargin: root.cellPadding + 22
                                                anchors.rightMargin: root.cellPadding
                                                verticalAlignment: Text.AlignVCenter
                                                color: Theme.textPrimary
                                                font.pixelSize: 13
                                                elide: Text.ElideRight
                                            }

                                            Rectangle {
                                                anchors.right: parent.right
                                                anchors.top: parent.top
                                                anchors.bottom: parent.bottom
                                                width: 1
                                                color: Theme.border
                                            }
                                        }

                                        Item {
                                            width: root.getColumnWidth(2)
                                            height: parent.height

                                            Text {
                                                text: (modelData && modelData.cpu !== undefined) ? modelData.cpu : ""
                                                anchors.fill: parent
                                                anchors.leftMargin: root.cellPadding
                                                anchors.rightMargin: root.cellPadding
                                                verticalAlignment: Text.AlignVCenter
                                                color: Theme.textPrimary
                                                font.pixelSize: 13
                                                elide: Text.ElideRight
                                            }

                                            Rectangle {
                                                anchors.right: parent.right
                                                anchors.top: parent.top
                                                anchors.bottom: parent.bottom
                                                width: 1
                                                color: Theme.border
                                            }
                                        }

                                        Item {
                                            width: root.getColumnWidth(3)
                                            height: parent.height

                                            Text {
                                                text: (modelData && modelData.ram !== undefined) ? modelData.ram : ""
                                                anchors.fill: parent
                                                anchors.leftMargin: root.cellPadding
                                                anchors.rightMargin: root.cellPadding
                                                verticalAlignment: Text.AlignVCenter
                                                color: Theme.textPrimary
                                                font.pixelSize: 13
                                                elide: Text.ElideRight
                                            }

                                            Rectangle {
                                                anchors.right: parent.right
                                                anchors.top: parent.top
                                                anchors.bottom: parent.bottom
                                                width: 1
                                                color: Theme.border
                                            }
                                        }

                                        Item {
                                            width: root.getColumnWidth(4)
                                            height: parent.height

                                            Text {
                                                text: (modelData && modelData.disk !== undefined) ? modelData.disk : ""
                                                anchors.fill: parent
                                                anchors.leftMargin: root.cellPadding
                                                anchors.rightMargin: root.cellPadding
                                                verticalAlignment: Text.AlignVCenter
                                                color: Theme.textPrimary
                                                font.pixelSize: 13
                                                elide: Text.ElideRight
                                            }

                                            Rectangle {
                                                anchors.right: parent.right
                                                anchors.top: parent.top
                                                anchors.bottom: parent.bottom
                                                width: 1
                                                color: Theme.border
                                            }
                                        }

                                        Item {
                                            width: root.getColumnWidth(5)
                                            height: parent.height

                                            Text {
                                                text: (modelData && modelData.net !== undefined) ? modelData.net : ""
                                                anchors.fill: parent
                                                anchors.leftMargin: root.cellPadding
                                                anchors.rightMargin: root.cellPadding
                                                verticalAlignment: Text.AlignVCenter
                                                color: Theme.textPrimary
                                                font.pixelSize: 13
                                                elide: Text.ElideRight
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

    CielSquircle {
        id: borderOutline
        anchors.fill: parent
        color: "transparent"
        borderWidth: 1
        borderColor: Theme.border
        z: 10
    }
}
