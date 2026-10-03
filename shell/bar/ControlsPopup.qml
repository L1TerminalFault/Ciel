import Quickshell
import Quickshell.Wayland
import Quickshell.Io
import QtQuick
import QtQuick.Layouts
import QtQuick.Effects
import Ciel.Ui

PopupWindow {
    id: root

    property var targetWindow
    property rect anchorRect
    property bool isOpen: false

    property real volumeLevel: 0.50
    property real brightnessLevel: 0.70
    property real targetVolume: 0.50
    property real targetBrightness: 0.70
    property real lastDispatchedVolume: -1.0
    property real lastDispatchedBrightness: -1.0

    function readHardwareLevels() {
        if (!volReader.running) {
            volReader.buffer = "";
            volReader.running = true;
        }
        if (!brightReader.running) {
            brightReader.buffer = "";
            brightReader.running = true;
        }
    }

    function applyVolume(val) {
        root.targetVolume = val;
        if (!volSetter.running)
            root.dispatchVolume();
    }

    function dispatchVolume() {
        root.lastDispatchedVolume = root.targetVolume;
        volSetter.command = ["wpctl", "set-volume", "@DEFAULT_AUDIO_SINK@", root.targetVolume.toFixed(2)];
        volSetter.running = true;
    }

    function applyBrightness(val) {
        root.targetBrightness = val;
        if (!brightSetter.running)
            root.dispatchBrightness();
    }

    function dispatchBrightness() {
        root.lastDispatchedBrightness = root.targetBrightness;
        brightSetter.command = ["brightnessctl", "set", Math.round(root.targetBrightness * 100) + "%", "-q"];
        brightSetter.running = true;
    }

    Process {
        id: volSetter
        command: []
        running: false
        onExited: (code, status) => {
            if (Math.abs(root.targetVolume - root.lastDispatchedVolume) > 0.005) {
                root.dispatchVolume();
            }
        }
    }

    Process {
        id: brightSetter
        command: []
        running: false
        onExited: (code, status) => {
            if (Math.abs(root.targetBrightness - root.lastDispatchedBrightness) > 0.005) {
                root.dispatchBrightness();
            }
        }
    }

    Process {
        id: volReader
        command: ["sh", "-c", "wpctl get-volume @DEFAULT_AUDIO_SINK@ 2>/dev/null | awk '{print $2}' || pactl get-sink-volume @DEFAULT_SINK@ 2>/dev/null | grep -o '[0-9]\\+%' | head -n1 | tr -d '%' | awk '{print $1/100}'"]
        running: false
        property string buffer: ""
        stdout: SplitParser {
            onRead: data => {
                volReader.buffer += data.trim();
            }
        }
        onExited: (code, status) => {
            if (code === 0 && volReader.buffer !== "") {
                var v = parseFloat(volReader.buffer);
                if (!isNaN(v)) {
                    root.volumeLevel = Math.max(0.0, Math.min(1.0, v));
                    root.targetVolume = root.volumeLevel;
                }
            }
            volReader.buffer = "";
        }
    }

    Process {
        id: brightReader
        command: ["sh", "-c", "brightnessctl g 2>/dev/null && brightnessctl m 2>/dev/null || (cat /sys/class/backlight/*/brightness 2>/dev/null | head -n1; cat /sys/class/backlight/*/max_brightness 2>/dev/null | head -n1)"]
        running: false
        property string buffer: ""
        stdout: SplitParser {
            onRead: data => {
                brightReader.buffer += data.trim() + "\n";
            }
        }
        onExited: (code, status) => {
            if (code === 0 && brightReader.buffer.trim() !== "") {
                var lines = brightReader.buffer.trim().split("\n");
                if (lines.length >= 2) {
                    var cur = parseFloat(lines[0]);
                    var max = parseFloat(lines[1]);
                    if (!isNaN(cur) && !isNaN(max) && max > 0) {
                        root.brightnessLevel = Math.max(0.0, Math.min(1.0, cur / max));
                        root.targetBrightness = root.brightnessLevel;
                    }
                }
            }
            brightReader.buffer = "";
        }
    }

    readonly property real startY: -14
    readonly property real startXScale: 0.84
    readonly property real startYScale: 0.74

    readonly property real springY: 4.2
    readonly property real dampingY: 0.34
    readonly property real springX: 4.5
    readonly property real dampingX: 0.36
    readonly property real springPosY: 4.2
    readonly property real dampingPosY: 0.34

    readonly property int fadeInDuration: 95
    readonly property int closeDuration: 130

    function toggle() {
        isOpen = !isOpen;
    }

    onIsOpenChanged: {
        if (isOpen) {
            closeAnim.stop();
            openAnim.restart();
            root.readHardwareLevels();
        } else {
            openAnim.stop();
            closeAnim.restart();
        }
    }

    anchor.window: targetWindow
    anchor.rect: anchorRect
    anchor.edges: Edges.Bottom
    anchor.gravity: Edges.Bottom

    implicitWidth: 380
    implicitHeight: popupContent.height + 50
    color: "transparent"

    visible: root.isOpen || openAnim.running || closeAnim.running

    RectangularShadow {
        anchors.fill: popupContent
        radius: 20
        offset.y: 10
        blur: 38
        spread: -8
        color: "#00000080"
        opacity: popupContent.opacity
    }

    Item {
        id: popupContent
        property real radius: 20
        width: 320
        height: contentLayout.implicitHeight + 36
        anchors.horizontalCenter: parent.horizontalCenter
        y: 0
        opacity: 0.0

        transform: Scale {
            id: scaleTransform
            origin.x: popupContent.width / 2
            origin.y: 0
            xScale: root.startXScale
            yScale: root.startYScale
        }

        CielSquircle {
            anchors.fill: parent
            radius: popupContent.radius
            color: Theme.surface
            borderColor: Theme.surfaceHover
            borderWidth: 1
        }

        ColumnLayout {
            id: contentLayout
            anchors.fill: parent
            anchors.margins: 18
            spacing: 14

            RowLayout {
                Layout.fillWidth: true
                Text {
                    text: "Control Center"
                    color: Theme.textPrimary
                    font.pixelSize: 15
                    font.bold: true
                    renderType: Text.NativeRendering
                }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: Theme.surfaceHover
            }

            CielSlider {
                Layout.fillWidth: true
                height: 42
                value: root.volumeLevel
                onSliderMoved: val => root.applyVolume(val)

                VolumeIcon {
                    value: root.volumeLevel
                    color: Theme.textPrimary
                }
            }

            CielSlider {
                Layout.fillWidth: true
                height: 42
                value: root.brightnessLevel
                onSliderMoved: val => root.applyBrightness(val)

                BrightnessIcon {
                    value: root.brightnessLevel
                    color: Theme.textPrimary
                }
            }
        }

        SequentialAnimation {
            id: openAnim

            PauseAnimation {
                duration: 140
            }

            ParallelAnimation {
                SpringAnimation {
                    target: popupContent
                    property: "y"
                    from: root.startY
                    to: 0
                    spring: root.springPosY
                    damping: root.dampingPosY
                    epsilon: 0.001
                }
                SpringAnimation {
                    target: scaleTransform
                    property: "yScale"
                    from: root.startYScale
                    to: 1.0
                    spring: root.springY
                    damping: root.dampingY
                    epsilon: 0.001
                }
                SpringAnimation {
                    target: scaleTransform
                    property: "xScale"
                    from: root.startXScale
                    to: 1.0
                    spring: root.springX
                    damping: root.dampingX
                    epsilon: 0.001
                }
                NumberAnimation {
                    target: popupContent
                    property: "opacity"
                    from: 0.0
                    to: 1.0
                    duration: root.fadeInDuration
                    easing.type: Easing.OutQuad
                }
            }
        }

        ParallelAnimation {
            id: closeAnim

            NumberAnimation {
                target: popupContent
                property: "y"
                to: root.startY
                duration: root.closeDuration
                easing.type: Easing.InQuad
            }
            NumberAnimation {
                target: scaleTransform
                property: "yScale"
                to: root.startYScale
                duration: root.closeDuration + 10
                easing.type: Easing.InQuad
            }
            NumberAnimation {
                target: scaleTransform
                property: "xScale"
                to: root.startXScale
                duration: root.closeDuration + 10
                easing.type: Easing.InQuad
            }
            NumberAnimation {
                target: popupContent
                property: "opacity"
                to: 0.0
                duration: root.closeDuration - 10
                easing.type: Easing.InQuad
            }
        }
    }
}
