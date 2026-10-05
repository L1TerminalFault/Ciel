import QtQuick
import QtQuick.Controls as T
import Ciel.Ui

ListView {
    id: view

    clip: true
    boundsBehavior: Flickable.StopAtBounds
    flickDeceleration: 580
    maximumFlickVelocity: 3600

    property real scrollBarGutter: 16
    property color thumbColor: Theme.border

    property real wheelStepSize: 80.0
    property real targetContentY: contentY
    property real overscrollY: 0.0
    property bool isTrackingOverscroll: false

    property var touchSamples: []
    property real lastSampleTime: 0
    property real lastFlickVelocityY: 0

    contentItem.transform: Translate {
        y: -view.overscrollY
    }

    Behavior on overscrollY {
        enabled: !view.isTrackingOverscroll
        CielSpring {
            damping: 0.58
            spring: 5.0
            mass: 1.0
            epsilon: 0.05
        }
    }

    onDragStarted: {
        smoothY.stop();
        targetContentY = contentY;
    }

    onFlickStarted: {
        smoothY.stop();
        lastFlickVelocityY = verticalVelocity;
    }

    onContentYChanged: {
        if (!view.flicking)
            return;
        const maxScrollY = Math.max(0, view.contentHeight - view.height);

        if (contentY <= 0 && lastFlickVelocityY > 180) {
            const impact = Math.min(90.0, (lastFlickVelocityY / 3600.0) * 90.0);
            view.isTrackingOverscroll = false;
            view.overscrollY = -impact;
            lastFlickVelocityY = 0;
            Qt.callLater(() => {
                view.overscrollY = 0.0;
            });
        } else if (contentY >= maxScrollY && lastFlickVelocityY < -180) {
            const impact = Math.min(90.0, (Math.abs(lastFlickVelocityY) / 3600.0) * 90.0);
            view.isTrackingOverscroll = false;
            view.overscrollY = impact;
            lastFlickVelocityY = 0;
            Qt.callLater(() => {
                view.overscrollY = 0.0;
            });
        }
    }

    SmoothedAnimation {
        id: smoothY
        target: view
        property: "contentY"
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
            const maxScrollY = Math.max(0, view.contentHeight - view.height);

            const hasPixelDelta = (px.x !== 0 || px.y !== 0);
            const isTouchpad = (devType === PointerDevice.TouchPad) || hasPixelDelta || (ang.x === 0 && ang.y === 0);

            if (isTouchpad) {
                const now = Date.now();

                if (px.y !== 0) {
                    if (view.flicking) {
                        view.cancelFlick();
                        view.touchSamples = [];
                    } else if (now - view.lastSampleTime > 90) {
                        view.touchSamples = [];
                    }
                }
                view.lastSampleTime = now;

                if (px.x === 0 && px.y === 0) {
                    const cutoff = now - 75;
                    const recent = view.touchSamples.filter(s => s.t >= cutoff);
                    view.touchSamples = [];

                    view.isTrackingOverscroll = false;
                    view.overscrollY = 0.0;

                    if (recent.length >= 1) {
                        const lastSample = recent[recent.length - 1];
                        const targetSignY = Math.sign(lastSample.dy);

                        let totalDy = 0;
                        let earliestTime = lastSample.t;

                        for (let i = recent.length - 1; i >= 0; --i) {
                            const s = recent[i];
                            if (targetSignY !== 0 && s.dy !== 0 && Math.sign(s.dy) !== targetSignY)
                                break;
                            totalDy += s.dy;
                            earliestTime = s.t;
                        }

                        const dt = Math.max(0.016, (now - earliestTime) / 1000.0);
                        const vy = (totalDy / dt) * 0.72;

                        if (Math.abs(vy) > 80) {
                            view.cancelFlick();
                            view.flick(0, vy);
                        }
                    }

                    event.accepted = false;
                    return;
                }

                view.touchSamples.push({
                    dx: px.x,
                    dy: px.y,
                    t: now
                });
                if (view.touchSamples.length > 14)
                    view.touchSamples.shift();

                if (px.y !== 0) {
                    if (view.contentY <= 0) {
                        if (px.y > 0 || view.isTrackingOverscroll) {
                            view.isTrackingOverscroll = true;
                            const resistance = Math.max(0.12, 1.0 - (Math.abs(view.overscrollY) / 160.0));
                            const nextOverscrollY = view.overscrollY - px.y * resistance * 0.7;
                            if (nextOverscrollY > 0) {
                                view.overscrollY = 0.0;
                                view.isTrackingOverscroll = false;
                            } else {
                                view.overscrollY = Math.max(-120.0, nextOverscrollY);
                            }
                        }
                    } else if (view.contentY >= maxScrollY) {
                        if (px.y < 0 || view.isTrackingOverscroll) {
                            view.isTrackingOverscroll = true;
                            const resistance = Math.max(0.12, 1.0 - (Math.abs(view.overscrollY) / 160.0));
                            const nextOverscrollY = view.overscrollY - px.y * resistance * 0.7;
                            if (nextOverscrollY < 0) {
                                view.overscrollY = 0.0;
                                view.isTrackingOverscroll = false;
                            } else {
                                view.overscrollY = Math.min(120.0, nextOverscrollY);
                            }
                        }
                    } else if (view.isTrackingOverscroll) {
                        view.isTrackingOverscroll = false;
                        view.overscrollY = 0.0;
                    }
                }

                event.accepted = false;
                return;
            }

            event.accepted = true;

            const rawDelta = ang.y !== 0 ? ang.y : ang.x;
            const step = (rawDelta / 120.0) * view.wheelStepSize;
            const nextY = view.targetContentY - step;

            if (nextY < 0) {
                view.targetContentY = 0;
                smoothY.to = 0;
                smoothY.start();
                view.isTrackingOverscroll = false;
                view.overscrollY = -30.0;
                Qt.callLater(() => {
                    view.overscrollY = 0.0;
                });
            } else if (nextY > maxScrollY) {
                view.targetContentY = maxScrollY;
                smoothY.to = maxScrollY;
                smoothY.start();
                view.isTrackingOverscroll = false;
                view.overscrollY = 30.0;
                Qt.callLater(() => {
                    view.overscrollY = 0.0;
                });
            } else {
                view.isTrackingOverscroll = false;
                view.overscrollY = 0.0;
                view.targetContentY = nextY;
                smoothY.to = nextY;
                smoothY.start();
            }
        }
    }

    T.ScrollBar.vertical: T.ScrollBar {
        id: vBar
        parent: view
        z: 100
        anchors.top: view.top
        anchors.bottom: view.bottom
        anchors.right: view.right
        anchors.rightMargin: 1
        policy: view.contentHeight > view.height ? T.ScrollBar.AsNeeded : T.ScrollBar.AlwaysOff
        interactive: true
        hoverEnabled: true
        width: 14

        contentItem: Item {
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
                color: view.thumbColor
                opacity: {
                    if (vBar.pressed)
                        return 0.95;
                    if (vThumbHover.hovered)
                        return 0.85;
                    if (vBar.hovered)
                        return 0.55;
                    if (view.moving || view.flicking || smoothY.running || Math.abs(view.overscrollY) > 1.0)
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
                    origin.x: vThumbVisual.width / 2
                    origin.y: vThumbVisual.height / 2
                    xScale: vBar.pressed ? 1.75 : (vThumbHover.hovered ? 1.30 : 1.0)
                    yScale: vBar.pressed ? 0.84 : (vThumbHover.hovered ? 0.94 : 1.0)
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
