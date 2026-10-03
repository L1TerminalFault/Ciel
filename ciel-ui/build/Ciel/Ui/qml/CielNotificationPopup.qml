import QtQuick
import QtQuick.Layouts
import Ciel.Ui

CielSurface {
    id: root

    property int notificationId: 0
    property string appName: "System"
    property string summary: ""
    property string body: ""

    implicitWidth: 340
    implicitHeight: layout.implicitHeight + (Theme.metrics.spacingMd * 2)
    radius: Theme.metrics.radiusMd

    // Floating card styling
    color: Theme.surfaceHover

    ColumnLayout {
        id: layout
        anchors.fill: parent
        anchors.margins: Theme.metrics.spacingMd
        spacing: Theme.metrics.spacingXs

        RowLayout {
            Layout.fillWidth: true
            Text {
                text: root.appName.toUpperCase()
                font.pixelSize: 10
                font.weight: Font.Bold
                color: Theme.accent
            }
            Item {
                Layout.fillWidth: true
            }
            Text {
                text: "Dismiss"
                font.pixelSize: 10
                color: Theme.textSecondary
            }
        }

        Text {
            text: root.summary
            font.pixelSize: 13
            font.weight: Font.SemiBold
            color: Theme.textPrimary
            Layout.fillWidth: true
            elide: Text.ElideRight
        }

        Text {
            text: root.body
            font.pixelSize: 11
            color: Theme.textSecondary
            Layout.fillWidth: true
            wrapMode: Text.Wrap
            visible: text.length > 0
        }
    }

    onClicked: {
        NotificationsModel.dismiss(root.notificationId);
    }
}
