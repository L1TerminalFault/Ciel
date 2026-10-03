import QtQuick
import QtQuick.Layouts
import Ciel.Ui

Item {
    id: root

    property string downloadId: ""
    property string filename: ""
    property string destination: ""
    property string category: "Other"
    property string state: "downloading"
    property real bytesReceived: 0
    property real totalBytes: 0
    property real speed: 0
    property var segments: []
    property string errorMessage: ""

    signal pauseRequested(string id)
    signal resumeRequested(string id)
    signal cancelRequested(string id)
    signal openFolderRequested(string path)
    signal retryRequested(string id)

    implicitHeight: 64
    implicitWidth: 340

    HoverHandler {
        id: hoverHandler
    }

    CielSquircle {
        anchors.fill: parent
        radius: 8
        color: Theme.surfaceHover
        opacity: hoverHandler.hovered ? 1.0 : 0.0
        z: -1

        Behavior on opacity {
            NumberAnimation {
                duration: Theme.transitionMs > 0 ? Theme.transitionMs : 100
                easing.type: Easing.OutQuad
            }
        }
    }

    function resolveCategoryIcon(cat) {
        switch (cat) {
        case "Images":
            return "image";
        case "Documents":
            return "file-text";
        case "Archives":
            return "file-zip";
        case "Software":
            return "package";
        case "Audio":
            return "speaker-high";
        case "Video":
            return "video";
        case "Code":
            return "code";
        default:
            return "file";
        }
    }

    function formatBytes(bytes) {
        if (!bytes || bytes <= 0)
            return "0 B";
        var k = 1024;
        var sizes = ["B", "KB", "MB", "GB", "TB"];
        var i = Math.floor(Math.log(bytes) / Math.log(k));
        return (bytes / Math.pow(k, i)).toFixed(i > 1 ? 1 : 0) + " " + sizes[i];
    }

    function formatSpeed(bytesPerSec) {
        if (!bytesPerSec || bytesPerSec <= 0)
            return "0 B/s";
        return formatBytes(bytesPerSec) + "/s";
    }

    function formatEta(received, total, spd) {
        if (!spd || spd < 1024 || !total || total <= received)
            return "";
        var remaining = Math.round((total - received) / spd);
        if (remaining < 60)
            return remaining + "s left";
        var mins = Math.floor(remaining / 60);
        var secs = remaining % 60;
        return mins + "m " + (secs > 0 ? secs + "s left" : "left");
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 10

        CielSquircle {
            Layout.preferredWidth: 38
            Layout.preferredHeight: 38
            Layout.alignment: Qt.AlignVCenter
            color: Theme.surfaceHover
            radius: 8

            CielIcon {
                anchors.centerIn: parent
                icon: root.resolveCategoryIcon(root.category)
                color: Theme.textPrimary
                size: Theme.MEDIUM
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter
            spacing: 3

            Text {
                Layout.fillWidth: true
                text: root.filename
                color: Theme.textPrimary
                font.pixelSize: 13
                font.weight: Font.Medium
                elide: Text.ElideMiddle
                renderType: Text.NativeRendering
            }

            CielSegmentedProgressBar {
                Layout.fillWidth: true
                Layout.preferredHeight: 5
                visible: root.state === "downloading" || root.state === "paused"
                segments: root.segments
                progress: root.totalBytes > 0 ? (root.bytesReceived / root.totalBytes) : 0.0
                indeterminate: root.totalBytes <= 0 && root.state === "downloading"
            }

            Text {
                Layout.fillWidth: true
                color: root.state === "failed" ? Theme.accent : Theme.textSecondary
                font.pixelSize: 11
                elide: Text.ElideRight
                renderType: Text.NativeRendering

                text: {
                    if (root.state === "downloading") {
                        var eta = root.formatEta(root.bytesReceived, root.totalBytes, root.speed);
                        var status = root.formatBytes(root.bytesReceived);
                        if (root.totalBytes > 0)
                            status += " / " + root.formatBytes(root.totalBytes);
                        status += " • " + root.formatSpeed(root.speed);
                        if (eta !== "")
                            status += " • " + eta;
                        return status;
                    } else if (root.state === "paused") {
                        return "Paused • " + root.formatBytes(root.bytesReceived) + " / " + root.formatBytes(root.totalBytes);
                    } else if (root.state === "finished") {
                        return "Completed • " + root.category;
                    } else if (root.state === "failed") {
                        return root.errorMessage !== "" ? root.errorMessage : "Download failed";
                    }
                    return "";
                }
            }
        }

        Row {
            Layout.alignment: Qt.AlignVCenter
            spacing: 2
            z: 10

            CielIconButton {
                visible: root.state === "downloading"
                icon: "pause"
                size: Theme.SMALL
                onClicked: root.pauseRequested(root.downloadId)
            }

            CielIconButton {
                visible: root.state === "paused"
                icon: "play"
                size: Theme.SMALL
                onClicked: root.resumeRequested(root.downloadId)
            }

            CielIconButton {
                visible: root.state === "downloading" || root.state === "paused"
                icon: "x"
                size: Theme.SMALL
                onClicked: root.cancelRequested(root.downloadId)
            }

            CielIconButton {
                visible: root.state === "finished"
                icon: "folder"
                size: Theme.SMALL
                onClicked: root.openFolderRequested(root.destination)
            }

            CielIconButton {
                visible: root.state === "failed"
                icon: "arrow-clockwise"
                size: Theme.SMALL
                onClicked: root.retryRequested(root.downloadId)
            }
        }
    }
}
