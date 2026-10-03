import QtQuick
import QtQuick.Controls

FocusScope {
    id: root

    anchors.fill: parent

    default property alias content: mainLayout.data

    MouseArea {
        anchors.fill: parent

        propagateComposedEvents: true
        preventStealing: false

        onPressed: mouse => {
            if (Window.window && Window.window.activeFocusItem) {
                let currentFocus = Window.window.activeFocusItem;
                if (currentFocus && typeof currentFocus.deselect !== "undefined") {
                    currentFocus.deselect();
                    if (Window.window.contentItem) {
                        Window.window.contentItem.forceActiveFocus();
                    } else {
                        root.forceActiveFocus();
                    }
                    mouse.accepted = true;
                } else {
                    mouse.accepted = false;
                }
            } else {
                mouse.accepted = false;
            }
        }
    }

    Item {
        id: mainLayout
        anchors.fill: parent
    }
}
