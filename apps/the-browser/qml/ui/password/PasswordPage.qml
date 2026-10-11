import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Ciel.Ui
import Ciel.Browser 1.0

Item {
    id: root

    property string searchQuery: ""
    property string errorText: ""

    ListModel {
        id: passwordModel
    }

    function reloadPasswords() {
        var items = PasswordManager.list(searchQuery);
        passwordModel.clear();
        for (var i = 0; i < items.length; ++i)
            passwordModel.append(items[i]);
    }

    Component.onCompleted: reloadPasswords()

    onVisibleChanged: {
        if (visible)
            reloadPasswords();
    }

    Connections {
        target: PasswordManager
        function onPasswordsChanged() {
            if (root.visible && !passwordList.busyClosing)
                root.reloadPasswords();
        }
        function onKeychainError(message) {
            root.errorText = "Keyring error: " + message;
        }
    }

    Connections {
        target: ProfileManager
        function onActiveProfileChanged() {
            root.reloadPasswords();
        }
    }

    Item {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: 32
        anchors.bottomMargin: 24
        width: Math.min(740, parent.width - 64)

        CielFocusWrapper {
            anchors.fill: parent

            ColumnLayout {
                anchors.fill: parent
                spacing: 18

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 16

                    Text {
                        text: "Passwords"
                        font.pixelSize: 22
                        font.weight: Font.DemiBold
                        color: Theme.textPrimary
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    CielSearch {
                        id: searchBar
                        Layout.preferredWidth: 240
                        placeholder: "Search passwords…"
                        onTextChanged: {
                            root.searchQuery = text;
                            root.errorText = "";
                            root.reloadPasswords();
                        }
                    }
                }

                Text {
                    visible: root.errorText !== ""
                    text: root.errorText
                    color: "#EF4444"
                    font.pixelSize: 12
                    wrapMode: Text.Wrap
                    Layout.fillWidth: true
                }

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Item {
                        anchors.centerIn: parent
                        visible: passwordModel.count === 0

                        ColumnLayout {
                            anchors.centerIn: parent
                            spacing: 8

                            CielIcon {
                                icon: "key"
                                size: Theme.MEDIUM
                                color: Theme.textSecondary
                                opacity: 0.35
                                Layout.alignment: Qt.AlignHCenter
                            }

                            Text {
                                text: root.searchQuery.length > 0
                                      ? "No matching passwords found"
                                      : "No saved passwords yet. Sign in to a site and choose Save."
                                font.pixelSize: 13
                                color: Theme.textSecondary
                                opacity: 0.8
                                Layout.alignment: Qt.AlignHCenter
                            }
                        }
                    }

                    CielScrollView {
                        anchors.fill: parent
                        visible: passwordModel.count > 0

                        ListView {
                            id: passwordList
                            anchors.fill: parent
                            spacing: 6
                            clip: true
                            model: passwordModel

                            property bool busyClosing: false

                            delegate: Item {
                                id: pwDelegate
                                width: passwordList.width
                                height: Math.max(0, 61 * closeDimension)

                                readonly property bool hovered: rowMouse.containsMouse

                                property string revealed: ""
                                readonly property bool isRevealed: revealed !== ""
                                property bool copied: false

                                property real closeYScale: 1.0
                                property real closeXScale: 1.0
                                property real closeOpacity: 1.0
                                property real closeDimension: 1.0
                                property bool isClosing: false

                                Connections {
                                    target: PasswordManager
                                    function onPasswordRevealed(entryId, password) {
                                        if (entryId === model.id) {
                                            pwDelegate.revealed = password;
                                            hideTimer.restart();
                                        }
                                    }
                                }

                                Timer {
                                    id: hideTimer
                                    interval: 10000
                                    onTriggered: pwDelegate.revealed = ""
                                }

                                Timer {
                                    id: copiedTimer
                                    interval: 1500
                                    onTriggered: pwDelegate.copied = false
                                }

                                function requestClose() {
                                    if (isClosing)
                                        return;
                                    isClosing = true;
                                    revealed = "";
                                    passwordList.busyClosing = true;
                                    closeAnimation.restart();
                                }

                                ParallelAnimation {
                                    id: closeAnimation

                                    NumberAnimation {
                                        target: pwDelegate
                                        property: "closeYScale"
                                        to: 0.0
                                        duration: 200
                                        easing.type: Easing.InQuad
                                    }
                                    NumberAnimation {
                                        target: pwDelegate
                                        property: "closeXScale"
                                        to: 0.2
                                        duration: 200
                                        easing.type: Easing.InQuad
                                    }
                                    SequentialAnimation {
                                        PauseAnimation { duration: 45 }
                                        NumberAnimation {
                                            target: pwDelegate
                                            property: "closeDimension"
                                            to: 0.0
                                            duration: 175
                                            easing.type: Easing.InOutQuad
                                        }
                                    }
                                    NumberAnimation {
                                        target: pwDelegate
                                        property: "closeOpacity"
                                        to: 0.0
                                        duration: 180
                                        easing.type: Easing.OutQuad
                                    }

                                    onFinished: {
                                        var targetId = model.id;
                                        passwordModel.remove(index);
                                        PasswordManager.remove(targetId);
                                        passwordList.busyClosing = false;
                                    }
                                }

                                Item {
                                    id: visualContent
                                    anchors.fill: parent
                                    opacity: pwDelegate.isClosing ? pwDelegate.closeOpacity : 1.0

                                    transform: Scale {
                                        origin.x: visualContent.width / 2
                                        origin.y: visualContent.height / 2
                                        xScale: pwDelegate.isClosing ? pwDelegate.closeXScale : 1.0
                                        yScale: pwDelegate.isClosing ? pwDelegate.closeYScale : 1.0
                                    }

                                    CielSquircle {
                                        anchors.fill: parent
                                        color: Theme.surface
                                        borderWidth: 1
                                        borderColor: Theme.border
                                    }

                                    RowLayout {
                                        anchors.fill: parent
                                        anchors.leftMargin: 14
                                        anchors.rightMargin: 10
                                        spacing: 12

                                        CielIcon {
                                            icon: "globe"
                                            size: Theme.SMALL
                                            color: pwDelegate.hovered ? Theme.accent : Theme.textSecondary
                                            opacity: pwDelegate.hovered ? 1.0 : 0.6
                                            Layout.alignment: Qt.AlignVCenter

                                            Behavior on color {
                                                ColorAnimation { duration: 120 }
                                            }
                                            Behavior on opacity {
                                                NumberAnimation { duration: 100 }
                                            }
                                        }

                                        ColumnLayout {
                                            Layout.fillWidth: true
                                            spacing: 2
                                            Layout.alignment: Qt.AlignVCenter

                                            Text {
                                                text: model.origin.replace(/^https?:\/\//, "")
                                                font.pixelSize: 13
                                                font.weight: Font.Medium
                                                color: Theme.textPrimary
                                                elide: Text.ElideRight
                                                Layout.fillWidth: true
                                            }

                                            Text {
                                                text: model.username !== "" ? model.username : "No username"
                                                font.pixelSize: 11
                                                color: Theme.textSecondary
                                                elide: Text.ElideRight
                                                Layout.fillWidth: true
                                            }
                                        }

                                        Text {
                                            text: pwDelegate.isRevealed ? pwDelegate.revealed : "••••••••"
                                            font.pixelSize: 12
                                            font.family: pwDelegate.isRevealed ? "monospace" : font.family
                                            color: Theme.textSecondary
                                            elide: Text.ElideRight
                                            Layout.preferredWidth: 130
                                            Layout.alignment: Qt.AlignVCenter
                                        }

                                        CielIconButton {
                                            icon: pwDelegate.isRevealed ? "eye-slash" : "eye"
                                            Layout.alignment: Qt.AlignVCenter
                                            onClicked: {
                                                if (pwDelegate.isRevealed)
                                                    pwDelegate.revealed = "";
                                                else
                                                    PasswordManager.reveal(model.id);
                                            }
                                        }

                                        CielIconButton {
                                            icon: pwDelegate.copied ? "check" : "copy"
                                            Layout.alignment: Qt.AlignVCenter
                                            onClicked: {
                                                PasswordManager.copyPassword(model.id);
                                                pwDelegate.copied = true;
                                                copiedTimer.restart();
                                            }
                                        }

                                        CielIconButton {
                                            icon: "trash"
                                            Layout.alignment: Qt.AlignVCenter
                                            onClicked: pwDelegate.requestClose()
                                        }
                                    }

                                    MouseArea {
                                        id: rowMouse
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        acceptedButtons: Qt.NoButton
                                        z: -1
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
