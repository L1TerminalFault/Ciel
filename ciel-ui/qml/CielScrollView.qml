import QtQuick
import QtQuick.Controls as T
import Ciel.Ui

Flickable {
    id: root

    property alias orientation: controller.orientation
    property bool showScrollBar: true
    property real scrollBarGutter: 16
    property color thumbColor: Theme.border

    readonly property real availableWidth: (!root.showScrollBar || controller.orientation === Qt.Horizontal) ? width : Math.max(0, width - scrollBarGutter)
    readonly property real availableHeight: (!root.showScrollBar || controller.orientation !== Qt.Horizontal) ? height : Math.max(0, height - scrollBarGutter)

    clip: true

    flickableDirection: {
        if (controller.orientation === Qt.Horizontal)
            return Flickable.HorizontalFlick;
        if (controller.orientation === Qt.Vertical)
            return Flickable.VerticalFlick;
        return (root.contentWidth > root.width) ? Flickable.AutoFlickDirection : Flickable.VerticalFlick;
    }

    CielScrollController {
        id: controller
    }

    T.ScrollBar.vertical: CielScrollBar {
        parent: root
        z: 100
        anchors.top: root.top
        anchors.bottom: root.bottom
        anchors.right: root.right
        anchors.rightMargin: 1
        policy: (!root.showScrollBar || controller.orientation === Qt.Horizontal) ? T.ScrollBar.AlwaysOff : (root.contentHeight > root.height ? T.ScrollBar.AsNeeded : T.ScrollBar.AlwaysOff)
        thumbColor: root.thumbColor
    }

    T.ScrollBar.horizontal: CielScrollBar {
        parent: root
        z: 100
        anchors.left: root.left
        anchors.right: root.right
        anchors.bottom: root.bottom
        anchors.bottomMargin: 1
        policy: {
            if (!root.showScrollBar || controller.orientation === Qt.Vertical)
                return T.ScrollBar.AlwaysOff;
            if (controller.orientation === Qt.Horizontal)
                return root.contentWidth > root.width ? T.ScrollBar.AsNeeded : T.ScrollBar.AlwaysOff;
            return (root.contentWidth > root.width && root.contentHeight <= root.height) ? T.ScrollBar.AsNeeded : T.ScrollBar.AlwaysOff;
        }
        thumbColor: root.thumbColor
    }
}
