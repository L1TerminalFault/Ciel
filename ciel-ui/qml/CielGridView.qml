import QtQuick
import QtQuick.Controls as T
import Ciel.Ui

GridView {
    id: root

    property bool showScrollBar: true
    property color thumbColor: Theme.border

    clip: true

    CielScrollController {
        id: controller
        orientation: Qt.Vertical
    }

    T.ScrollBar.vertical: CielScrollBar {
        parent: root
        z: 100
        anchors.top: root.top
        anchors.bottom: root.bottom
        anchors.right: root.right
        anchors.rightMargin: 1
        policy: !root.showScrollBar ? T.ScrollBar.AlwaysOff : (root.contentHeight > root.height ? T.ScrollBar.AsNeeded : T.ScrollBar.AlwaysOff)
        thumbColor: root.thumbColor
    }
}
