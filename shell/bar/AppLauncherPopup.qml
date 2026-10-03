import Quickshell
import Quickshell.Wayland
import QtQuick
import QtQuick.Layouts
import QtQuick.Effects
import Ciel.Ui

PopupWindow {
    id: root

    property var targetWindow
    property rect anchorRect
    property bool isOpen: false

    readonly property real startY: -14
    readonly property real startXScale: 0.88
    readonly property real startYScale: 0.78

    readonly property real springY: 4.2
    readonly property real dampingY: 0.34
    readonly property real springX: 4.5
    readonly property real dampingX: 0.36
    readonly property real springPosY: 4.2
    readonly property real dampingPosY: 0.34

    readonly property int fadeInDuration: 95
    readonly property int closeDuration: 130

    function toggle() {
        isOpen = !isOpen;
    }

    onIsOpenChanged: {
        if (isOpen) {
            closeAnim.stop();
            openAnim.restart();
            searchInput.text = "";
            focusTimer.restart();
        } else {
            openAnim.stop();
            closeAnim.restart();
            searchInput.focus = false;
        }
    }

    Timer {
        id: focusTimer
        interval: 40
        onTriggered: {
            if (root.isOpen)
                searchInput.forceActiveFocus();
        }
    }

    anchor.window: targetWindow
    anchor.rect: anchorRect
    anchor.edges: Edges.Bottom
    anchor.gravity: Edges.Bottom

    implicitWidth: 690
    implicitHeight: 640
    color: "transparent"

    visible: root.isOpen || openAnim.running || closeAnim.running

    Item {
        anchors.fill: parent
        focus: true
        Keys.forwardTo: [searchInput]
        Keys.onEscapePressed: {
            root.isOpen = false;
        }
    }

    RectangularShadow {
        anchors.fill: popupContent
        radius: 22
        offset.y: 12
        blur: 42
        spread: -8
        color: "#00000080"
        opacity: popupContent.opacity
    }

    Item {
        id: popupContent
        property real radius: 22
        width: 630
        height: 580
        anchors.horizontalCenter: parent.horizontalCenter
        y: 0
        opacity: 0.0

        transform: Scale {
            id: scaleTransform
            origin.x: popupContent.width / 2
            origin.y: 0
            xScale: root.startXScale
            yScale: root.startYScale
        }

        CielSquircle {
            anchors.fill: parent
            radius: popupContent.radius
            color: Theme.surface
            borderColor: Theme.surfaceHover
            borderWidth: 1
        }

        SequentialAnimation {
            id: openAnim

            PauseAnimation {
                duration: 190
            }

            ParallelAnimation {
                SpringAnimation {
                    target: popupContent
                    property: "y"
                    from: root.startY
                    to: 0
                    spring: root.springPosY
                    damping: root.dampingPosY
                    epsilon: 0.001
                }
                SpringAnimation {
                    target: scaleTransform
                    property: "yScale"
                    from: root.startYScale
                    to: 1.0
                    spring: root.springY
                    damping: root.dampingY
                    epsilon: 0.001
                }
                SpringAnimation {
                    target: scaleTransform
                    property: "xScale"
                    from: root.startXScale
                    to: 1.0
                    spring: root.springX
                    damping: root.dampingX
                    epsilon: 0.001
                }
                NumberAnimation {
                    target: popupContent
                    property: "opacity"
                    from: 0.0
                    to: 1.0
                    duration: root.fadeInDuration
                    easing.type: Easing.OutQuad
                }
                NumberAnimation {
                    target: searchBarContainer
                    property: "opacity"
                    from: 0.0
                    to: 1.0
                    duration: 160
                    easing.type: Easing.OutCubic
                }
                NumberAnimation {
                    target: searchTranslate
                    property: "y"
                    from: 12
                    to: 0
                    duration: 200
                    easing.type: Easing.OutCubic
                }
                NumberAnimation {
                    target: appGridArea
                    property: "opacity"
                    from: 0.0
                    to: 1.0
                    duration: 180
                    easing.type: Easing.OutCubic
                }
                NumberAnimation {
                    target: appGridTranslate
                    property: "y"
                    from: 8
                    to: 0
                    duration: 220
                    easing.type: Easing.OutCubic
                }
            }
        }

        ParallelAnimation {
            id: closeAnim

            NumberAnimation {
                target: popupContent
                property: "y"
                to: root.startY
                duration: root.closeDuration
                easing.type: Easing.InQuad
            }
            NumberAnimation {
                target: scaleTransform
                property: "yScale"
                to: root.startYScale
                duration: root.closeDuration + 10
                easing.type: Easing.InQuad
            }
            NumberAnimation {
                target: scaleTransform
                property: "xScale"
                to: root.startXScale
                duration: root.closeDuration + 10
                easing.type: Easing.InQuad
            }
            NumberAnimation {
                target: popupContent
                property: "opacity"
                to: 0.0
                duration: root.closeDuration - 10
                easing.type: Easing.InQuad
            }
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 16

            Rectangle {
                id: searchBarContainer
                Layout.fillWidth: true
                height: 40
                radius: Theme.metrics.radiusMd
                color: Theme.surfaceHover
                border.color: searchInput.activeFocus ? Theme.accent : "transparent"
                border.width: 1

                transform: Translate {
                    id: searchTranslate
                    y: 0
                }

                Behavior on border.color {
                    ColorAnimation {
                        duration: 140
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.IBeamCursor
                    onClicked: searchInput.forceActiveFocus()
                }

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 12
                    anchors.rightMargin: 12
                    spacing: 10

                    // Search icon
                    Item {
                        Layout.alignment: Qt.AlignVCenter
                        width: 14
                        height: 14

                        Rectangle {
                            x: 0
                            y: 0
                            width: 9.5
                            height: 9.5
                            radius: 4.75
                            color: "transparent"
                            border.color: searchInput.activeFocus ? Theme.textPrimary : Theme.textSecondary
                            border.width: 1.5

                            Behavior on border.color {
                                ColorAnimation {
                                    duration: 140
                                }
                            }
                        }

                        Rectangle {
                            x: 8
                            y: 8
                            width: 5
                            height: 1.5
                            radius: 0.75
                            color: searchInput.activeFocus ? Theme.textPrimary : Theme.textSecondary
                            rotation: 45
                            transformOrigin: Item.TopLeft

                            Behavior on color {
                                ColorAnimation {
                                    duration: 140
                                }
                            }
                        }
                    }

                    Item {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        Text {
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            visible: !searchInput.text && !searchInput.activeFocus
                            text: "Type to search..."
                            color: Theme.textSecondary
                            font.pixelSize: 13
                            renderType: Text.NativeRendering
                        }

                        TextInput {
                            id: searchInput
                            anchors.fill: parent
                            verticalAlignment: TextInput.AlignVCenter
                            color: Theme.textPrimary
                            font.pixelSize: 13
                            clip: true
                            selectByMouse: true
                            focus: true
                        }
                    }

                    Rectangle {
                        visible: searchInput.text !== ""
                        Layout.alignment: Qt.AlignVCenter
                        width: 18
                        height: 18
                        radius: 9
                        color: clearMouse.containsMouse ? Theme.surface : "transparent"

                        Text {
                            anchors.centerIn: parent
                            text: "✕"
                            color: Theme.textSecondary
                            font.pixelSize: 9
                        }

                        MouseArea {
                            id: clearMouse
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                searchInput.text = "";
                                searchInput.forceActiveFocus();
                            }
                        }
                    }
                }
            }

            Item {
                id: appGridArea
                Layout.fillWidth: true
                Layout.fillHeight: true

                transform: Translate {
                    id: appGridTranslate
                    y: 0
                }

                AppGrid {
                    id: appGrid
                    anchors.fill: parent
                    searchFilter: searchInput.text
                    onAppLaunched: app => {
                        root.isOpen = false;
                    }
                }
            }
        }
    }
}
