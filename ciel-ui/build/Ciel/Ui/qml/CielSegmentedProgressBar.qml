import QtQuick

Item {
    id: root

    property real progress: 0.0
    property var segments: []
    property bool indeterminate: false

    property color barColor: Theme.accent
    property color trackColor: Theme.surfaceHover
    property int radius: Math.round(height / 2)
    property real segmentSpacing: 2

    readonly property bool hasSegments: segments && segments.length > 1
    readonly property int segmentCount: hasSegments ? segments.length : 1

    implicitWidth: 200
    implicitHeight: 6
    clip: true

    function resolveProgress(val) {
        if (val === undefined || val === null)
            return 0.0;
        if (typeof val === "number")
            return Math.max(0.0, Math.min(1.0, val));
        if (typeof val === "object" && val.progress !== undefined)
            return Math.max(0.0, Math.min(1.0, val.progress));
        return 0.0;
    }

    CielSquircle {
        anchors.fill: parent
        color: root.trackColor
        radius: root.radius
        visible: !root.hasSegments
    }

    CielSquircle {
        id: singleFill
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: Math.max(0, Math.min(root.width, root.width * root.progress))
        color: root.barColor
        radius: root.radius
        visible: !root.hasSegments && !root.indeterminate

        Behavior on width {
            CielSpring {
                damping: 3.2
                spring: 7.0
                mass: 1.0
                epsilon: 0.001
            }
        }
    }

    Row {
        id: segmentRow
        anchors.fill: parent
        spacing: root.segmentSpacing
        visible: root.hasSegments && !root.indeterminate

        Repeater {
            model: root.segments

            Item {
                id: segmentSlot
                height: segmentRow.height
                width: Math.max(0, (segmentRow.width - ((root.segmentCount - 1) * root.segmentSpacing)) / root.segmentCount)

                CielSquircle {
                    anchors.fill: parent
                    color: root.trackColor
                    radius: root.radius
                }

                CielSquircle {
                    id: segmentFill
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    width: Math.max(0, Math.min(parent.width, parent.width * root.resolveProgress(modelData)))
                    color: root.barColor
                    radius: root.radius

                    Behavior on width {
                        CielSpring {
                            damping: 3.2
                            spring: 7.0
                            mass: 1.0
                            epsilon: 0.001
                        }
                    }
                }
            }
        }
    }

    CielSquircle {
        id: indeterminatePulse
        height: parent.height
        width: Math.max(40, parent.width * 0.3)
        color: root.barColor
        radius: root.radius
        visible: root.indeterminate
        opacity: 0.85

        SequentialAnimation {
            running: root.indeterminate && root.visible
            loops: Animation.Infinite

            NumberAnimation {
                target: indeterminatePulse
                property: "x"
                from: -indeterminatePulse.width
                to: root.width
                duration: 950
                easing.type: Easing.InOutQuad
            }
            PauseAnimation {
                duration: 80
            }
        }
    }
}
