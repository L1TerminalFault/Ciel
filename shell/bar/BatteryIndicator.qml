import Quickshell
import Quickshell.Io
import QtQuick
import QtQuick.Layouts
import Ciel.Ui

Rectangle {
    id: root

    property int percentage: 100
    property bool isCharging: false
    // Low battery warning red vs standard accent color
    property color batteryColor: percentage <= 20 && !isCharging ? "#FF3B30" : Theme.accent

    implicitWidth: 34
    implicitHeight: 34
    Layout.preferredWidth: implicitWidth
    Layout.preferredHeight: implicitHeight

    radius: Theme.metrics.radiusSm
    color: mouseArea.containsMouse ? Theme.surfaceHover : Theme.surface

    Behavior on color {
        ColorAnimation {
            duration: 120
            easing.type: Easing.OutQuad
        }
    }

    Process {
        id: batReader
        command: ["sh", "-c", "cat /sys/class/power_supply/BAT*/capacity 2>/dev/null | head -n1; cat /sys/class/power_supply/BAT*/status 2>/dev/null | head -n1"]
        running: false
        property string buffer: ""

        stdout: SplitParser {
            onRead: data => {
                batReader.buffer += data + "\n";
            }
        }

        onExited: (code, status) => {
            if (code === 0 && batReader.buffer.trim() !== "") {
                var lines = batReader.buffer.trim().split("\n");
                if (lines.length >= 1) {
                    var cap = parseInt(lines[0], 10);
                    if (!isNaN(cap))
                        root.percentage = Math.max(0, Math.min(100, cap));
                }
                if (lines.length >= 2) {
                    var st = lines[1].trim();
                    root.isCharging = (st === "Charging" || st === "Full");
                }
            }
            batReader.buffer = "";
        }
    }

    Timer {
        interval: 3500
        running: true
        repeat: true
        onTriggered: {
            if (!batReader.running) {
                batReader.buffer = "";
                batReader.running = true;
            }
        }
    }

    Component.onCompleted: {
        batReader.running = true;
    }

    Item {
        id: batteryBody
        anchors.centerIn: parent
        width: 31
        height: 16.5

        Rectangle {
            id: frame
            x: 0
            y: 0
            width: 28
            height: 16.5
            radius: 5.0
            color: Theme.surface
            border.color: Theme.surfaceHover
            border.width: 1

            Rectangle {
                id: fillBar
                x: 1.5
                y: 1.5
                height: parent.height - 3
                radius: 3.5
                color: root.batteryColor
                width: Math.max(3, (frame.width - 3) * (root.percentage / 100.0))

                Behavior on width {
                    NumberAnimation {
                        duration: 250
                        easing.type: Easing.OutQuad
                    }
                }

                Behavior on color {
                    ColorAnimation {
                        duration: 220
                        easing.type: Easing.OutQuad
                    }
                }
            }

            Text {
                id: percentText
                anchors.centerIn: parent
                text: String(root.percentage)
                color: "#FFFFFF"
                font.pixelSize: root.percentage === 100 ? 9.5 : 10.5
                font.bold: true
                renderType: Text.NativeRendering

                scale: root.isCharging ? 0.0 : 1.0
                opacity: root.isCharging ? 0.0 : 1.0

                Behavior on scale {
                    SpringAnimation {
                        spring: 4.8
                        damping: 0.30
                        epsilon: 0.001
                    }
                }

                Behavior on opacity {
                    NumberAnimation {
                        duration: 100
                        easing.type: Easing.OutQuad
                    }
                }
            }

            ChargerIcon {
                anchors.centerIn: parent
                anchors.verticalCenterOffset: 1.5
                active: root.isCharging
                color: "#FFFFFF"
            }
        }

        Rectangle {
            id: terminalNub
            anchors.left: frame.right
            anchors.leftMargin: 1
            anchors.verticalCenter: frame.verticalCenter
            width: 2.2
            height: 6.0
            radius: 1.1
            color: Theme.surfaceHover
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
    }
}
