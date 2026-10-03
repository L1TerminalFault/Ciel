import QtQuick
import QtQuick.Layouts
import Ciel.Ui

Item {
    id: root

    property int notificationId: 0
    property string appName: ""
    property string summary: ""
    property string body: ""

    readonly property real startY: -18
    readonly property real startXScale: 0.78
    readonly property real startYScale: 0.82

    readonly property real harmonicSpring: 4.2
    readonly property real motionMass: 1.0
    readonly property real dampingY: 0.36
    readonly property real dampingX: 0.28
    readonly property real dampingPosY: 0.36

    readonly property int fadeInDuration: 300
    readonly property int closeDuration: 130

    width: parent ? parent.width : 390
    height: popupContent.height + 4

    property bool isHovered: rootMouseArea.containsMouse
    property bool isClosing: false

    property int secondsLeft: 5

    Timer {
        id: countdownTimer
        interval: 1000
        repeat: true
        running: !root.isHovered && !root.isClosing
        onTriggered: {
            root.secondsLeft -= 1;
            if (root.secondsLeft <= 0) {
                countdownTimer.stop();
                root.close();
            }
        }
    }

    onIsHoveredChanged: {
        if (!root.isHovered && root.secondsLeft < 3) {
            root.secondsLeft = 3;
        }
    }

    function close() {
        if (root.isClosing)
            return;
        root.isClosing = true;
        countdownTimer.stop();
        closeAnim.restart();
    }

    Component.onCompleted: openAnim.restart()

    MouseArea {
        id: rootMouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        z: 10
        enabled: !root.isClosing
        onClicked: root.close()
    }

    Item {
        id: popupContent
        width: parent.width - 34
        height: cardSurface.implicitHeight
        anchors.horizontalCenter: parent.horizontalCenter
        y: root.startY
        opacity: 0.0

        transformOrigin: Item.Top

        transform: Scale {
            id: scaleTransform
            origin.x: popupContent.width / 2
            origin.y: 0
            xScale: root.startXScale
            yScale: root.startYScale
        }

        CielSquircle {
            id: cardSurface
            anchors.fill: parent
            radius: Theme.metrics.radiusLg
            color: Theme.surface

            implicitHeight: Math.max(62, innerGrid.implicitHeight + (Theme.metrics.spacingMd * 2))

            RowLayout {
                id: innerGrid
                anchors.fill: parent
                anchors.margins: Theme.metrics.spacingMd
                spacing: Theme.metrics.spacingMd

                // Left app initial badge
                CielSquircle {
                    Layout.alignment: Qt.AlignTop
                    Layout.preferredWidth: 38
                    Layout.preferredHeight: 38
                    radius: 12
                    color: Theme.surfaceHover

                    Text {
                        anchors.centerIn: parent
                        text: root.appName.length > 0 ? root.appName.substring(0, 1).toUpperCase() : "•"
                        font.pixelSize: 14
                        font.weight: Font.Bold
                        color: Theme.textSecondary
                        renderType: Text.NativeRendering
                    }
                }

                // Middle summary & body
                ColumnLayout {
                    Layout.alignment: Qt.AlignTop
                    Layout.fillWidth: true
                    spacing: Theme.metrics.spacingXs

                    Text {
                        text: root.summary
                        font.pixelSize: 13
                        font.weight: Font.SemiBold
                        color: Theme.textPrimary
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                        renderType: Text.NativeRendering
                    }

                    Text {
                        text: root.body
                        font.pixelSize: 12
                        color: Theme.textSecondary
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                        visible: text.length > 0
                        renderType: Text.NativeRendering
                    }
                }

                // Right side: only the clean "now" indicator
                ColumnLayout {
                    Layout.alignment: Qt.AlignTop | Qt.AlignRight
                    spacing: 0

                    Text {
                        text: "now"
                        font.pixelSize: 10
                        color: Theme.textSecondary
                        Layout.alignment: Qt.AlignRight
                        renderType: Text.NativeRendering
                    }
                }
            }
        }
    }

    SequentialAnimation {
        id: openAnim

        PauseAnimation {
            duration: 80
        }

        ParallelAnimation {
            NumberAnimation {
                target: popupContent
                property: "opacity"
                from: 0.0
                to: 1.0
                duration: root.fadeInDuration
                easing.type: Easing.OutCubic
            }

            SpringAnimation {
                target: popupContent
                property: "y"
                from: root.startY
                to: 0
                spring: root.harmonicSpring
                damping: root.dampingPosY
                mass: root.motionMass
                epsilon: 0.001
            }

            SpringAnimation {
                target: scaleTransform
                property: "yScale"
                from: root.startYScale
                to: 1.0
                spring: root.harmonicSpring
                damping: root.dampingY
                mass: root.motionMass
                epsilon: 0.001
            }

            SpringAnimation {
                target: scaleTransform
                property: "xScale"
                from: root.startXScale
                to: 1.0
                spring: root.harmonicSpring
                damping: root.dampingX
                mass: root.motionMass
                epsilon: 0.001
            }
        }
    }

    SequentialAnimation {
        id: closeAnim

        ParallelAnimation {
            NumberAnimation {
                target: popupContent
                property: "opacity"
                to: 0.0
                duration: root.closeDuration
                easing.type: Easing.InQuad
            }

            NumberAnimation {
                target: root
                property: "height"
                to: 0
                duration: root.closeDuration + 10
                easing.type: Easing.OutQuad
            }
        }

        ScriptAction {
            script: NotificationsModel.dismiss(root.notificationId)
        }
    }
}
