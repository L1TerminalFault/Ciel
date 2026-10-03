import QtQuick
import Ciel.Ui

Item {
    id: root

    property real value: 0.7
    property color color: Theme.textSecondary

    implicitWidth: 22
    implicitHeight: 22

    Item {
        id: sunBeams
        anchors.centerIn: parent
        width: 22
        height: 22
        rotation: root.value * 180

        Repeater {
            model: 8

            delegate: Item {
                anchors.centerIn: parent
                width: 22
                height: 22
                rotation: index * 45

                Rectangle {
                    anchors.horizontalCenter: parent.horizontalCenter
                    y: 1
                    width: 1.6
                    height: 2.8
                    radius: 0.8
                    color: root.color
                    opacity: 0.3 + (root.value * 0.7)
                }
            }
        }
    }

    Rectangle {
        id: coreCircle
        anchors.centerIn: parent
        width: Math.round(5.0 + (root.value * 6.0))
        height: width
        radius: width / 2
        color: root.color
    }
}
