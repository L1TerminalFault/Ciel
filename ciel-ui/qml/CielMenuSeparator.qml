import QtQuick
import Ciel.Ui 1.0

Item {
    id: root

    implicitWidth: parent ? parent.width : 160
    implicitHeight: 7
    width: parent ? parent.width : implicitWidth

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 6
        anchors.rightMargin: 6
        anchors.verticalCenter: parent.verticalCenter
        height: 1
        color: Theme.border
    }
}
