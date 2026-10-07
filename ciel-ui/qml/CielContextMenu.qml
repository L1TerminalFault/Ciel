import QtQuick
import QtQuick.Layouts
import Ciel.Ui 1.0

Item {
    id: root

    property bool isOpen: false
    property int zIndex: 99999

    default property alias content: menuColumn.data

    visible: overlayArea.visible

    function popup(x, y, callerItem) {
        if (!root.Window.window)
            return;

        var win = root.Window.window;
        var source = callerItem !== undefined ? callerItem : root.parent;
        var globalPos = source ? source.mapToItem(win.contentItem, x, y) : Qt.point(x, y);

        var margin = 8;
        var targetX = globalPos.x;
        var targetY = globalPos.y;

        if (targetX + menuCard.width > win.width - margin) {
            targetX = globalPos.x - menuCard.width;
        }

        if (targetY + menuCard.height > win.height - margin) {
            targetY = globalPos.y - menuCard.height;
        }

        targetX = Math.max(margin, Math.min(win.width - menuCard.width - margin, targetX));
        targetY = Math.max(margin, Math.min(win.height - menuCard.height - margin, targetY));

        menuCard.x = targetX;
        menuCard.y = targetY;

        var opensUp = targetY < globalPos.y;
        var opensLeft = targetX < globalPos.x;

        if (opensUp && opensLeft) {
            menuCard.transformOrigin = Item.BottomRight;
        } else if (opensUp && !opensLeft) {
            menuCard.transformOrigin = Item.BottomLeft;
        } else if (!opensUp && opensLeft) {
            menuCard.transformOrigin = Item.TopRight;
        } else {
            menuCard.transformOrigin = Item.TopLeft;
        }

        isOpen = true;
    }

    function close() {
        isOpen = false;
    }

    Item {
        id: overlayArea
        parent: root.Window.window ? root.Window.window.contentItem : root
        anchors.fill: parent
        visible: root.isOpen || menuCard.opacity > 0.0
        z: root.zIndex

        property real openProgress: root.isOpen ? 1.0 : 0.0

        Behavior on openProgress {
            CielSpring {
                damping: 0.32
                spring: 5.2
                mass: 1.0
                epsilon: 0.001
            }
        }

        MouseArea {
            id: backdropMouse
            anchors.fill: parent
            hoverEnabled: false
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onPressed: function (mouse) {
                var p = menuCard.mapFromItem(backdropMouse, mouse.x, mouse.y);
                if (p.x >= 0 && p.x <= menuCard.width && p.y >= 0 && p.y <= menuCard.height) {
                    mouse.accepted = false;
                    return;
                }
                root.close();
            }
        }

        Item {
            id: menuCard
            width: Math.max(200, menuColumn.implicitWidth + 12)
            height: menuColumn.implicitHeight + 12
            scale: 0.94 + (overlayArea.openProgress * 0.06)
            opacity: Math.min(1.0, overlayArea.openProgress * 1.8)

            CielSquircle {
                anchors.fill: parent
                color: Theme.surface
                borderWidth: 1
                borderColor: Theme.border
            }

            Column {
                id: menuColumn
                property bool menuOpen: root.isOpen
                anchors.fill: parent
                anchors.margins: 6
                spacing: 2
            }
        }
    }
}
