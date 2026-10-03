import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import Ciel.Ui

Item {
    id: root

    property alias model: internalList.model
    property var columnHeaders: []
    property var columnRoles: []
    property var columnWidths: []

    property real rowHeight: 32
    property real headerHeight: rowHeight * 2
    property color rowColor: Theme.transparent
    property real cellPadding: Theme.metrics.paddingSm ?? 8
    property real radius: Theme.metrics.radiusMd ?? 12

    property Component customRowDelegate: null

    implicitWidth: 400
    implicitHeight: 300

    function getColumnWidth(col) {
        if (columnWidths && col < columnWidths.length && columnWidths[col] > 0) {
            return columnWidths[col];
        }
        if (columnHeaders && columnHeaders.length > 0) {
            return Math.max(80, root.width / columnHeaders.length);
        }
        return 100;
    }

    function setColumnWidth(col, newWidth) {
        let widths = (columnWidths && columnWidths.length > 0) ? columnWidths.slice() : new Array(columnHeaders.length).fill(root.width / (columnHeaders.length || 1));
        widths[col] = Math.max(40, newWidth);
        columnWidths = widths;
    }

    function getCellText(rowIdx, colIdx, rowModel) {
        if (root.columnRoles && colIdx < root.columnRoles.length) {
            let roleName = root.columnRoles[colIdx];
            if (rowModel && rowModel[roleName] !== undefined) {
                return rowModel[roleName];
            }
        }
        if (root.model && typeof root.model.data === "function" && typeof root.model.index === "function") {
            let val = root.model.data(root.model.index(rowIdx, colIdx), 0);
            if (val !== undefined && val !== null) {
                return val;
            }
        }
        return "";
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
        color: "black"
    }

    Item {
        id: headerContainer
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: root.columnHeaders.length > 0 ? root.headerHeight : 0
        visible: root.columnHeaders.length > 0
        clip: true

        Row {
            id: headerRow
            x: -internalList.contentX
            height: parent.height

            Repeater {
                model: root.columnHeaders.length

                Item {
                    id: headerCell
                    width: root.getColumnWidth(index)
                    height: headerContainer.height

                    Text {
                        anchors.fill: parent
                        anchors.leftMargin: root.cellPadding
                        anchors.rightMargin: root.cellPadding
                        verticalAlignment: Text.AlignVCenter
                        color: Theme.textSecondary ?? Theme.textPrimary
                        font.pixelSize: 13
                        font.weight: Font.DemiBold
                        text: root.columnHeaders[index]
                        elide: Text.ElideRight
                    }

                    Rectangle {
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        width: 1
                        height: parent.height * 0.4
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

    ListView {
        id: internalList
        anchors.top: headerContainer.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        clip: true
        boundsBehavior: Flickable.StopAtBounds

        displaced: Transition {
            NumberAnimation {
                properties: "y"
                duration: 350
                easing.type: Easing.OutCubic
            }
        }

        move: Transition {
            NumberAnimation {
                properties: "y"
                duration: 350
                easing.type: Easing.OutCubic
            }
        }

        add: Transition {
            NumberAnimation {
                properties: "y"
                duration: 250
                easing.type: Easing.OutQuad
            }
        }

        delegate: Loader {
            id: rowLoader
            width: internalList.width
            height: root.rowHeight

            sourceComponent: root.customRowDelegate ? root.customRowDelegate : defaultRowComponent

            property var rowModel: model
            property int rowIndex: index
        }

        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AsNeeded
            active: internalList.activeFocus
        }
    }

    Component {
        id: defaultRowComponent

        Rectangle {
            id: rowRect
            width: parent.width
            height: root.rowHeight
            color: root.rowColor

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                height: 1
                color: Theme.border
            }

            Row {
                anchors.fill: parent

                Repeater {
                    model: root.columnHeaders.length > 0 ? root.columnHeaders.length : (root.columnWidths.length || 1)

                    Item {
                        id: cellItem
                        width: root.getColumnWidth(index)
                        height: rowRect.height

                        Text {
                            text: root.getCellText(rowIndex, index, rowModel)
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
                            anchors.verticalCenter: parent.verticalCenter
                            width: 1
                            height: parent.height * 0.5
                            color: Theme.border
                            visible: index < (root.columnHeaders.length - 1)
                        }
                    }
                }
            }
        }
    }

    CielSquircle {
        id: borderOutline
        anchors.fill: parent
        color: Theme.transparent
        borderWidth: 1
        borderColor: Theme.border
        z: 10
    }
}
