import QtQuick
import Ciel.Ui
import QtQuick.Layouts

Item {
    id: root
    width: parent ? parent.width : 0
    height: parent ? parent.height : 0

    property var model
    property var currentTabId

    signal tabSelected(var id)
    signal tabAddRequested
    signal tabCloseRequested(var id)

    Rectangle {
        anchors.fill: parent
        color: Theme.background
    }

    RowLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.leftMargin: 8
        anchors.rightMargin: 8
        height: 36
        spacing: 8

        CielScrollView {
            id: tabBarScrollView
            orientation: Qt.Horizontal
            Layout.fillWidth: true
            Layout.fillHeight: true
            showScrollBar: false
            contentWidth: tabRow.implicitWidth
            RowLayout {
                id: tabRow
                spacing: 8
                height: tabBarScrollView.height
                Repeater {
                    id: tabRepeater
                    model: root.model
                    delegate: Item {
                        id: delegateRoot
                        Layout.fillWidth: true
                        Layout.maximumWidth: 200
                        Layout.minimumWidth: 100
                        Layout.preferredWidth: 200
                        Layout.preferredHeight: 30
                        Layout.alignment: Qt.AlignLeft
                        transform: Translate {
                            id: slide
                            y: 40
                        }
                        opacity: 0
                        Behavior on opacity {
                            NumberAnimation {
                                duration: 300
                                easing.type: Easing.OutCubic
                            }
                        }
                        CielSpring {
                            id: slideAnim
                            target: slide
                            property: "y"
                            to: 0

                            damping: 0.32
                            spring: 4.8
                            mass: 0.9
                            epsilon: 0.01
                        }
                        Component.onCompleted: {
                            opacity = 1;
                            slideAnim.start();
                        }

                        readonly property bool isCurrent: String(model.id) === String(root.currentTabId)
                        readonly property bool isNextCurrent: {
                            root.currentTabId;
                            let next = tabRepeater.itemAt(index + 1);
                            return next ? next.isCurrent : false;
                        }

                        CielSquircle {
                            id: tabShape
                            topOnly: true
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: -4
                            height: 32
                            width: 200
                            color: delegateRoot.isCurrent ? Theme.surface : Theme.background

                            property bool isPressed: false

                            transform: Scale {
                                origin.x: tabShape.width / 2
                                origin.y: tabShape.height / 2
                                xScale: tabShape.isPressed ? 1.02 : 1.0
                                yScale: tabShape.isPressed ? 0.92 : 1.0

                                Behavior on xScale {
                                    CielSpring {
                                        damping: 0.28
                                        spring: 5.4
                                        mass: 0.9
                                        epsilon: 0.001
                                    }
                                }
                                Behavior on yScale {
                                    CielSpring {
                                        damping: 0.28
                                        spring: 5.4
                                        mass: 0.9
                                        epsilon: 0.001
                                    }
                                }
                            }
                            MouseArea {
                                anchors.fill: parent

                                onPressed: {
                                    tabShape.isPressed = true;
                                }

                                onClicked: {
                                    tabShape.isPressed = false;
                                    root.tabSelected(model.id);
                                }

                                onCanceled: {
                                    tabShape.isPressed = false;
                                }
                            }
                            RowLayout {
                                anchors.fill: parent
                                anchors.leftMargin: 8
                                spacing: 8

                                CielIcon {
                                    icon: model.icon
                                    visible: model.icon !== ""
                                }

                                Text {
                                    text: model.title
                                    color: Theme.textPrimary
                                }

                                Item {
                                    Layout.fillWidth: true
                                }

                                CielIconButton {
                                    icon: "x"
                                    iconSize: Theme.XSMALL
                                    onClicked: {
                                        slideAnim.to = 50;
                                        slideAnim.start();
                                        opacity = 0;
                                        closeTimer.start();
                                    }
                                }
                                Timer {
                                    id: closeTimer
                                    interval: 200
                                    onTriggered: root.tabCloseRequested(model.id)
                                }
                                Rectangle {
                                    opacity: (!delegateRoot.isCurrent && !delegateRoot.isNextCurrent) ? 1.0 : 0.0
                                    height: 18
                                    width: 1
                                    color: Theme.border
                                }
                            }
                        }
                    }
                }

                CielIconButton {
                    id: plusButton
                    icon: "plus"
                    iconSize: Theme.XSMALL
                    onClicked: root.tabAddRequested()

                    property real prevX: x
                    property bool isReady: false

                    transform: Translate {
                        id: plusTrans
                    }

                    CielSpring {
                        id: springAnim
                        target: plusTrans
                        property: "x"
                        to: 0
                        damping: 0.38
                        spring: 4.2
                        mass: 1.0
                        epsilon: 0.25
                    }

                    onXChanged: {
                        if (isReady && prevX > 0) {
                            springAnim.stop();
                            plusTrans.x = prevX - x;
                            springAnim.start();
                        }
                        prevX = x;
                    }

                    Component.onCompleted: {
                        prevX = x;
                        Qt.callLater(() => isReady = true);
                    }
                }

                Item {
                    Layout.fillWidth: true
                }
            }
        }
    }
}
