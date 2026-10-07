import QtQuick
import QtQuick.Layouts
import Ciel.Ui 1.0

Item {
    id: root

    property string text: ""
    property string icon: ""
    property string trailingIcon: ""
    property bool enabled: true

    signal clicked
    signal triggered

    default property alias content: subDropdown.content
    readonly property alias isOpen: subDropdown.isOpen

    function open() {
        openTimer.stop();
        closeTimer.stop();
        subDropdown.open();
    }

    function close() {
        openTimer.stop();
        closeTimer.stop();
        subDropdown.close();
    }

    function toggle() {
        openTimer.stop();
        closeTimer.stop();
        subDropdown.toggle();
    }

    implicitWidth: contentRow.implicitWidth + 20
    implicitHeight: 32
    width: parent ? parent.width : implicitWidth

    readonly property bool parentMenuOpen: parent && parent.menuOpen !== undefined ? parent.menuOpen : true
    readonly property bool isHovered: subMouse.containsMouse || subDropdown.isOpen || subDropdown.isHovered

    onParentMenuOpenChanged: {
        if (!parentMenuOpen) {
            root.close();
        }
    }

    Timer {
        id: openTimer
        interval: 180
        repeat: false
        onTriggered: {
            if (subMouse.containsMouse && !subDropdown.isOpen) {
                subDropdown.open();
            }
        }
    }

    Timer {
        id: closeTimer
        interval: 450
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

        Item {
            implicitWidth: Theme.SMALL
            implicitHeight: Theme.SMALL
            Layout.preferredWidth: Theme.SMALL
            Layout.preferredHeight: Theme.SMALL
            Layout.alignment: Qt.AlignVCenter

            CielIcon {
                anchors.centerIn: parent
                icon: root.icon
                size: Theme.SMALL
                visible: root.icon.length > 0
                color: root.enabled ? Theme.textPrimary : Theme.textSecondary
            }
        }

        Text {
            text: root.text
            font.pixelSize: 13
            font.weight: Font.Normal
            color: root.enabled ? Theme.textPrimary : Theme.textSecondary
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: true
            elide: Text.ElideRight
        }

        CielIcon {
            icon: root.trailingIcon
            size: Theme.XSMALL
            visible: root.trailingIcon.length > 0
            color: Theme.accent
            Layout.alignment: Qt.AlignVCenter
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
        cursorShape: root.enabled ? Qt.PointingHandCursor : Qt.ArrowCursor

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
            if (!root.enabled)
                return;

            openTimer.stop();
            closeTimer.stop();
            subDropdown.toggle();
            root.clicked();
            root.triggered();
        }
    }

    CielDropDown {
        id: subDropdown
        trigger: root
        placement: "right"
        modalOverlay: false
        zIndex: 100005
    }
}
