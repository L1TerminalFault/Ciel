import QtQuick
import Ciel.Ui

Item {
    id: controller

    readonly property Flickable target: parent

    anchors.fill: parent

    property var orientation: undefined
    property real wheelStepSize: 90.0

    property real targetContentY: target ? target.contentY : 0.0
    property real targetContentX: target ? target.contentX : 0.0

    property real overscrollY: 0.0
    property real overscrollX: 0.0
    property bool isTrackingOverscroll: false

    property var touchSamples: []
    property real lastSampleTime: 0

    property real lastFlickVelocityY: 0
    property real lastFlickVelocityX: 0

    property real pendingWheelY: target ? target.contentY : 0.0
    property real pendingWheelX: target ? target.contentX : 0.0
    property real lastStepSignY: 0
    property real lastStepSignX: 0

    Translate {
        id: overscrollTranslate
        x: -controller.overscrollX
        y: -controller.overscrollY
    }

    Component.onCompleted: {
        if (target) {
            target.boundsBehavior = Flickable.StopAtBounds;
            target.flickDeceleration = 1400;
            target.maximumFlickVelocity = 6500;

            if (target.contentItem)
                target.contentItem.transform = [overscrollTranslate];
        }

        controller.pendingWheelY = target ? target.contentY : 0.0;
        controller.pendingWheelX = target ? target.contentX : 0.0;
    }

    Behavior on overscrollY {
        enabled: !controller.isTrackingOverscroll
        CielSpring {
            damping: 0.58
            spring: 5.0
            mass: 1.0
            epsilon: 0.05
        }
    }

    Behavior on overscrollX {
        enabled: !controller.isTrackingOverscroll
        CielSpring {
            damping: 0.58
            spring: 5.0
            mass: 1.0
            epsilon: 0.05
        }
    }

    Connections {
        target: controller.target

        function onDragStarted() {
            smoothY.stop();
            smoothX.stop();
            touchMomentumY.stop();
            touchMomentumX.stop();

            controller.pendingWheelY = controller.target.contentY;
            controller.pendingWheelX = controller.target.contentX;

            controller.targetContentY = controller.target.contentY;
            controller.targetContentX = controller.target.contentX;
        }

        function onFlickStarted() {
            smoothY.stop();
            smoothX.stop();
            touchMomentumY.stop();
            touchMomentumX.stop();

            controller.pendingWheelY = controller.target.contentY;
            controller.pendingWheelX = controller.target.contentX;

            controller.lastFlickVelocityY = controller.target.verticalVelocity;
            controller.lastFlickVelocityX = controller.target.horizontalVelocity;
        }

        function onContentYChanged() {
            if (!controller.target.flicking || controller.orientation === Qt.Horizontal)
                return;
            const maxScrollY = Math.max(0, controller.target.contentHeight - controller.target.height);

            if (controller.target.contentY <= 0 && controller.lastFlickVelocityY > 220) {
                const impact = Math.min(110.0, (controller.lastFlickVelocityY / 6500.0) * 110.0);

                controller.isTrackingOverscroll = false;
                controller.overscrollY = -impact;
                controller.lastFlickVelocityY = 0;

                Qt.callLater(() => controller.overscrollY = 0.0);
            } else if (controller.target.contentY >= maxScrollY && controller.lastFlickVelocityY < -220) {
                const impact = Math.min(110.0, (Math.abs(controller.lastFlickVelocityY) / 6500.0) * 110.0);

                controller.isTrackingOverscroll = false;
                controller.overscrollY = impact;
                controller.lastFlickVelocityY = 0;

                Qt.callLater(() => controller.overscrollY = 0.0);
            }
        }

        function onContentXChanged() {
            if (!controller.target.flicking || controller.orientation === Qt.Vertical)
                return;
            const maxScrollX = Math.max(0, controller.target.contentWidth - controller.target.width);

            if (maxScrollX <= 0)
                return;
            if (controller.target.contentX <= 0 && controller.lastFlickVelocityX > 220) {
                const impact = Math.min(110.0, (controller.lastFlickVelocityX / 6500.0) * 110.0);

                controller.isTrackingOverscroll = false;
                controller.overscrollX = -impact;
                controller.lastFlickVelocityX = 0;

                Qt.callLater(() => controller.overscrollX = 0.0);
            } else if (controller.target.contentX >= maxScrollX && controller.lastFlickVelocityX < -220) {
                const impact = Math.min(110.0, (Math.abs(controller.lastFlickVelocityX) / 6500.0) * 110.0);

                controller.isTrackingOverscroll = false;
                controller.overscrollX = impact;
                controller.lastFlickVelocityX = 0;

                Qt.callLater(() => controller.overscrollX = 0.0);
            }
        }

        function onFlickEnded() {
        }
    }

    SmoothedAnimation {
        id: smoothY
        target: controller.target
        property: "contentY"
        velocity: 2200
        maximumEasingTime: 160
        reversingMode: SmoothedAnimation.Eased
    }

    SmoothedAnimation {
        id: smoothX
        target: controller.target
        property: "contentX"
        velocity: 2200
        maximumEasingTime: 160
        reversingMode: SmoothedAnimation.Eased
    }

    function applyWheelY(step) {
        const maxScrollY = Math.max(0, controller.target.contentHeight - controller.target.height);
        const currentY = controller.target.contentY;
        const running = smoothY.running;
        const stepSign = Math.sign(step);
        const reversing = running && controller.lastStepSignY !== 0 && stepSign !== controller.lastStepSignY;
        controller.lastStepSignY = stepSign;

        if (!running)
            controller.pendingWheelY = currentY;

        const base = controller.pendingWheelY;
        const targetY = Math.max(0, Math.min(maxScrollY, base - step));

        if (reversing) {
            smoothY.stop();
            controller.pendingWheelY = currentY;
        }

        const finalY = reversing ? Math.max(0, Math.min(maxScrollY, currentY - step)) : targetY;
        controller.pendingWheelY = finalY;
        controller.isTrackingOverscroll = false;
        controller.overscrollY = 0.0;
        smoothY.to = finalY;
        smoothY.velocity = Math.max(400, Math.abs(finalY - currentY) / 0.18);

        if (!smoothY.running)
            smoothY.start();
    }

    function applyWheelX(step) {
        const maxScrollX = Math.max(0, controller.target.contentWidth - controller.target.width);
        if (maxScrollX <= 0 && controller.orientation !== Qt.Horizontal)
            return;

        const currentX = controller.target.contentX;
        const running = smoothX.running;
        const stepSign = Math.sign(step);
        const reversing = running && controller.lastStepSignX !== 0 && stepSign !== controller.lastStepSignX;
        controller.lastStepSignX = stepSign;

        if (!running)
            controller.pendingWheelX = currentX;

        const base = controller.pendingWheelX;
        const targetX = Math.max(0, Math.min(maxScrollX, base - step));

        if (reversing) {
            smoothX.stop();
            controller.pendingWheelX = currentX;
        }

        const finalX = reversing ? Math.max(0, Math.min(maxScrollX, currentX - step)) : targetX;
        controller.pendingWheelX = finalX;
        controller.isTrackingOverscroll = false;
        controller.overscrollX = 0.0;
        smoothX.to = finalX;
        smoothX.velocity = Math.max(400, Math.abs(finalX - currentX) / 0.18);

        if (!smoothX.running)
            smoothX.start();
    }

    SmoothedAnimation {
        id: touchMomentumY
        target: controller.target
        property: "contentY"
        velocity: 0
        maximumEasingTime: 280
        reversingMode: SmoothedAnimation.Eased
    }

    SmoothedAnimation {
        id: touchMomentumX
        target: controller.target
        property: "contentX"
        velocity: 0
        maximumEasingTime: 280
        reversingMode: SmoothedAnimation.Eased
    }

    function startTouchMomentum(vx, vy) {
        touchMomentumY.stop();
        touchMomentumX.stop();

        const maxY = Math.max(0, controller.target.contentHeight - controller.target.height);
        const maxX = Math.max(0, controller.target.contentWidth - controller.target.width);

        if (controller.orientation !== Qt.Horizontal && Math.abs(vy) > 40 && maxY > 0) {
            const distanceY = Math.max(-900, Math.min(900, vy * 0.22));
            const targetY = Math.max(0, Math.min(maxY, controller.target.contentY - distanceY));
            touchMomentumY.to = targetY;
            touchMomentumY.velocity = Math.max(400, Math.abs(targetY - controller.target.contentY) / 0.28);
            touchMomentumY.start();
        }

        if (controller.orientation !== Qt.Vertical && Math.abs(vx) > 40 && maxX > 0) {
            const distanceX = Math.max(-900, Math.min(900, vx * 0.22));
            const targetX = Math.max(0, Math.min(maxX, controller.target.contentX - distanceX));
            touchMomentumX.to = targetX;
            touchMomentumX.velocity = Math.max(400, Math.abs(targetX - controller.target.contentX) / 0.28);
            touchMomentumX.start();
        }
    }

    WheelHandler {
        id: wheelHandler

        target: null

        acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad

        blocking: false

        onWheel: event => {
            if (!controller.target)
                return;
            const devType = event.device ? event.device.type : "unknown";

            const px = event.pixelDelta;
            const ang = event.angleDelta;

            const maxScrollY = Math.max(0, controller.target.contentHeight - controller.target.height);

            const maxScrollX = Math.max(0, controller.target.contentWidth - controller.target.width);

            const hasPixelDelta = px.x !== 0 || px.y !== 0;

            const isTouchpad = devType === PointerDevice.TouchPad || hasPixelDelta || (ang.x === 0 && ang.y === 0);

            if (isTouchpad) {
                const now = Date.now();

                if (px.x !== 0 || px.y !== 0) {
                    if (now - controller.lastSampleTime > 90) {
                        controller.touchSamples = [];
                    }

                    if (controller.target.flicking) {
                        controller.target.cancelFlick();
                    }
                }

                controller.lastSampleTime = now;

                if (px.x === 0 && px.y === 0) {
                    const cutoff = now - 75;
                    const recent = controller.touchSamples.filter(s => s.t >= cutoff);

                    controller.touchSamples = [];
                    controller.isTrackingOverscroll = false;
                    controller.overscrollY = 0.0;
                    controller.overscrollX = 0.0;

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

                        const gestureVx = controller.orientation !== Qt.Vertical && maxScrollX > 0 ? (totalDx / dt) * 0.85 : 0;

                        const gestureVy = controller.orientation !== Qt.Horizontal ? (totalDy / dt) * 0.85 : 0;

                        const vx = Math.max(-6500, Math.min(6500, controller.lastFlickVelocityX + gestureVx));

                        const vy = Math.max(-6500, Math.min(6500, controller.lastFlickVelocityY + gestureVy));

                        if (Math.abs(vy) > 40 || Math.abs(vx) > 40) {
                            controller.target.cancelFlick();
                            controller.target.flick(vx, vy);
                        } else {}

                        controller.lastFlickVelocityX = 0;
                        controller.lastFlickVelocityY = 0;
                    }

                    event.accepted = false;
                    return;
                }

                controller.touchSamples.push({
                    dx: px.x,
                    dy: px.y,
                    t: now
                });

                if (controller.touchSamples.length > 14)
                    controller.touchSamples.shift();

                if (controller.orientation !== Qt.Horizontal && px.y !== 0) {
                    if (controller.target.contentY <= 0) {
                        if (px.y > 0 || controller.isTrackingOverscroll) {
                            controller.isTrackingOverscroll = true;

                            const resistance = Math.max(0.12, 1.0 - Math.abs(controller.overscrollY) / 160.0);

                            const nextOverscrollY = controller.overscrollY - px.y * resistance * 0.7;

                            if (nextOverscrollY > 0) {
                                controller.overscrollY = 0.0;
                                controller.isTrackingOverscroll = false;
                            } else {
                                controller.overscrollY = Math.max(-120.0, nextOverscrollY);
                            }
                        }
                    } else if (controller.target.contentY >= maxScrollY) {
                        if (px.y < 0 || controller.isTrackingOverscroll) {
                            controller.isTrackingOverscroll = true;

                            const resistance = Math.max(0.12, 1.0 - Math.abs(controller.overscrollY) / 160.0);

                            const nextOverscrollY = controller.overscrollY - px.y * resistance * 0.7;

                            if (nextOverscrollY < 0) {
                                controller.overscrollY = 0.0;
                                controller.isTrackingOverscroll = false;
                            } else {
                                controller.overscrollY = Math.min(120.0, nextOverscrollY);
                            }
                        }
                    } else if (controller.isTrackingOverscroll) {
                        controller.isTrackingOverscroll = false;
                        controller.overscrollY = 0.0;
                    }
                }

                if (controller.orientation !== Qt.Vertical && maxScrollX > 0 && px.x !== 0) {
                    if (controller.target.contentX <= 0) {
                        if (px.x > 0 || controller.isTrackingOverscroll) {
                            controller.isTrackingOverscroll = true;

                            const resistance = Math.max(0.12, 1.0 - Math.abs(controller.overscrollX) / 160.0);

                            const nextOverscrollX = controller.overscrollX - px.x * resistance * 0.7;

                            if (nextOverscrollX > 0) {
                                controller.overscrollX = 0.0;
                                controller.isTrackingOverscroll = false;
                            } else {
                                controller.overscrollX = Math.max(-120.0, nextOverscrollX);
                            }
                        }
                    } else if (controller.target.contentX >= maxScrollX) {
                        if (px.x < 0 || controller.isTrackingOverscroll) {
                            controller.isTrackingOverscroll = true;

                            const resistance = Math.max(0.12, 1.0 - Math.abs(controller.overscrollX) / 160.0);

                            const nextOverscrollX = controller.overscrollX - px.x * resistance * 0.7;

                            if (nextOverscrollX < 0) {
                                controller.overscrollX = 0.0;
                                controller.isTrackingOverscroll = false;
                            } else {
                                controller.overscrollX = Math.min(120.0, nextOverscrollX);
                            }
                        }
                    } else if (controller.isTrackingOverscroll && (controller.orientation === Qt.Horizontal || maxScrollY <= 0)) {
                        controller.isTrackingOverscroll = false;
                        controller.overscrollX = 0.0;
                    }
                } else if (controller.orientation === Qt.Vertical) {
                    controller.overscrollX = 0.0;
                }

                event.accepted = false;
                return;
            }

            event.accepted = true;

            const isHorizontalOnly = controller.target.contentWidth > controller.target.width && controller.target.contentHeight <= controller.target.height;

            const isHorizontal = controller.orientation === Qt.Horizontal || (controller.orientation !== Qt.Vertical && isHorizontalOnly);

            const rawDelta = ang.y !== 0 ? ang.y : ang.x;

            const step = (rawDelta / 120.0) * controller.wheelStepSize;

            if (isHorizontal && (maxScrollX > 0 || controller.orientation === Qt.Horizontal)) {
                controller.applyWheelX(step);
            } else if (controller.orientation !== Qt.Horizontal) {
                controller.applyWheelY(step);
            }
        }
    }
}
