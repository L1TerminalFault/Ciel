import QtQuick
import QtQuick.Layouts
import QtQuick.Effects
import Ciel.Ui

Item {
    id: root

    property Item trigger: null
    property string placement: "right"
    property bool isOpen: false
    property var downloadsModel: null

    property real targetX: 0
    property real targetY: 0
    property real birthOriginX: 0
    property real birthOriginY: 0
    property string activeDirection: "right"

    property real transitionProgress: isOpen ? 1.0 : 0.0

    readonly property real p: transitionProgress

    readonly property real surfaceProgress: {
        if (p <= 0.0)
            return 0.0;
        if (p < 1.0)
            return p * p * (3.0 - 2.0 * p);
        return p;
    }

    readonly property real contentProgress: {
        var t = Math.max(0.0, Math.min(1.0, (p - 0.10) / 0.72));
        return t * t * (3.0 - 2.0 * t);
    }

    readonly property real contentOpacity: contentProgress

    readonly property bool isHorizDir: activeDirection === "right" || activeDirection === "left"

    readonly property real travelDistance: 10.0

    readonly property real travelX: isHorizDir ? (activeDirection === "right" ? -travelDistance * (1.0 - p) : travelDistance * (1.0 - p)) : 0.0

    readonly property real travelY: !isHorizDir ? (activeDirection === "bottom" ? -travelDistance * (1.0 - p) : travelDistance * (1.0 - p)) : 0.0

    readonly property real effectiveTargetY: {
        if (!root.Window.window)
            return targetY;
        var win = root.Window.window;
        var margin = 10;
        return Math.max(margin, Math.min(win.height - menuCard.height - margin, targetY));
    }

    property real triggerNudge: 0.0

    Behavior on triggerNudge {
        CielSpring {
            spring: 4.2
            damping: 0.32
            mass: 1.0
            epsilon: 0.001
        }
    }

    SequentialAnimation {
        id: triggerImpulseAnim

        ScriptAction {
            script: root.triggerNudge = 4.0
        }
        PauseAnimation {
            duration: 110
        }
        ScriptAction {
            script: root.triggerNudge = 0.0
        }
    }

    signal openFolderRequested(string path)
    signal pauseRequested(string id)
    signal resumeRequested(string id)
    signal cancelRequested(string id)
    signal retryRequested(string id)
    signal clearFinishedRequested

    visible: overlayArea.visible

    Behavior on transitionProgress {
        CielSpring {
            spring: 5.0
            damping: 0.29
            mass: 1.0
            epsilon: 0.001
        }
    }

    function open() {
        if (isOpen)
            return;
        updateCoordinates();
        isOpen = true;
        triggerImpulseAnim.restart();
    }

    function close() {
        if (!isOpen)
            return;
        triggerImpulseAnim.stop();
        root.triggerNudge = 0.0;
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
        if (!win.contentItem)
            return;

        var triggerPos = trigger.mapToItem(win.contentItem, 0, 0);

        var margin = 10;
        var spacing = 6;

        var tCenterX = triggerPos.x + trigger.width / 2;
        var tCenterY = triggerPos.y + trigger.height / 2;

        var computedX = triggerPos.x;
        var computedY = triggerPos.y;
        var dir = root.placement;

        if (dir === "right") {
            computedX = triggerPos.x + trigger.width + spacing;
            computedY = triggerPos.y + trigger.height - menuCard.height;

            if (computedX + menuCard.width > win.width - margin) {
                computedX = triggerPos.x - menuCard.width - spacing;
                dir = "left";
            }
        } else if (dir === "bottom") {
            computedX = tCenterX - menuCard.width / 2;
            computedY = triggerPos.y + trigger.height + spacing;

            if (computedY + menuCard.height > win.height - margin) {
                computedY = triggerPos.y - menuCard.height - spacing;
                dir = "top";
            }
        } else if (dir === "left") {
            computedX = triggerPos.x - menuCard.width - spacing;
            computedY = triggerPos.y + trigger.height - menuCard.height;

            if (computedX < margin) {
                computedX = triggerPos.x + trigger.width + spacing;
                dir = "right";
            }
        } else if (dir === "top") {
            computedX = tCenterX - menuCard.width / 2;
            computedY = triggerPos.y - menuCard.height - spacing;

            if (computedY < margin) {
                computedY = triggerPos.y + trigger.height + spacing;
                dir = "bottom";
            }
        }

        computedX = Math.max(margin, Math.min(win.width - menuCard.width - margin, computedX));
        computedY = Math.max(margin, Math.min(win.height - menuCard.height - margin, computedY));

        targetX = computedX;
        targetY = computedY;
        activeDirection = dir;

        birthOriginX = Math.max(0, Math.min(menuCard.width, tCenterX - targetX));
        birthOriginY = Math.max(0, Math.min(menuCard.height, tCenterY - targetY));
    }

    Translate {
        id: triggerTranslate

        x: root.activeDirection === "right" ? root.triggerNudge : (root.activeDirection === "left" ? -root.triggerNudge : 0)
        y: root.activeDirection === "bottom" ? root.triggerNudge : (root.activeDirection === "top" ? -root.triggerNudge : 0)
    }

    onTriggerChanged: {
        if (trigger)
            trigger.transform = [triggerTranslate];
    }

    Connections {
        target: root.trigger
        function onClicked() {
            root.toggle();
        }
    }

    Item {
        id: overlayArea

        parent: root.Window.window ? root.Window.window.contentItem : root
        anchors.fill: parent
        visible: root.isOpen || root.p > 0.005
        z: 99999

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onPressed: root.close()
        }

        Item {
            id: menuCard

            width: 360
            height: Math.min(480, contentLayout.implicitHeight + 16)
            clip: false

            x: root.targetX + root.travelX
            y: root.effectiveTargetY + root.travelY

            onHeightChanged: {
                if (root.isOpen)
                    root.updateCoordinates();
            }

            transform: [
                Scale {
                    origin.x: root.birthOriginX
                    origin.y: root.birthOriginY
                    xScale: 0.93 + 0.07 * root.surfaceProgress
                    yScale: 0.93 + 0.07 * root.surfaceProgress
                }
            ]

            opacity: Math.max(0.0, Math.min(1.0, root.p * 4.0))

            MouseArea {
                anchors.fill: parent
                onPressed: mouse => mouse.accepted = true
            }

            CielSquircle {
                id: cardBg

                anchors.fill: parent
                color: Theme.background
                borderWidth: 1
                borderColor: Theme.border
                radius: 12
                visible: false
            }

            MultiEffect {
                anchors.fill: cardBg
                source: cardBg
                shadowEnabled: true
                shadowColor: Qt.rgba(0, 0, 0, 0.05)
                shadowBlur: 0.95
                shadowVerticalOffset: 16
                shadowHorizontalOffset: 0
            }

            MultiEffect {
                anchors.fill: cardBg
                source: cardBg
                shadowEnabled: true
                shadowColor: Qt.rgba(0, 0, 0, 0.08)
                shadowBlur: 0.55
                shadowVerticalOffset: 6
                shadowHorizontalOffset: 0
            }

            MultiEffect {
                anchors.fill: cardBg
                source: cardBg
                shadowEnabled: true
                shadowColor: Qt.rgba(0, 0, 0, 0.09)
                shadowBlur: 0.36
                shadowVerticalOffset: 2.0
                shadowHorizontalOffset: 0
            }

            ColumnLayout {
                id: contentLayout

                anchors.fill: parent
                anchors.margins: 8
                spacing: 6
                opacity: root.contentOpacity

                transform: [
                    Translate {
                        y: (1.0 - root.contentProgress) * 4.0
                    }
                ]

                RowLayout {
                    Layout.fillWidth: true
                    Layout.margins: 4

                    Text {
                        text: "Downloads"
                        color: Theme.textPrimary
                        font.pixelSize: 14
                        font.weight: Font.DemiBold
                        renderType: Text.NativeRendering
                        Layout.fillWidth: true
                    }

                    CielIconButton {
                        icon: "folder"
                        size: Theme.SMALL
                        onClicked: root.openFolderRequested("")
                    }

                    CielIconButton {
                        icon: "trash"
                        size: Theme.SMALL
                        visible: listView.count > 0
                        onClicked: root.clearFinishedRequested()
                    }
                }

                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 180
                    visible: listView.count === 0

                    ColumnLayout {
                        anchors.centerIn: parent
                        spacing: 8

                        CielIcon {
                            icon: "download-simple"
                            size: 32
                            color: Theme.textSecondary
                            opacity: 0.35
                            Layout.alignment: Qt.AlignHCenter
                        }

                        Text {
                            text: "No downloads yet"
                            color: Theme.textSecondary
                            font.pixelSize: 13
                            renderType: Text.NativeRendering
                            Layout.alignment: Qt.AlignHCenter
                        }
                    }
                }

                ListView {
                    id: listView

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredHeight: Math.min(380, count * 68)
                    visible: count > 0
                    clip: true
                    spacing: 4
                    model: root.downloadsModel

                    delegate: BrowserDownloadItem {
                        width: listView.width
                        downloadId: model.id
                        filename: model.filename
                        destination: model.destination
                        category: model.category
                        state: model.state
                        bytesReceived: model.bytesReceived
                        totalBytes: model.totalBytes
                        speed: model.speed
                        segments: model.segments
                        errorMessage: model.errorMessage

                        onPauseRequested: id => root.pauseRequested(id)
                        onResumeRequested: id => root.resumeRequested(id)
                        onCancelRequested: id => root.cancelRequested(id)
                        onOpenFolderRequested: path => root.openFolderRequested(path)
                        onRetryRequested: id => root.retryRequested(id)
                    }
                }
            }
        }
    }
}
