import QtQuick
import QtQuick.Layouts
import Ciel.Ui 1.0

Item {
    id: root

    property var model: []
    property int currentIndex: 0
    readonly property string currentText: (model && model.length > currentIndex && currentIndex >= 0) ? model[currentIndex] : ""

    signal activated(int index, string text)

    implicitWidth: selectLayout.implicitWidth + 24
    implicitHeight: 34

    readonly property bool hovered: mouseArea.containsMouse
    property bool isOpen: false

    readonly property real pressScaleTarget: mouseArea.pressed ? Math.max(0.984, 1.0 - (3.2 / Math.max(root.width, 1.0))) : 1.0
    property real currentScale: 1.0

    Behavior on currentScale {
        CielSpring {
            damping: 0.24
            spring: 7.4
            mass: 0.7
            epsilon: 0.001
        }
    }

    scale: currentScale

    property real chevronRotation: 0.0

    Behavior on chevronRotation {
        CielSpring {
            damping: 0.26
            spring: 6.6
            mass: 0.75
            epsilon: 0.001
        }
    }

    function open() {
        if (!isOpen) {
            updateCoordinates();
            root.chevronRotation = 180.0;
            isOpen = true;
        }
    }

    function close() {
        if (isOpen) {
            root.chevronRotation = 0.0;
            isOpen = false;
        }
    }

    function toggle() {
        if (isOpen)
            close();
        else
            open();
    }

    function updateCoordinates() {
        if (!root.Window.window || !root.Window.window.contentItem)
            return;

        var win = root.Window.window;
        var triggerPos = root.mapToItem(win.contentItem, 0, 0);
        var margin = 8;
        var spacing = 4;

        var targetX = triggerPos.x;
        var targetY = triggerPos.y + root.height + spacing;

        if (targetY + menuCard.height > win.height - margin) {
            targetY = triggerPos.y - menuCard.height - spacing;
        }

        targetX = Math.max(margin, Math.min(win.width - root.width - margin, targetX));
        targetY = Math.max(margin, Math.min(win.height - menuCard.height - margin, targetY));

        menuCard.x = targetX;
        menuCard.y = targetY;
        menuCard.opensUp = targetY < triggerPos.y;
    }

    CielSquircle {
        id: selectBg
        anchors.fill: parent
        color: "transparent"
        borderWidth: 1
        borderColor: Theme.border
    }

    RowLayout {
        id: selectLayout
        anchors.fill: parent
        anchors.leftMargin: 12
        anchors.rightMargin: 10
        spacing: 8

        Text {
            text: root.currentText
            font.pixelSize: 12
            font.weight: Font.Medium
            color: Theme.textPrimary
            elide: Text.ElideRight
            Layout.fillWidth: true
        }

        Item {
            Layout.preferredWidth: 16
            Layout.preferredHeight: 16
            Layout.alignment: Qt.AlignVCenter

            CielIcon {
                id: chevronIcon
                anchors.centerIn: parent
                icon: "caret-down"
                size: Theme.XSMALL
                color: Theme.textSecondary

                transform: Rotation {
                    origin.x: chevronIcon.width / 2
                    origin.y: chevronIcon.height / 2
                    angle: root.chevronRotation
                }
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onPressed: {
            root.currentScale = Qt.binding(function () {
                return root.pressScaleTarget;
            });
        }
        onReleased: {
            root.currentScale = 1.0;
        }
        onCanceled: {
            root.currentScale = 1.0;
        }
        onClicked: root.toggle()
    }

    Item {
        id: overlayArea
        parent: root.Window.window ? root.Window.window.contentItem : root
        anchors.fill: parent
        visible: root.isOpen || menuCard.opacity > 0.0
        z: 99999

        property real openProgress: root.isOpen ? 1.0 : 0.0

        Behavior on openProgress {
            CielSpring {
                damping: 0.30
                spring: 5.6
                mass: 0.95
                epsilon: 0.001
            }
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: false
            onPressed: root.close()
        }

        Item {
            id: menuCard
            property bool opensUp: false

            width: root.width
            height: menuColumn.implicitHeight + 12

            opacity: Math.min(1.0, overlayArea.openProgress * 1.8)

            transform: Scale {
                origin.x: menuCard.width / 2
                origin.y: menuCard.opensUp ? menuCard.height : 0
                xScale: 1.0
                yScale: 0.88 + (overlayArea.openProgress * 0.12)
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

                Repeater {
                    model: root.model

                    CielMenuItem {
                        width: menuColumn.width
                        text: modelData
                        icon: ""
                        onTriggered: {
                            root.currentIndex = index;
                            root.activated(index, modelData);
                            root.close();
                        }
                    }
                }
            }
        }
    }
}
