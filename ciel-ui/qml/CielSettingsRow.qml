import QtQuick
import QtQuick.Layouts
import Ciel.Ui

Item {
    id: root

    property string title: ""
    property string subtitle: ""

    property bool clickable: false
    property string detailText: ""
    property bool showChevron: false
    signal clicked

    property bool showDivider: true
    property real horizontalPadding: 16
    property real verticalPadding: 12

    default property alias rightContent: rightSlot.data

    Layout.fillWidth: true
    implicitHeight: Math.max(48, mainRow.implicitHeight + verticalPadding * 2)

    Rectangle {
        id: rowHighlight
        anchors.fill: parent
        anchors.bottomMargin: root.showDivider ? 1 : 0
        color: Theme.background
        opacity: {
            if (!root.clickable)
                return 0.0;
            if (mouseArea.pressed)
                return 0.8;
            if (mouseArea.containsMouse)
                return 0.5;
            return 0.0;
        }

        Behavior on opacity {
            NumberAnimation {
                duration: 80
                easing.type: Easing.OutQuad
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        enabled: root.clickable
        hoverEnabled: root.clickable
        cursorShape: root.clickable ? Qt.PointingHandCursor : Qt.ArrowCursor
        onClicked: root.clicked()
    }

    RowLayout {
        id: mainRow
        anchors.fill: parent
        anchors.leftMargin: root.horizontalPadding
        anchors.rightMargin: root.horizontalPadding
        spacing: 12

        ColumnLayout {
            id: textContainer
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter
            spacing: 2

            Text {
                text: root.title
                color: Theme.textPrimary
                font.pixelSize: 13
                font.weight: Font.Medium
                elide: Text.ElideRight
                Layout.fillWidth: true
            }

            Text {
                visible: root.subtitle !== ""
                text: root.subtitle
                color: Theme.textSecondary
                font.pixelSize: 12
                elide: Text.ElideRight
                Layout.fillWidth: true
            }
        }

        RowLayout {
            id: rightSlot
            Layout.alignment: Qt.AlignVCenter
            spacing: 8

            Text {
                visible: root.detailText !== ""
                text: root.detailText
                color: Theme.textSecondary
                font.pixelSize: 13
                Layout.alignment: Qt.AlignVCenter
            }

            Item {
                visible: root.showChevron
                Layout.preferredWidth: 14
                Layout.preferredHeight: 14
                Layout.alignment: Qt.AlignVCenter

                property real nudge: (root.clickable && mouseArea.pressed) ? 3.0 : 0.0

                Behavior on nudge {
                    CielSpring {
                        mass: 1.0
                        spring: 22.0
                        damping: 5.5
                        epsilon: 0.001
                    }
                }

                CielIcon {
                    x: parent.nudge
                    anchors.verticalCenter: parent.verticalCenter
                    icon: "caret-right"
                    size: 13
                    color: Theme.textSecondary
                }
            }
        }
    }

    Rectangle {
        id: bottomBorder
        visible: root.showDivider
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: 1
        color: Theme.border
    }
}
