import QtQuick
import QtQuick.Layouts
import Ciel.Ui 1.0
import QtQuick

Item {
    id: root
    
    parent: root.Window.window ? root.Window.window.contentItem : null
    anchors.fill: parent
    
    property string text: ""
    property Item trigger: null
    property real requestedX: 0
    property real requestedY: 0
    property bool useAbsoluteCoordinates: false
    
    property bool isOpen: false
    property int zIndex: 100000
    
    property real openProgress: isOpen ? 1.0 : 0.0

    Behavior on openProgress {
        CielSpring {
            damping: 0.35
            spring: 5.5
            mass: 0.9
            epsilon: 0.001
        }
    }

    function open() {
            updateCoordinates();
            isOpen = true;
    }

    Timer {
        id: closeDelay
        interval: 60
        onTriggered: {
            root.isOpen = false;
            root.useAbsoluteCoordinates = false;
        }
    }

    property int openDelay: 500

    Timer {
        id: openTimer
        interval: root.openDelay
        onTriggered: {
            updateCoordinates();
            root.isOpen = true;
        }
    }

    function openAt(absoluteX, absoluteY) {
        root.requestedX = absoluteX;
        root.requestedY = absoluteY;
        root.useAbsoluteCoordinates = true;

        if (isOpen) return; 

        
        if (closeDelay.running) {
            closeDelay.stop();
        }

        
        if (!openTimer.running) {
            openTimer.start();
        }
    }

    
    function close() {
        openTimer.stop(); 
        closeDelay.restart(); 
    }

    function updateCoordinates() {
        if (!root.Window.window) return;

        if (root.useAbsoluteCoordinates) {
            var leftMargin = 8;
            var rightMargin = 8;
            var bottomMargin = 8;
            var topMargin = 50; 

            
            var maxX = root.width - menuCard.width - rightMargin;
            var maxY = root.height - menuCard.height - bottomMargin;
            
            
            menuCard.x = Math.max(leftMargin, Math.min(root.requestedX, maxX));
            menuCard.y = Math.max(topMargin, Math.min(root.requestedY, maxY));
            
            menuCard.transformOrigin = Item.TopLeft;
        }
    }

    visible: isOpen || menuCard.opacity > 0.0
    z: root.zIndex

    Item {
        id: menuCard
        
        width: Math.max(60, tooltipText.implicitWidth + 24)
        height: tooltipText.implicitHeight + 16
        
        scale: 0.94 + (root.openProgress * 0.06)
        opacity: root.openProgress

        CielSquircle {
            anchors.fill: parent
            color: Theme.surface
            borderWidth: 1
            borderColor: Theme.border
        }

        Text {
            id: tooltipText
            anchors.centerIn: parent
            anchors.margins: 8
            text: root.text
            font.pixelSize: 13
            font.weight: Font.Medium
            color: Theme.textPrimary
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}
