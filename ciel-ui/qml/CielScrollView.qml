import QtQuick
import QtQuick.Controls as T
import Ciel.Ui

Flickable {
    id: flick

    property var orientation: undefined
    property bool showScrollBar: true

    clip: true
    boundsBehavior: Flickable.StopAtBounds
    flickableDirection: {
        if (flick.orientation === Qt.Horizontal)
            return Flickable.HorizontalFlick;
        if (flick.orientation === Qt.Vertical)
            return Flickable.VerticalFlick;
        return (flick.contentWidth > flick.width) ? Flickable.AutoFlickDirection : Flickable.VerticalFlick;
    }
    flickDeceleration: 580
    maximumFlickVelocity: 3600

    property real scrollBarGutter: 16
    readonly property real availableWidth: (!flick.showScrollBar || flick.orientation === Qt.Horizontal) ? width : Math.max(0, width - scrollBarGutter)
    readonly property real availableHeight: (!flick.showScrollBar || flick.orientation !== Qt.Horizontal) ? height : Math.max(0, height - scrollBarGutter)

    property color thumbColor: Theme.border

    property real wheelStepSize: 80.0
    property real targetContentY: contentY
    property real targetContentX: contentX

    property real overscrollY: 0.0
    property real overscrollX: 0.0
    property bool isTrackingOverscroll: false

    property var touchSamples: []
    property real lastSampleTime: 0
    property real lastFlickVelocityY: 0
    property real lastFlickVelocityX: 0

    contentItem.transform: Translate {
        x: -flick.overscrollX
        y: -flick.overscrollY
    }

    Behavior on overscrollY {
        enabled: !flick.isTrackingOverscroll
        CielSpring {
            damping: 0.58
            spring: 5.0
            mass: 1.0
            epsilon: 0.05
        }
    }

    Behavior on overscrollX {
        enabled: !flick.isTrackingOverscroll
        CielSpring {
            damping: 0.58
            spring: 5.0
            mass: 1.0
            epsilon: 0.05
        }
    }

    onDragStarted: {
        smoothY.stop();
        smoothX.stop();
        targetContentY = contentY;
        targetContentX = contentX;
    }

    onFlickStarted: {
        smoothY.stop();
        smoothX.stop();
        lastFlickVelocityY = verticalVelocity;
        lastFlickVelocityX = horizontalVelocity;
    }

    onContentYChanged: {
        if (!flick.flicking || flick.orientation === Qt.Horizontal)
            return;
        const maxScrollY = Math.max(0, flick.contentHeight - flick.height);

        if (contentY <= 0 && lastFlickVelocityY > 180) {
            const impact = Math.min(90.0, (lastFlickVelocityY / 3600.0) * 90.0);
            flick.isTrackingOverscroll = false;
            flick.overscrollY = -impact;
            lastFlickVelocityY = 0;
            Qt.callLater(() => {
                flick.overscrollY = 0.0;
            });
        } else if (contentY >= maxScrollY && lastFlickVelocityY < -180) {
            const impact = Math.min(90.0, (Math.abs(lastFlickVelocityY) / 3600.0) * 90.0);
            flick.isTrackingOverscroll = false;
            flick.overscrollY = impact;
            lastFlickVelocityY = 0;
            Qt.callLater(() => {
                flick.overscrollY = 0.0;
            });
        }
    }

    onContentXChanged: {
        if (!flick.flicking || flick.orientation === Qt.Vertical)
            return;
        const maxScrollX = Math.max(0, flick.contentWidth - flick.width);
        if (maxScrollX <= 0)
            return;
        if (contentX <= 0 && lastFlickVelocityX > 180) {
            const impact = Math.min(90.0, (lastFlickVelocityX / 3600.0) * 90.0);
            flick.isTrackingOverscroll = false;
            flick.overscrollX = -impact;
            lastFlickVelocityX = 0;
            Qt.callLater(() => {
                flick.overscrollX = 0.0;
            });
        } else if (contentX >= maxScrollX && lastFlickVelocityX < -180) {
            const impact = Math.min(90.0, (Math.abs(lastFlickVelocityX) / 3600.0) * 90.0);
            flick.isTrackingOverscroll = false;
            flick.overscrollX = impact;
            lastFlickVelocityX = 0;
            Qt.callLater(() => {
                flick.overscrollX = 0.0;
            });
        }
    }

    SmoothedAnimation {
        id: smoothY
        target: flick
        property: "contentY"
        velocity: 1100
        maximumEasingTime: 140
        reversingMode: SmoothedAnimation.Eased
    }

    SmoothedAnimation {
        id: smoothX
        target: flick
        property: "contentX"
        velocity: 1100
        maximumEasingTime: 140
        reversingMode: SmoothedAnimation.Eased
    }

    WheelHandler {
        id: wheelHandler
        target: null
        acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad
        blocking: false

        onWheel: event => {
            const devType = event.device ? event.device.type : "unknown";
            const px = event.pixelDelta;
            const ang = event.angleDelta;
            const maxScrollY = Math.max(0, flick.contentHeight - flick.height);
            const maxScrollX = Math.max(0, flick.contentWidth - flick.width);

            const hasPixelDelta = (px.x !== 0 || px.y !== 0);
            const isTouchpad = (devType === PointerDevice.TouchPad) || hasPixelDelta || (ang.x === 0 && ang.y === 0);

            if (isTouchpad) {
                const now = Date.now();

                if (px.x !== 0 || px.y !== 0) {
                    if (flick.flicking) {
                        flick.cancelFlick();
                        flick.touchSamples = [];
                    } else if (now - flick.lastSampleTime > 90) {
                        flick.touchSamples = [];
                    }
                }
                flick.lastSampleTime = now;

                if (px.x === 0 && px.y === 0) {
                    const cutoff = now - 75;
                    const recent = flick.touchSamples.filter(s => s.t >= cutoff);
                    flick.touchSamples = [];

                    flick.isTrackingOverscroll = false;
                    flick.overscrollY = 0.0;
                    flick.overscrollX = 0.0;

                    if (recent.length >= 1) {
                        const lastSample = recent[recent.length - 1];
                        const targetSignY = Math.sign(lastSample.dy);
                        const targetSignX = Math.sign(lastSample.dx);

                        let totalDx = 0;
                        let totalDy = 0;
                        let earliestTime = lastSample.t;

                        for (let i = recent.length - 1; i >= 0; --i) {
                            const s = recent[i];
                            if (targetSignY !== 0 && s.dy !== 0 && Math.sign(s.dy) !== targetSignY)
                                break;
                            totalDy += s.dy;
                            totalDx += s.dx;
                            earliestTime = s.t;
                        }

                        const dt = Math.max(0.016, (now - earliestTime) / 1000.0);
                        const vx = (flick.orientation !== Qt.Vertical && maxScrollX > 0) ? (totalDx / dt) * 0.72 : 0;
                        const vy = (flick.orientation !== Qt.Horizontal) ? (totalDy / dt) * 0.72 : 0;

                        if (Math.abs(vy) > 80 || Math.abs(vx) > 80) {
                            flick.cancelFlick();
                            flick.flick(vx, vy);
                        }
                    }

                    event.accepted = false;
                    return;
                }

                flick.touchSamples.push({
                    dx: px.x,
                    dy: px.y,
                    t: now
                });
                if (flick.touchSamples.length > 14) {
                    flick.touchSamples.shift();
                }

                if (flick.orientation !== Qt.Horizontal && px.y !== 0) {
                    if (flick.contentY <= 0) {
                        if (px.y > 0 || flick.isTrackingOverscroll) {
                            flick.isTrackingOverscroll = true;
                            const resistance = Math.max(0.12, 1.0 - (Math.abs(flick.overscrollY) / 160.0));
                            const nextOverscrollY = flick.overscrollY - px.y * resistance * 0.7;
                            if (nextOverscrollY > 0) {
                                flick.overscrollY = 0.0;
                                flick.isTrackingOverscroll = false;
                            } else {
                                flick.overscrollY = Math.max(-120.0, nextOverscrollY);
                            }
                        }
                    } else if (flick.contentY >= maxScrollY) {
                        if (px.y < 0 || flick.isTrackingOverscroll) {
                            flick.isTrackingOverscroll = true;
                            const resistance = Math.max(0.12, 1.0 - (Math.abs(flick.overscrollY) / 160.0));
                            const nextOverscrollY = flick.overscrollY - px.y * resistance * 0.7;
                            if (nextOverscrollY < 0) {
                                flick.overscrollY = 0.0;
                                flick.isTrackingOverscroll = false;
                            } else {
                                flick.overscrollY = Math.min(120.0, nextOverscrollY);
                            }
                        }
                    } else if (flick.isTrackingOverscroll) {
                        flick.isTrackingOverscroll = false;
                        flick.overscrollY = 0.0;
                    }
                }

                if (flick.orientation !== Qt.Vertical && maxScrollX > 0 && px.x !== 0) {
                    if (flick.contentX <= 0) {
                        if (px.x > 0 || flick.isTrackingOverscroll) {
                            flick.isTrackingOverscroll = true;
                            const resistance = Math.max(0.12, 1.0 - (Math.abs(flick.overscrollX) / 160.0));
                            const nextOverscrollX = flick.overscrollX - px.x * resistance * 0.7;
                            if (nextOverscrollX > 0) {
                                flick.overscrollX = 0.0;
                                flick.isTrackingOverscroll = false;
                            } else {
                                flick.overscrollX = Math.max(-120.0, nextOverscrollX);
                            }
                        }
                    } else if (flick.contentX >= maxScrollX) {
                        if (px.x < 0 || flick.isTrackingOverscroll) {
                            flick.isTrackingOverscroll = true;
                            const resistance = Math.max(0.12, 1.0 - (Math.abs(flick.overscrollX) / 160.0));
                            const nextOverscrollX = flick.overscrollX - px.x * resistance * 0.7;
                            if (nextOverscrollX < 0) {
                                flick.overscrollX = 0.0;
                                flick.isTrackingOverscroll = false;
                            } else {
                                flick.overscrollX = Math.min(120.0, nextOverscrollX);
                            }
                        }
                    } else if (flick.isTrackingOverscroll && (flick.orientation === Qt.Horizontal || maxScrollY <= 0)) {
                        flick.isTrackingOverscroll = false;
                        flick.overscrollX = 0.0;
                    }
                } else if (flick.orientation === Qt.Vertical) {
                    flick.overscrollX = 0.0;
                }

                event.accepted = false;
                return;
            }

            event.accepted = true;

            const isHorizontalOnly = (flick.contentWidth > flick.width) && (flick.contentHeight <= flick.height);
            const isHorizontal = flick.orientation === Qt.Horizontal || (flick.orientation !== Qt.Vertical && isHorizontalOnly);

            if (isHorizontal && (maxScrollX > 0 || flick.orientation === Qt.Horizontal)) {
                const rawDelta = ang.y !== 0 ? ang.y : ang.x;
                const step = (rawDelta / 120.0) * flick.wheelStepSize;
                const nextX = flick.targetContentX - step;

                if (nextX < 0) {
                    flick.targetContentX = 0;
                    smoothX.to = 0;
                    smoothX.start();
                    flick.isTrackingOverscroll = false;
                    flick.overscrollX = -30.0;
                    Qt.callLater(() => {
                        flick.overscrollX = 0.0;
                    });
                } else if (nextX > maxScrollX) {
                    flick.targetContentX = maxScrollX;
                    smoothX.to = maxScrollX;
                    smoothX.start();
                    flick.isTrackingOverscroll = false;
                    flick.overscrollX = 30.0;
                    Qt.callLater(() => {
                        flick.overscrollX = 0.0;
                    });
                } else {
                    flick.isTrackingOverscroll = false;
                    flick.overscrollX = 0.0;
                    flick.targetContentX = nextX;
                    smoothX.to = nextX;
                    smoothX.start();
                }
            } else if (flick.orientation !== Qt.Horizontal) {
                const rawDelta = ang.y !== 0 ? ang.y : ang.x;
                const step = (rawDelta / 120.0) * flick.wheelStepSize;
                const nextY = flick.targetContentY - step;

                if (nextY < 0) {
                    flick.targetContentY = 0;
                    smoothY.to = 0;
                    smoothY.start();
                    flick.isTrackingOverscroll = false;
                    flick.overscrollY = -30.0;
                    Qt.callLater(() => {
                        flick.overscrollY = 0.0;
                    });
                } else if (nextY > maxScrollY) {
                    flick.targetContentY = maxScrollY;
                    smoothY.to = maxScrollY;
                    smoothY.start();
                    flick.isTrackingOverscroll = false;
                    flick.overscrollY = 30.0;
                    Qt.callLater(() => {
                        flick.overscrollY = 0.0;
                    });
                } else {
                    flick.isTrackingOverscroll = false;
                    flick.overscrollY = 0.0;
                    flick.targetContentY = nextY;
                    smoothY.to = nextY;
                    smoothY.start();
                }
            }
        }
    }

    T.ScrollBar.vertical: T.ScrollBar {
        id: vBar
        parent: flick
        z: 100
        anchors.top: flick.top
        anchors.bottom: flick.bottom
        anchors.right: flick.right
        anchors.rightMargin: 1
        policy: (!flick.showScrollBar || flick.orientation === Qt.Horizontal) ? T.ScrollBar.AlwaysOff : (flick.contentHeight > flick.height ? T.ScrollBar.AsNeeded : T.ScrollBar.AlwaysOff)
        interactive: true
        hoverEnabled: true
        width: 14

        contentItem: Item {
            id: vThumbContainer
            implicitWidth: 8

            HoverHandler {
                id: vThumbHover
            }

            Rectangle {
                id: vThumbVisual
                anchors.centerIn: parent
                width: 4
                height: parent.height
                radius: 2
                color: flick.thumbColor

                opacity: {
                    if (vBar.pressed)
                        return 0.95;
                    if (vThumbHover.hovered)
                        return 0.85;
                    if (vBar.hovered)
                        return 0.55;
                    if (flick.moving || flick.flicking || smoothY.running || Math.abs(flick.overscrollY) > 1.0)
                        return 0.40;
                    return 0.0;
                }

                Behavior on opacity {
                    NumberAnimation {
                        duration: 180
                        easing.type: Easing.OutQuad
                    }
                }

                transform: Scale {
                    id: vThumbScale
                    origin.x: vThumbVisual.width / 2
                    origin.y: vThumbVisual.height / 2

                    xScale: {
                        if (vBar.pressed)
                            return 1.75;
                        if (vThumbHover.hovered)
                            return 1.30;
                        return 1.0;
                    }

                    yScale: {
                        if (vBar.pressed)
                            return 0.84;
                        if (vThumbHover.hovered)
                            return 0.94;
                        return 1.0;
                    }

                    Behavior on xScale {
                        CielSpring {
                            damping: 0.22
                            spring: 3.6
                            mass: 1.0
                            epsilon: 0.001
                        }
                    }

                    Behavior on yScale {
                        CielSpring {
                            damping: 0.22
                            spring: 3.6
                            mass: 1.0
                            epsilon: 0.001
                        }
                    }
                }
            }
        }
    }

    T.ScrollBar.horizontal: T.ScrollBar {
        id: hBar
        parent: flick
        z: 100
        anchors.left: flick.left
        anchors.right: flick.right
        anchors.bottom: flick.bottom
        anchors.bottomMargin: 1
        policy: {
            if (!flick.showScrollBar || flick.orientation === Qt.Vertical)
                return T.ScrollBar.AlwaysOff;
            if (flick.orientation === Qt.Horizontal)
                return flick.contentWidth > flick.width ? T.ScrollBar.AsNeeded : T.ScrollBar.AlwaysOff;
            return (flick.contentWidth > flick.width && flick.contentHeight <= flick.height) ? T.ScrollBar.AsNeeded : T.ScrollBar.AlwaysOff;
        }
        interactive: true
        hoverEnabled: true
        height: 14

        contentItem: Item {
            id: hThumbContainer
            implicitHeight: 8

            HoverHandler {
                id: hThumbHover
            }

            Rectangle {
                id: hThumbVisual
                anchors.centerIn: parent
                height: 4
                width: parent.width
                radius: 2
                color: flick.thumbColor

                opacity: {
                    if (hBar.pressed)
                        return 0.95;
                    if (hThumbHover.hovered)
                        return 0.85;
                    if (hBar.hovered)
                        return 0.55;
                    if (flick.moving || flick.flicking || smoothX.running || Math.abs(flick.overscrollX) > 1.0)
                        return 0.40;
                    return 0.0;
                }

                Behavior on opacity {
                    NumberAnimation {
                        duration: 180
                    }
                }

                transform: Scale {
                    id: hThumbScale
                    origin.x: hThumbVisual.width / 2
                    origin.y: hThumbVisual.height / 2

                    xScale: {
                        if (hBar.pressed)
                            return 0.84;
                        if (hThumbHover.hovered)
                            return 0.94;
                        return 1.0;
                    }

                    yScale: {
                        if (hBar.pressed)
                            return 1.75;
                        if (hThumbHover.hovered)
                            return 1.30;
                        return 1.0;
                    }

                    Behavior on xScale {
                        CielSpring {
                            damping: 0.22
                            spring: 3.6
                            mass: 1.0
                            epsilon: 0.001
                        }
                    }

                    Behavior on yScale {
                        CielSpring {
                            damping: 0.22
                            spring: 3.6
                            mass: 1.0
                            epsilon: 0.001
                        }
                    }
                }
            }
        }
    }
}
