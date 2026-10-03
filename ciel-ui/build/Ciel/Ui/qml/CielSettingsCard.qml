import QtQuick
import QtQuick.Layouts
import QtQuick.Effects
import Ciel.Ui

ColumnLayout {
    id: root

    property string title: ""
    property string footer: ""
    property real cardRadius: 14

    default property alias rows: rowContainer.data

    Layout.fillWidth: true
    spacing: 10

    Text {
        visible: root.title !== ""
        text: root.title
        color: Theme.textSecondary
        font.pixelSize: 11
        font.weight: Font.DemiBold
        font.capitalization: Font.AllUppercase
        Layout.leftMargin: 12
    }

    CielSquircle {
        id: cardBody
        Layout.fillWidth: true
        radius: root.cardRadius
        color: Theme.surface
        borderColor: Theme.border
        borderWidth: 1

        implicitHeight: rowContainer.implicitHeight + 2
        Layout.preferredHeight: implicitHeight

        Item {
            id: maskedContentArea
            anchors.fill: parent
            anchors.margins: 1

            layer.enabled: true
            layer.smooth: true
            layer.effect: MultiEffect {
                maskEnabled: true
                maskSource: squircleMaskSource
            }

            ShaderEffectSource {
                id: squircleMaskSource
                sourceItem: squircleMaskShape
                hideSource: true
                live: false
            }

            CielSquircle {
                id: squircleMaskShape
                anchors.fill: parent
                radius: Math.max(0, root.cardRadius - 1)
                color: "white"
            }

            ColumnLayout {
                id: rowContainer
                anchors.fill: parent
                spacing: 0
            }
        }
    }

    Text {
        visible: root.footer !== ""
        text: root.footer
        color: Theme.textSecondary
        font.pixelSize: 12
        wrapMode: Text.WordWrap
        Layout.fillWidth: true
        Layout.leftMargin: 12
        Layout.rightMargin: 12
    }
}
