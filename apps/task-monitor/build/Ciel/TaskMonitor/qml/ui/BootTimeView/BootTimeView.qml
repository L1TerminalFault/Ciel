import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Ciel.Ui
import Ciel.TaskMonitor 1.0

Item {
    id: root

    BootPerformanceModel {
        id: bootModel
    }

    CielScrollView {
        id: mainScroll
        anchors.fill: parent
        contentWidth: availableWidth
        contentHeight: Math.max(root.height, contentCol.implicitHeight)

        ColumnLayout {
            id: contentCol
            width: mainScroll.availableWidth
            height: Math.max(mainScroll.height, implicitHeight)
            spacing: 14

            CielSquircle {
                id: statsCard
                Layout.preferredWidth: mainScroll.availableWidth
                implicitHeight: statsCol.implicitHeight + 32
                Layout.preferredHeight: implicitHeight
                color: "transparent"
                borderWidth: 1
                borderColor: Theme.border

                ColumnLayout {
                    id: statsCol
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 14

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 0

                        Text {
                            text: "Startup Performance"
                            font.pixelSize: 15
                            font.weight: Font.DemiBold
                            color: Theme.textPrimary
                        }

                        Item {
                            Layout.fillWidth: true
                        }

                        Text {
                            text: bootModel.totalTimeString
                            font.pixelSize: 15
                            font.weight: Font.DemiBold
                            color: Theme.textPrimary
                        }
                    }

                    CielStackedBar {
                        Layout.fillWidth: true
                        barHeight: 12
                        segmentSpacing: 6
                        segments: bootModel.segments
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 8

                        Repeater {
                            model: bootModel.segments

                            delegate: RowLayout {
                                id: legendRow
                                required property var modelData
                                required property int index

                                Layout.fillWidth: true
                                spacing: 10

                                Item {
                                    Layout.preferredWidth: 10
                                    Layout.preferredHeight: 10
                                    Layout.alignment: Qt.AlignVCenter

                                    CielSquircle {
                                        anchors.fill: parent
                                        color: legendRow.modelData.color || Theme.accent
                                    }
                                }

                                Text {
                                    text: legendRow.modelData.label
                                    font.pixelSize: 13
                                    font.weight: Font.Medium
                                    color: Theme.textPrimary
                                    Layout.alignment: Qt.AlignVCenter
                                }

                                Item {
                                    Layout.fillWidth: true
                                }

                                RowLayout {
                                    spacing: 4
                                    Layout.alignment: Qt.AlignVCenter

                                    Text {
                                        text: Number(legendRow.modelData.value).toFixed(2)
                                        font.pixelSize: 13
                                        font.weight: Font.DemiBold
                                        color: Theme.textPrimary
                                    }

                                    Text {
                                        text: "s"
                                        font.pixelSize: 13
                                        font.weight: Font.Normal
                                        color: Theme.textSecondary
                                    }
                                }
                            }
                        }
                    }
                }
            }

            CielSquircle {
                id: servicesCard
                Layout.preferredWidth: mainScroll.availableWidth
                Layout.fillHeight: true
                color: "transparent"
                borderWidth: 1
                borderColor: Theme.border

                WheelHandler {
                    target: null
                    acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad
                    blocking: true
                }

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 1
                    spacing: 0

                    Item {
                        Layout.fillWidth: true
                        height: 38
                        clip: true

                        CielSquircle {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.top: parent.top
                            height: parent.height + 32
                            color: Theme.transparent
                        }

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 14
                            anchors.rightMargin: 14

                            Text {
                                text: "Startup Services"
                                font.pixelSize: 12
                                font.weight: Font.DemiBold
                                color: Theme.textSecondary
                                Layout.alignment: Qt.AlignVCenter
                            }

                            Item {
                                Layout.fillWidth: true
                            }

                            Text {
                                text: "Startup Duration"
                                font.pixelSize: 12
                                font.weight: Font.DemiBold
                                color: Theme.textSecondary
                                Layout.alignment: Qt.AlignVCenter
                            }
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        height: 1
                        color: Theme.border
                    }

                    CielScrollView {
                        id: serviceScroll
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        contentWidth: availableWidth
                        contentHeight: serviceRowsCol.implicitHeight

                        ColumnLayout {
                            id: serviceRowsCol
                            width: serviceScroll.availableWidth
                            spacing: 0

                            Repeater {
                                model: bootModel

                                delegate: Item {
                                    id: serviceRow
                                    required property int index
                                    required property string unitName
                                    required property string durationString
                                    required property real relativeRatio

                                    width: serviceRowsCol.width
                                    height: 40

                                    Rectangle {
                                        anchors.fill: parent
                                        anchors.bottomMargin: serviceRow.index < bootModel.rowCount() - 1 ? 1 : 0
                                        color: "transparent"
                                    }

                                    MouseArea {
                                        id: rowMouse
                                        anchors.fill: parent
                                        hoverEnabled: true
                                    }

                                    RowLayout {
                                        anchors.fill: parent
                                        anchors.leftMargin: 14
                                        anchors.rightMargin: 14
                                        spacing: 10

                                        CielIcon {
                                            icon: "gear-six"
                                            size: 16
                                            color: Theme.textSecondary
                                            Layout.alignment: Qt.AlignVCenter
                                        }

                                        Text {
                                            text: serviceRow.unitName
                                            font.pixelSize: 12
                                            font.weight: Font.Medium
                                            color: Theme.textPrimary
                                            elide: Text.ElideRight
                                            Layout.preferredWidth: Math.min(280, serviceRowsCol.width * 0.4)
                                            Layout.alignment: Qt.AlignVCenter
                                        }

                                        Item {
                                            Layout.fillWidth: true
                                            height: 16
                                            Layout.alignment: Qt.AlignVCenter

                                            Rectangle {
                                                anchors.left: parent.left
                                                anchors.right: parent.right
                                                anchors.verticalCenter: parent.verticalCenter
                                                height: 4
                                                radius: 2
                                                color: Theme.surface
                                            }

                                            Rectangle {
                                                anchors.left: parent.left
                                                anchors.verticalCenter: parent.verticalCenter
                                                height: 4
                                                radius: 2
                                                color: Theme.accent
                                                opacity: 0.75
                                                width: Math.max(4, parent.width * serviceRow.relativeRatio)
                                            }
                                        }

                                        Text {
                                            text: serviceRow.durationString
                                            font.pixelSize: 12
                                            font.weight: Font.DemiBold
                                            color: Theme.textPrimary
                                            Layout.alignment: Qt.AlignVCenter
                                        }
                                    }

                                    Rectangle {
                                        anchors.left: parent.left
                                        anchors.right: parent.right
                                        anchors.bottom: parent.bottom
                                        height: 1
                                        color: Theme.border
                                        visible: serviceRow.index < bootModel.rowCount() - 1
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
