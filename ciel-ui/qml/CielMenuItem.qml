import QtQuick
import QtQuick.Layouts
import Ciel.Ui 1.0

Item {
    id: root

    property string text: ""
    property string icon: ""
    property string shortcut: ""
    property bool destructive: false
    property bool isEnabled: true

    signal triggered

    implicitWidth: contentRow.implicitWidth + 20
    implicitHeight: 32
    width: parent ? parent.width : implicitWidth

    readonly property bool isHovered: itemMouse.containsMouse && root.isEnabled

    CielSquircle {
        anchors.fill: parent
        color: root.destructive ? Theme.border : Theme.background
        borderWidth: 0
        opacity: root.isHovered ? 0.75 : 0.0

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
        anchors.rightMargin: 10
        spacing: 8

        Item {
            implicitWidth: Theme.SMALL
            implicitHeight: Theme.SMALL
            Layout.preferredWidth: Theme.SMALL
            Layout.preferredHeight: Theme.SMALL
            Layout.alignment: Qt.AlignVCenter

            CielIcon {
                id: iconItem
                anchors.centerIn: parent
                icon: root.icon
                size: Theme.SMALL
                visible: root.icon.length > 0
                color: root.destructive ? Theme.textPrimary : (root.isEnabled ? Theme.textPrimary : Theme.textSecondary)
            }
        }

        Text {
            text: root.text
            font.pixelSize: 13
            font.weight: Font.Normal
            color: root.destructive ? Theme.textPrimary : (root.isEnabled ? Theme.textPrimary : Theme.textSecondary)
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: true
            elide: Text.ElideRight
        }

        Text {
            text: root.shortcut
            font.pixelSize: 11
            color: Theme.textSecondary
            visible: root.shortcut.length > 0
            Layout.alignment: Qt.AlignVCenter
        }
    }

    MouseArea {
        id: itemMouse
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: root.isEnabled ? Qt.PointingHandCursor : Qt.ArrowCursor
        onClicked: {
            if (root.isEnabled) {
                root.triggered();
                var p = root.parent;
                while (p) {
                    if (p.dropdown) {
                        p.dropdown.close();
                        break;
                    }
                    p = p.parent;
                }
            }
        }
    }
}
