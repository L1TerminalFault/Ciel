import QtQuick
import QtQuick.Layouts
import Ciel.Ui

Item {
    id: root
    implicitWidth: breadCrumbRow.implicitWidth
    implicitHeight: 36

    property var model
    RowLayout {
        anchors.fill: parent
        CielScrollView {
            id: tabBarScrollView
            orientation: Qt.Horizontal
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            height: 36
            showScrollBar: false
            contentWidth: breadCrumbRow.implicitWidth

            RowLayout {
                id: breadCrumbRow
                spacing: 4
                height: 36
                Repeater {
                    model: root.model
                    delegate: Item {
                        implicitWidth: innerRow.implicitWidth
                        implicitHeight: innerRow.implicitHeight
                        RowLayout {
                            id: innerRow
                            CielSquircle {
                                Layout.preferredWidth: title.implicitWidth + 16
                                Layout.preferredHeight: title.implicitHeight + 8
                                color: Theme.surface

                                Text {
                                    id: title
                                    text: modelData.title
                                    color: Theme.textPrimary
                                    anchors.centerIn: parent
                                }
                            }
                            CielIcon {
                                icon: "caret-right"
                                size: Theme.XSMALL
                            }
                        }
                    }
                }
            }
        }
    }
}
