import QtQuick
import QtQuick.Layouts
import Ciel.Ui

Item {
    id: root

    property var segments: []
    property real barHeight: 17
    property real segmentSpacing: 3.5
    property int hoveredIndex: -1

    readonly property real totalValue: {
        let total = 0.0;
        if (!segments)
            return 0.0;
        for (let i = 0; i < segments.length; i++) {
            total += Math.max(0.0, Number(segments[i].value) || 0.0);
        }
        return total;
    }

    readonly property int activeCount: {
        let count = 0;
        if (!segments)
            return 0;
        for (let i = 0; i < segments.length; i++) {
            if ((Number(segments[i].value) || 0.0) > 0.0)
                count++;
        }
        return count;
    }

    readonly property real usableWidth: Math.max(0.0, width - Math.max(0, activeCount - 1) * segmentSpacing)

    implicitWidth: 320
    implicitHeight: barHeight
    clip: true

    Row {
        id: segmentRow
        anchors.fill: parent
        spacing: root.segmentSpacing

        Repeater {
            model: root.segments

            delegate: Item {
                id: segWrapper
                required property int index
                required property var modelData

                readonly property real segVal: Math.max(0.0, Number(modelData.value) || 0.0)
                readonly property real targetWidth: (root.totalValue > 0 && segVal > 0) ? Math.max(root.barHeight, (segVal / root.totalValue) * root.usableWidth) : 0.0

                width: targetWidth
                height: root.barHeight
                visible: segVal > 0

                opacity: {
                    if (root.hoveredIndex === -1)
                        return 1.0;
                    return (root.hoveredIndex === index) ? 1.0 : 0.4;
                }

                Behavior on width {
                    CielSpring {
                        damping: 13.5
                        spring: 6.5
                        mass: 3.0
                        epsilon: 0.002
                    }
                }

                Behavior on opacity {
                    NumberAnimation {
                        duration: 120
                        easing.type: Easing.OutQuad
                    }
                }

                CielSquircle {
                    anchors.fill: parent
                    color: segWrapper.modelData.color || Theme.accent
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: root.hoveredIndex = segWrapper.index
                    onExited: {
                        if (root.hoveredIndex === segWrapper.index) {
                            root.hoveredIndex = -1;
                        }
                    }
                }
            }
        }
    }
}
