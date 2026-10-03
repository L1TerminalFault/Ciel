import QtQuick
import QtQuick.Layouts
import Ciel.Ui 1.0

Item {
    id: root

    property string text: ""
    property string icon: ""

    default property alias content: subDropdown.content

    implicitWidth: contentRow.implicitWidth + 20
    implicitHeight: 32
    width: parent ? parent.width : implicitWidth

    readonly property bool parentMenuOpen: parent && parent.menuOpen !== undefined ? parent.menuOpen : true
    readonly property bool isHovered: subMouse.containsMouse || subDropdown.isOpen

    onParentMenuOpenChanged: {
        if (!parentMenuOpen) {
            openTimer.stop();
            closeTimer.stop();
            subDropdown.close();
        }
    }

    Timer {
        id: openTimer
        interval: 280
        repeat: false
        onTriggered: {
            if (subMouse.containsMouse && !subDropdown.isOpen) {
                subDropdown.open();
            }
        }
    }

    Timer {
        id: closeTimer
        interval: 280
        repeat: false
        onTriggered: {
            if (!subMouse.containsMouse && !subDropdown.isHovered && subDropdown.isOpen) {
                subDropdown.close();
            }
        }
    }

    Connections {
        target: subDropdown
        function onIsHoveredChanged() {
            if (subDropdown.isHovered) {
                closeTimer.stop();
            } else if (!subMouse.containsMouse && subDropdown.isOpen) {
                closeTimer.restart();
            }
        }
    }

    CielSquircle {
        anchors.fill: parent
        color: Theme.border
        borderWidth: 0
        opacity: root.isHovered ? 0.45 : 0.0

        Behavior on opacity {
            CielSpring {
                damping: 0.35
                spring: 7.0
                mass: 0.8
                epsilon: 0.001
            }
        }
    }

    RowLayout {
        id: contentRow
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 8
        spacing: 8

        CielIcon {
            icon: root.icon
            size: Theme.SMALL
            visible: root.icon.length > 0
            color: Theme.textPrimary
            Layout.alignment: Qt.AlignVCenter
        }

        Text {
            text: root.text
            font.pixelSize: 13
            font.weight: Font.Normal
            color: Theme.textPrimary
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: true
            elide: Text.ElideRight
        }

        CielIcon {
            icon: "caret-right"
            size: Theme.XSMALL
            color: root.isHovered ? Theme.textPrimary : Theme.textSecondary
            Layout.alignment: Qt.AlignVCenter
        }
    }

    MouseArea {
        id: subMouse
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor

        onEntered: {
            closeTimer.stop();
            if (!subDropdown.isOpen) {
                openTimer.restart();
            }
        }

        onExited: {
            openTimer.stop();
            if (subDropdown.isOpen) {
                closeTimer.restart();
            }
        }

        onClicked: {
            openTimer.stop();
            closeTimer.stop();
            if (!subDropdown.isOpen) {
                subDropdown.open();
            }
        }
    }

    CielDropDown {
        id: subDropdown
        trigger: root
        placement: "right"
    }
}
