import QtQuick
import QtQuick.Layouts
import Ciel.Ui
import Ciel.Browser 1.0

Item {
    id: tabDelegateH

    required property int index
    required property string title
    required property string url
    required property bool loading
    required property TabModel tabModel

    property var views: null
    property var collisionHub: null
    property real containerWidth: 0
    property real tabHeight: 36
    property real workspaceDiff: 0.0
    property bool isTargetWorkspace: false

    readonly property int totalTabs: tabModel ? tabModel.count : 1
    readonly property real staggerOffset: isTargetWorkspace ? ((totalTabs - 1 - index) * 0.04) : (index * 0.04)
    readonly property real absWsDiff: Math.abs(workspaceDiff)
    readonly property real wsP: Math.max(0.0, Math.min(1.0, (absWsDiff - staggerOffset) / 0.55))
    readonly property real wsDir: workspaceDiff >= 0 ? 1 : -1

    readonly property bool isCurrent: tabModel.currentIndex === tabDelegateH.index
    readonly property bool isHovered: tabHoverH.hovered
    readonly property var currentTabView: (views && views.itemAt) ? views.itemAt(tabDelegateH.index) : null
    readonly property url siteFavicon: {
        if (currentTabView && currentTabView.engine && currentTabView.engine.icon) {
            return currentTabView.engine.icon;
        }
        return "";
    }

    readonly property string displayTitle: {
        if (url.toString() === "ciel://history" || url.toString() === "about:history") {
            return "History";
        }
        if (title && title.length > 0 && title !== "about:blank" && !url.toString().startsWith("about:blank")) {
            return title;
        }
        return "New Tab";
    }

    property real targetWidth: Math.min(180, Math.max(80, (containerWidth - 50) / Math.max(1, tabModel.count)))
    readonly property real slotSpan: targetWidth + 4

    property real spawnProgress: 0.0
    property real closeY: 0.0
    property real closeYScale: 1.0
    property real closeXScale: 1.0
    property real closeOpacity: 1.0
    property real closeDimension: 1.0
    property bool isClosing: false

    property real shockwaveOffset: 0.0

    property bool isDragging: false
    property real dragX: 0.0
    property real landX: 0.0
    property bool isLanding: false

    readonly property bool isDragSource: collisionHub && collisionHub.dragSourceIndex === tabDelegateH.index

    readonly property real targetShift: {
        if (!collisionHub || collisionHub.dragSourceIndex === -1 || isDragSource)
            return 0.0;

        var src = collisionHub.dragSourceIndex;
        var dst = collisionHub.dragTargetIndex;

        if (src < dst) {
            if (tabDelegateH.index > src && tabDelegateH.index <= dst)
                return -slotSpan;
        } else if (src > dst) {
            if (tabDelegateH.index >= dst && tabDelegateH.index < src)
                return slotSpan;
        }
        return 0.0;
    }

    property real animatedShift: 0.0

    onIndexChanged: {
        shiftSpringH.stop();
        animatedShift = 0.0;
        if (isLanding) {
            isLanding = false;
            landX = 0.0;
            dragX = 0.0;
        }
    }

    onTargetShiftChanged: {
        if (!isDragSource && !isClosing) {
            if (collisionHub && collisionHub.dragSourceIndex !== -1) {
                shiftSpringH.stop();
                shiftSpringH.to = targetShift;
                shiftSpringH.restart();
            } else {
                shiftSpringH.stop();
                animatedShift = 0.0;
            }
        }
    }

    CielSpring {
        id: shiftSpringH
        target: tabDelegateH
        property: "animatedShift"
        damping: 0.32
        spring: 5.2
        mass: 1.0
        epsilon: 0.001
    }

    CielSpring {
        id: landSpringH
        target: tabDelegateH
        property: "landX"
        damping: 0.34
        spring: 5.4
        mass: 1.0
        epsilon: 0.001

        property int commitSrc: -1
        property int commitDst: -1

        onFinished: {
            var s = commitSrc;
            var d = commitDst;
            commitSrc = -1;
            commitDst = -1;

            if (s !== -1 && d !== -1 && s !== d) {
                tabModel.moveTab(s, d);
            }

            if (collisionHub) {
                collisionHub.dragSourceIndex = -1;
                collisionHub.dragTargetIndex = -1;
            }

            tabDelegateH.isLanding = false;
            tabDelegateH.dragX = 0.0;
            tabDelegateH.landX = 0.0;
            tabDelegateH.animatedShift = 0.0;
        }
    }

    z: isDragSource ? 100 : (isLanding ? 90 : 1)

    Behavior on shockwaveOffset {
        CielSpring {
            damping: 0.28
            spring: 5.2
            mass: 1.0
            epsilon: 0.001
        }
    }

    Connections {
        target: collisionHub
        function onTabCollisionImpulse(sourceIndex) {
            if (tabDelegateH.isClosing)
                return;
            var diff = sourceIndex - tabDelegateH.index;
            if (diff >= 1 && diff <= 3) {
                shockwaveTimerH.delayMs = (diff - 1) * 28;
                shockwaveTimerH.impulse = -14.0 * Math.pow(0.62, diff - 1);
                shockwaveTimerH.restart();
            }
        }
    }

    SequentialAnimation {
        id: shockwaveTimerH
        property int delayMs: 0
        property real impulse: 0.0

        PauseAnimation {
            duration: shockwaveTimerH.delayMs
        }
        ScriptAction {
            script: tabDelegateH.shockwaveOffset = shockwaveTimerH.impulse
        }
        PauseAnimation {
            duration: 115
        }
        ScriptAction {
            script: tabDelegateH.shockwaveOffset = 0.0
        }
    }

    function requestClose() {
        if (isClosing)
            return;
        isClosing = true;
        closeAnimationH.restart();
    }

    ParallelAnimation {
        id: closeAnimationH

        ScriptAction {
            script: if (collisionHub)
                collisionHub.tabCollisionImpulse(tabDelegateH.index)
        }

        NumberAnimation {
            target: tabDelegateH
            property: "closeY"
            from: 0.0
            to: -32.0
            duration: 190
            easing.type: Easing.OutCubic
        }

        SequentialAnimation {
            NumberAnimation {
                target: tabDelegateH
                property: "closeYScale"
                from: 1.0
                to: 0.32
                duration: 180
                easing.type: Easing.InQuad
            }
            NumberAnimation {
                target: tabDelegateH
                property: "closeYScale"
                to: 0.0
                duration: 30
            }
        }

        SequentialAnimation {
            NumberAnimation {
                target: tabDelegateH
                property: "closeXScale"
                from: 1.0
                to: 1.10
                duration: 60
                easing.type: Easing.OutQuad
            }
            NumberAnimation {
                target: tabDelegateH
                property: "closeXScale"
                from: 1.10
                to: 0.20
                duration: 150
                easing.type: Easing.InQuad
            }
        }

        SequentialAnimation {
            PauseAnimation {
                duration: 45
            }
            NumberAnimation {
                target: tabDelegateH
                property: "closeDimension"
                from: 1.0
                to: 0.0
                duration: 175
                easing.type: Easing.InOutQuad
            }
        }

        SequentialAnimation {
            PauseAnimation {
                duration: 60
            }
            NumberAnimation {
                target: tabDelegateH
                property: "closeOpacity"
                from: 1.0
                to: 0.0
                duration: 145
                easing.type: Easing.OutQuad
            }
        }

        onFinished: {
            tabModel.closeTab(tabDelegateH.index);
        }
    }

    Component.onCompleted: spawnProgress = 1.0

    width: Math.max(0, targetWidth * spawnProgress * closeDimension)
    height: tabHeight
    anchors.verticalCenter: parent.verticalCenter
    clip: false

    Behavior on spawnProgress {
        CielSpring {
            damping: 0.28
            spring: 4.8
            mass: 1.0
            epsilon: 0.002
        }
    }

    Behavior on targetWidth {
        CielSpring {
            damping: 3.6
            spring: 8.5
            mass: 2.0
            epsilon: 0.01
        }
    }

    HoverHandler {
        id: tabHoverH
    }

    MouseArea {
        id: tabMouseH
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: tabDelegateH.isDragging ? Qt.ClosedHandCursor : Qt.PointingHandCursor
        z: 0

        property real startMouseX: 0.0
        property bool dragThresholdMet: false

        onPressed: mouse => {
            startMouseX = mouse.x;
            dragThresholdMet = false;
            if (!tabDelegateH.isClosing) {
                tabModel.currentIndex = tabDelegateH.index;
            }
        }

        onPositionChanged: mouse => {
            if (!pressed || tabDelegateH.isClosing)
                return;

            var delta = mouse.x - startMouseX;
            if (!dragThresholdMet && Math.abs(delta) > 6) {
                dragThresholdMet = true;
                tabDelegateH.isDragging = true;
                if (collisionHub) {
                    collisionHub.dragSourceIndex = tabDelegateH.index;
                    collisionHub.dragTargetIndex = tabDelegateH.index;
                }
            }

            if (tabDelegateH.isDragging) {
                tabDelegateH.dragX = delta;
                var approxTarget = tabDelegateH.index + Math.round(tabDelegateH.dragX / tabDelegateH.slotSpan);
                var boundedTarget = Math.max(0, Math.min(tabModel.count - 1, approxTarget));
                if (collisionHub && collisionHub.dragTargetIndex !== boundedTarget) {
                    collisionHub.dragTargetIndex = boundedTarget;
                }
            }
        }

        onReleased: {
            if (tabDelegateH.isDragging) {
                var src = collisionHub ? collisionHub.dragSourceIndex : tabDelegateH.index;
                var dst = collisionHub ? collisionHub.dragTargetIndex : tabDelegateH.index;

                if (src !== -1 && dst !== -1 && src !== dst) {
                    var offsetGoal = (dst - src) * tabDelegateH.slotSpan;
                    landSpringH.stop();
                    landSpringH.from = tabDelegateH.dragX;
                    landSpringH.to = offsetGoal;
                    landSpringH.commitSrc = src;
                    landSpringH.commitDst = dst;
                    tabDelegateH.landX = tabDelegateH.dragX;
                    tabDelegateH.isLanding = true;
                    tabDelegateH.isDragging = false;
                    landSpringH.restart();
                } else {
                    landSpringH.stop();
                    landSpringH.from = tabDelegateH.dragX;
                    landSpringH.to = 0.0;
                    landSpringH.commitSrc = -1;
                    landSpringH.commitDst = -1;
                    tabDelegateH.landX = tabDelegateH.dragX;
                    tabDelegateH.isLanding = true;
                    tabDelegateH.isDragging = false;
                    landSpringH.restart();
                }
            }
        }

        onCanceled: {
            if (tabDelegateH.isDragging || tabDelegateH.isLanding) {
                tabDelegateH.isDragging = false;
                tabDelegateH.isLanding = false;
                tabDelegateH.dragX = 0.0;
                tabDelegateH.landX = 0.0;
                if (collisionHub) {
                    collisionHub.dragSourceIndex = -1;
                    collisionHub.dragTargetIndex = -1;
                }
            }
        }
    }

    Item {
        id: visualContentH
        width: tabDelegateH.targetWidth
        height: parent.height
        anchors.centerIn: parent
        z: 1

        opacity: tabDelegateH.isClosing ? tabDelegateH.closeOpacity : (Math.min(1.0, tabDelegateH.spawnProgress * 2.0) * Math.max(0.0, 1.0 - tabDelegateH.wsP * 1.5))

        transform: [
            Translate {
                x: {
                    var baseX = 0.0;
                    if (tabDelegateH.isDragging)
                        baseX = tabDelegateH.dragX;
                    else if (tabDelegateH.isLanding)
                        baseX = tabDelegateH.landX;
                    else
                        baseX = tabDelegateH.animatedShift;
                    return baseX + Math.round(tabDelegateH.wsDir * tabDelegateH.wsP * 48);
                }
                y: (tabDelegateH.isClosing ? tabDelegateH.closeY : tabDelegateH.shockwaveOffset) + Math.round(tabDelegateH.wsP * 12)
            },
            Scale {
                origin.x: visualContentH.width / 2
                origin.y: visualContentH.height / 2
                xScale: tabDelegateH.isClosing ? tabDelegateH.closeXScale : (tabDelegateH.wsP > 0 ? (1.0 - tabDelegateH.wsP * 0.12) : (tabDelegateH.isDragging ? 1.05 : (tabMouseH.pressed ? 1.04 : 1.0)))
                yScale: tabDelegateH.isClosing ? tabDelegateH.closeYScale : (tabDelegateH.wsP > 0 ? (1.0 - tabDelegateH.wsP * 0.12) : (tabDelegateH.isDragging ? 1.05 : (tabMouseH.pressed ? 0.88 : 1.0)))

                Behavior on xScale {
                    enabled: !tabDelegateH.isClosing && tabDelegateH.wsP === 0
                    CielSpring {
                        damping: 0.28
                        spring: 5.4
                        mass: 0.9
                        epsilon: 0.001
                    }
                }

                Behavior on yScale {
                    enabled: !tabDelegateH.isClosing && tabDelegateH.wsP === 0
                    CielSpring {
                        damping: 0.28
                        spring: 5.4
                        mass: 0.9
                        epsilon: 0.001
                    }
                }
            }
        ]

        CielSquircle {
            id: tabPillH
            anchors.fill: parent
            color: Theme.surface
            borderWidth: tabDelegateH.isDragging ? 1 : 0
            borderColor: Theme.border
            opacity: (tabDelegateH.isCurrent || tabDelegateH.isDragging) ? 1.0 : (tabDelegateH.isHovered ? 0.6 : 0.0)

            Behavior on opacity {
                NumberAnimation {
                    duration: 120
                    easing.type: Easing.OutQuad
                }
            }
        }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 6
            spacing: 6

            Item {
                Layout.preferredWidth: 16
                Layout.preferredHeight: 16
                Layout.alignment: Qt.AlignVCenter

                CielLoadingSpinner {
                    anchors.centerIn: parent
                    implicitWidth: 16
                    implicitHeight: 16
                    orbitRadius: 4.5
                    minDotSize: 1.2
                    maxDotSize: 3.2
                    finalSize: 2.8
                    finished: !tabDelegateH.loading
                    color: Theme.accent
                    visible: !finished
                }

                Image {
                    id: faviconH
                    anchors.centerIn: parent
                    width: 16
                    height: 16
                    sourceSize.width: 32
                    sourceSize.height: 32
                    fillMode: Image.PreserveAspectFit
                    source: tabDelegateH.siteFavicon
                    visible: status === Image.Ready && !tabDelegateH.loading
                }

                CielIcon {
                    anchors.centerIn: parent
                    icon: tabDelegateH.url.toString().indexOf("history") !== -1 ? "clock-counter-clockwise" : "globe"
                    size: Theme.XSMALL
                    color: tabDelegateH.isCurrent ? Theme.textPrimary : Theme.textSecondary
                    visible: !faviconH.visible && !tabDelegateH.loading
                }
            }

            Text {
                text: tabDelegateH.displayTitle
                font.pixelSize: 12
                font.weight: tabDelegateH.isCurrent ? Font.Medium : Font.Normal
                color: tabDelegateH.isCurrent ? Theme.textPrimary : Theme.textSecondary
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            CielIconButton {
                z: 10
                icon: "x"
                size: Theme.XSMALL
                visible: tabModel.count > 1 && (tabDelegateH.isHovered || tabDelegateH.isCurrent)
                Layout.alignment: Qt.AlignVCenter
                onClicked: tabDelegateH.requestClose()
            }
        }
    }
}
