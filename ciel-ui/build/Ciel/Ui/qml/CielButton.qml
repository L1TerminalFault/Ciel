import QtQuick
import Ciel.Ui

CielSurface {
    id: root

    property string text: "Button"
    property bool primary: false
    property bool destructive: false
    property string icon: ""
    property int iconSize: Theme.SMALL
    property bool iconOnly: false

    readonly property bool hasIcon: icon !== ""
    readonly property bool hasText: text !== ""
    readonly property int hPadding: Theme.metrics.spacingLg
    readonly property real contentWidth: (hasIcon ? root.iconSize : 0) + (hasIcon && hasText ? 8 : 0) + (hasText ? label.implicitWidth : 0)
    readonly property real fullWidth: Math.max(implicitHeight, contentWidth + (hPadding * 2))

    property real bloomProgress: iconOnly ? 1.0 : 0.0
    readonly property real iconOffset: (hasIcon && hasText) ? ((contentWidth - root.iconSize) / 2) * (1.0 - bloomProgress) : 0

    clip: true
    implicitHeight: 36
    implicitWidth: iconOnly ? implicitHeight : fullWidth

    Behavior on implicitWidth {
        CielSpring {
            damping: 4.5
            spring: 6.0
            mass: 3.5
            epsilon: 0.005
        }
    }

    Behavior on bloomProgress {
        CielSpring {
            damping: 4.5
            spring: 6.0
            mass: 3.5
            epsilon: 0.005
        }
    }

    color: {
        if (root.destructive) {
            if (pressed)
                return (typeof Theme.dangerPressed !== "undefined") ? Theme.dangerPressed : "#BE123C";
            if (hovered)
                return (typeof Theme.dangerHover !== "undefined") ? Theme.dangerHover : "#F43F5E";
            return (typeof Theme.danger !== "undefined") ? Theme.danger : "#E11D48";
        }
        if (!primary) {
            if (pressed)
                return Theme.surfacePressed;
            if (hovered)
                return Theme.surfaceHover;
            return Theme.surface;
        } else {
            if (pressed)
                return Theme.accentPressed;
            if (hovered)
                return Theme.accentHover;
            return Theme.accent;
        }
    }

    CielIcon {
        id: ic
        visible: root.hasIcon
        icon: root.icon
        size: root.iconSize
        color: (root.primary || root.destructive) ? "#FFFFFF" : Theme.textPrimary
        anchors.verticalCenter: parent.verticalCenter
        x: Math.round((parent.width / 2) - (root.iconSize / 2) - root.iconOffset)

        Behavior on color {
            enabled: Theme.transitionMs > 0
            ColorAnimation {
                duration: Theme.transitionMs
            }
        }
    }

    Text {
        id: label
        visible: root.hasText && opacity > 0.0
        text: root.text
        color: (root.primary || root.destructive) ? "#FFFFFF" : Theme.textPrimary
        font.pixelSize: 13
        font.weight: Font.Medium
        renderType: Text.NativeRendering
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: root.hasIcon ? ic.right : undefined
        anchors.leftMargin: 8
        anchors.horizontalCenter: root.hasIcon ? undefined : parent.horizontalCenter

        opacity: Math.max(0.0, 1.0 - root.bloomProgress * 3.5)
        scale: 0.90 + (0.10 * Math.max(0.0, 1.0 - root.bloomProgress))
        transformOrigin: Item.Left

        Behavior on color {
            enabled: Theme.transitionMs > 0
            ColorAnimation {
                duration: Theme.transitionMs
            }
        }
    }
}
