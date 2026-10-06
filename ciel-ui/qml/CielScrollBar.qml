import QtQuick
import QtQuick.Controls as T
import Ciel.Ui

T.ScrollBar {
    id: bar

    property color thumbColor: Theme.border

    interactive: true
    hoverEnabled: true
    width: orientation === Qt.Vertical ? 14 : undefined
    height: orientation === Qt.Horizontal ? 14 : undefined

    implicitWidth: 14
    implicitHeight: 14
    contentItem: Item {
        id: thumbContainer
        implicitWidth: bar.orientation === Qt.Vertical ? 8 : 0
        implicitHeight: bar.orientation === Qt.Horizontal ? 8 : 0

        HoverHandler {
            id: thumbHover
        }

        Rectangle {
            id: thumbVisual
            anchors.centerIn: parent
            width: bar.orientation === Qt.Vertical ? 4 : parent.width
            height: bar.orientation === Qt.Vertical ? parent.height : 4
            radius: 2
            color: bar.thumbColor

            opacity: {
                if (bar.pressed)
                    return 0.95;
                if (thumbHover.hovered)
                    return 0.85;
                if (bar.hovered)
                    return 0.55;
                if (bar.parent && (bar.parent.moving || bar.parent.flicking))
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
                id: thumbScale
                origin.x: thumbVisual.width / 2
                origin.y: thumbVisual.height / 2

                xScale: {
                    if (bar.orientation === Qt.Vertical) {
                        if (bar.pressed)
                            return 1.75;
                        if (thumbHover.hovered)
                            return 1.30;
                        return 1.0;
                    } else {
                        if (bar.pressed)
                            return 0.84;
                        if (thumbHover.hovered)
                            return 0.94;
                        return 1.0;
                    }
                }

                yScale: {
                    if (bar.orientation === Qt.Vertical) {
                        if (bar.pressed)
                            return 0.84;
                        if (thumbHover.hovered)
                            return 0.94;
                        return 1.0;
                    } else {
                        if (bar.pressed)
                            return 1.75;
                        if (thumbHover.hovered)
                            return 1.30;
                        return 1.0;
                    }
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
