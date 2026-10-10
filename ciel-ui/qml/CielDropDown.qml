import QtQuick
import QtQuick.Layouts
import Ciel.Ui 1.0

Item {
    id: root

    property Item trigger: null
    property string placement: "bottom"
    property bool isOpen: false
    property bool modalOverlay: true
    property int zIndex: 99999

    default property alias content: menuColumn.data

    readonly property bool isHovered: cardHover.hovered

    visible: overlayArea.visible

    function open() {
        if (!isOpen) {
            updateCoordinates();
            isOpen = true;
        }
    }

    function close() {
        isOpen = false;
    }

    function toggle() {
        if (isOpen)
            close();
        else
            open();
    }

    function updateCoordinates() {
        if (!trigger || !root.Window.window)
            return;

        var win = root.Window.window;
        var triggerPos = trigger.mapToItem(win.contentItem, 0, 0);
        var margin = 8;
        var spacing = 4;

        var targetX = triggerPos.x;
        var targetY = triggerPos.y;
        var activePlacement = root.placement;

        if (activePlacement === "bottom") {
            targetY = triggerPos.y + trigger.height + spacing;
            if (targetY + menuCard.height > win.height - margin) {
                targetY = triggerPos.y - menuCard.height - spacing;
            }
        } else if (activePlacement === "top") {
            targetY = triggerPos.y - menuCard.height - spacing;
            if (targetY < margin) {
                targetY = triggerPos.y + trigger.height + spacing;
            }
        } else if (activePlacement === "right") {
            targetX = triggerPos.x + trigger.width + spacing;
            targetY = triggerPos.y;
            if (targetX + menuCard.width > win.width - margin) {
                targetX = triggerPos.x - menuCard.width - spacing;
            }
        } else if (activePlacement === "left") {
            targetX = triggerPos.x - menuCard.width - spacing;
            targetY = triggerPos.y;
            if (targetX < margin) {
                targetX = triggerPos.x + trigger.width + spacing;
            }
        }

        targetX = Math.max(margin, Math.min(win.width - menuCard.width - margin, targetX));
        targetY = Math.max(margin, Math.min(win.height - menuCard.height - margin, targetY));

        menuCard.x = targetX;
        menuCard.y = targetY;

        var triggerCenterX = triggerPos.x + (trigger.width / 2);
        var triggerCenterY = triggerPos.y + (trigger.height / 2);
        var menuCenterX = targetX + (menuCard.width / 2);
        var menuCenterY = targetY + (menuCard.height / 2);

        var opensUp = menuCenterY < triggerCenterY;
        var opensLeft = menuCenterX < triggerCenterX;

        if (opensUp && opensLeft) {
            menuCard.transformOrigin = Item.BottomRight;
        } else if (opensUp && !opensLeft) {
            menuCard.transformOrigin = Item.BottomLeft;
        } else if (!opensUp && opensLeft) {
            menuCard.transformOrigin = Item.TopRight;
        } else {
            menuCard.transformOrigin = Item.TopLeft;
        }
    }

    Connections {
        target: root.modalOverlay ? root.trigger : null
        ignoreUnknownSignals: true
        function onClicked() {
            root.toggle();
        }
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
            enabled: root.modalOverlay
            hoverEnabled: false
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
            width: Math.max(260, menuColumn.implicitWidth + 12)
            height: menuColumn.implicitHeight + 12
            scale: 0.94 + (overlayArea.openProgress * 0.06)
            opacity: Math.min(1.0, overlayArea.openProgress * 1.8)

            HoverHandler {
                id: cardHover
            }

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
