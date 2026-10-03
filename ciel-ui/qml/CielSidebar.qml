import QtQuick
import QtQuick.Layouts
import Ciel.Ui

Item {
    id: root

    property alias model: navList.model
    property int currentIndex: 0
    property bool collapsed: false
    property real headerHeight: 52

    property bool showBottomAction: false
    property string bottomActionTitle: ""
    property string bottomActionIcon: ""
    property bool bottomActionSelected: false

    signal itemSelected(int index)
    signal bottomActionTriggered

    implicitWidth: collapsed ? 56 : 200
    implicitHeight: 400

    Behavior on implicitWidth {
        CielSpring {
            damping: 5.0
            spring: 6.0
            mass: 4.8
            epsilon: 0.005
        }
    }

    Item {
        id: menuHeaderArea
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: root.headerHeight

        CielIconButton {
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            icon: "list"
            iconRotation: root.collapsed ? 90 : 0
            onClicked: root.collapsed = !root.collapsed
        }
    }

    ListView {
        id: navList
        anchors.top: menuHeaderArea.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: root.showBottomAction ? bottomActionArea.top : parent.bottom
        anchors.bottomMargin: 8
        spacing: 4
        interactive: false
        boundsBehavior: Flickable.StopAtBounds

        delegate: Item {
            id: delegateRoot
            width: navList.width
            height: 38

            readonly property bool isSelected: !root.bottomActionSelected && root.currentIndex === index
            readonly property bool isHovered: mouseArea.containsMouse

            scale: mouseArea.pressed ? 0.98 : 1.0
            layer.enabled: scaleSpring.running
            layer.smooth: true
            transformOrigin: Item.Center

            Behavior on scale {
                CielSpring {
                    id: scaleSpring
                    damping: 4.85
                    spring: 8.0
                    mass: 4
                    epsilon: 0.01
                }
            }

            CielSquircle {
                id: pillBackground
                anchors.fill: parent
                anchors.leftMargin: 8
                anchors.rightMargin: 8
                color: delegateRoot.isSelected ? Theme.surface : Theme.background

                Behavior on color {
                    ColorAnimation {
                        duration: 100
                    }
                }
            }

            Item {
                id: iconContainer
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: 56

                CielIcon {
                    anchors.centerIn: parent
                    icon: modelData.icon
                    opacity: delegateRoot.isSelected ? 1.0 : (delegateRoot.isHovered ? 0.9 : 0.7)

                    Behavior on opacity {
                        NumberAnimation {
                            duration: 80
                        }
                    }
                }
            }

            Text {
                anchors.left: iconContainer.right
                anchors.right: parent.right
                anchors.rightMargin: 12
                anchors.verticalCenter: parent.verticalCenter
                clip: true
                opacity: root.collapsed ? 0.0 : 1.0
                text: modelData.title
                font.pixelSize: 13
                font.weight: delegateRoot.isSelected ? Font.DemiBold : Font.Normal
                color: delegateRoot.isSelected ? Theme.textPrimary : (delegateRoot.isHovered ? Theme.textPrimary : Theme.textSecondary)
                elide: Text.ElideRight

                Behavior on opacity {
                    NumberAnimation {
                        duration: 80
                    }
                }

                Behavior on color {
                    ColorAnimation {
                        duration: 80
                    }
                }
            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    root.currentIndex = index;
                    root.itemSelected(index);
                }
            }
        }
    }

    Item {
        id: bottomActionArea
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 16
        height: root.showBottomAction ? 38 : 0
        visible: root.showBottomAction

        readonly property bool isSelected: root.bottomActionSelected
        readonly property bool isHovered: bottomMouse.containsMouse

        scale: bottomMouse.pressed ? 0.98 : 1.0
        layer.enabled: bottomScaleSpring.running
        layer.smooth: true
        transformOrigin: Item.Center

        Behavior on scale {
            CielSpring {
                id: bottomScaleSpring
                damping: 4.85
                spring: 8.0
                mass: 4
                epsilon: 0.01
            }
        }

        CielSquircle {
            anchors.fill: parent
            anchors.leftMargin: 8
            anchors.rightMargin: 8
            color: bottomActionArea.isSelected ? Theme.surface : (bottomActionArea.isHovered ? Theme.surface : Theme.background)

            Behavior on color {
                ColorAnimation {
                    duration: 100
                }
            }
        }

        Item {
            id: bottomIconContainer
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 56

            CielIcon {
                anchors.centerIn: parent
                icon: root.bottomActionIcon
                opacity: bottomActionArea.isSelected ? 1.0 : (bottomActionArea.isHovered ? 0.9 : 0.7)

                Behavior on opacity {
                    NumberAnimation {
                        duration: 80
                    }
                }
            }
        }

        Text {
            anchors.left: bottomIconContainer.right
            anchors.right: parent.right
            anchors.rightMargin: 12
            anchors.verticalCenter: parent.verticalCenter
            clip: true
            opacity: root.collapsed ? 0.0 : 1.0
            text: root.bottomActionTitle
            font.pixelSize: 13
            font.weight: bottomActionArea.isSelected ? Font.DemiBold : Font.Normal
            color: bottomActionArea.isSelected ? Theme.textPrimary : (bottomActionArea.isHovered ? Theme.textPrimary : Theme.textSecondary)
            elide: Text.ElideRight

            Behavior on opacity {
                NumberAnimation {
                    duration: 80
                }
            }

            Behavior on color {
                ColorAnimation {
                    duration: 80
                }
            }
        }

        MouseArea {
            id: bottomMouse
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: root.bottomActionTriggered()
        }
    }
}
