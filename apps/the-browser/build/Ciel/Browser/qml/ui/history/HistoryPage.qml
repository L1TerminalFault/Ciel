import QtQuick
import QtQuick.Layouts
import Ciel.Ui
import Ciel.Browser 1.0

Item {
    id: root

    signal openTabRequested(string url)

    property string searchQuery: ""

    ListModel {
        id: historyModel
    }

    function reloadHistory() {
        var items = ProfileManager.fetchHistory(searchQuery);
        historyModel.clear();
        for (var i = 0; i < items.length; ++i) {
            historyModel.append(items[i]);
        }
    }

    Component.onCompleted: reloadHistory()

    onVisibleChanged: {
        if (visible)
            reloadHistory();
    }

    Connections {
        target: ProfileManager
        function onVisitRecorded() {
            root.reloadHistory();
        }
        function onActiveProfileChanged() {
            root.reloadHistory();
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
                        text: "History"
                        font.pixelSize: 22
                        font.weight: Font.DemiBold
                        color: Theme.textPrimary
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    RowLayout {
                        spacing: 8

                        CielButton {
                            text: "Delete History"
                            onClicked: clearHistoryPopup.open()
                        }

                        CielSearch {
                            id: searchBar
                            Layout.preferredWidth: 240
                            placeholder: "Search history…"
                            onTextChanged: {
                                root.searchQuery = text;
                                root.reloadHistory();
                            }
                        }
                    }
                }

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Item {
                        anchors.centerIn: parent
                        visible: historyModel.count === 0

                        ColumnLayout {
                            anchors.centerIn: parent
                            spacing: 8

                            CielIcon {
                                icon: "clock-counter-clockwise"
                                size: Theme.MEDIUM
                                color: Theme.textSecondary
                                opacity: 0.35
                                Layout.alignment: Qt.AlignHCenter
                            }

                            Text {
                                text: root.searchQuery.length > 0 ? "No matching history found" : "No history recorded yet"
                                font.pixelSize: 13
                                color: Theme.textSecondary
                                opacity: 0.8
                                Layout.alignment: Qt.AlignHCenter
                            }
                        }
                    }

                    CielScrollView {
                        anchors.fill: parent
                        visible: historyModel.count > 0

                        ListView {
                            id: historyList
                            anchors.fill: parent
                            spacing: 6
                            clip: true
                            model: historyModel

                            signal itemCollisionImpulse(int sourceIndex)

                            delegate: Item {
                                id: historyDelegate
                                width: historyList.width
                                height: Math.max(0, 61 * closeDimension)
                                clip: false

                                readonly property bool hovered: rowMouse.containsMouse

                                property real closeY: 0.0
                                property real closeYScale: 1.0
                                property real closeXScale: 1.0
                                property real closeOpacity: 1.0
                                property real closeDimension: 1.0
                                property bool isClosing: false
                                property real shockwaveOffset: 0.0

                                Behavior on shockwaveOffset {
                                    CielSpring {
                                        damping: 0.28
                                        spring: 5.2
                                        mass: 1.0
                                        epsilon: 0.001
                                    }
                                }

                                Connections {
                                    target: historyList
                                    function onItemCollisionImpulse(sourceIndex) {
                                        if (historyDelegate.isClosing)
                                            return;
                                        var diff = sourceIndex - index;
                                        if (diff >= 1 && diff <= 3) {
                                            shockwaveTimer.delayMs = (diff - 1) * 28;
                                            shockwaveTimer.impulse = -13.5 * Math.pow(0.62, diff - 1);
                                            shockwaveTimer.restart();
                                        }
                                    }
                                }

                                SequentialAnimation {
                                    id: shockwaveTimer
                                    property int delayMs: 0
                                    property real impulse: 0.0

                                    PauseAnimation {
                                        duration: shockwaveTimer.delayMs
                                    }
                                    ScriptAction {
                                        script: historyDelegate.shockwaveOffset = shockwaveTimer.impulse
                                    }
                                    PauseAnimation {
                                        duration: 115
                                    }
                                    ScriptAction {
                                        script: historyDelegate.shockwaveOffset = 0.0
                                    }
                                }

                                function requestClose() {
                                    if (isClosing)
                                        return;
                                    isClosing = true;
                                    closeAnimation.restart();
                                }

                                ParallelAnimation {
                                    id: closeAnimation

                                    ScriptAction {
                                        script: historyList.itemCollisionImpulse(index)
                                    }

                                    NumberAnimation {
                                        target: historyDelegate
                                        property: "closeY"
                                        from: 0.0
                                        to: -28.0
                                        duration: 190
                                        easing.type: Easing.OutCubic
                                    }

                                    SequentialAnimation {
                                        NumberAnimation {
                                            target: historyDelegate
                                            property: "closeYScale"
                                            from: 1.0
                                            to: 0.32
                                            duration: 180
                                            easing.type: Easing.InQuad
                                        }
                                        NumberAnimation {
                                            target: historyDelegate
                                            property: "closeYScale"
                                            to: 0.0
                                            duration: 30
                                        }
                                    }

                                    SequentialAnimation {
                                        NumberAnimation {
                                            target: historyDelegate
                                            property: "closeXScale"
                                            from: 1.0
                                            to: 1.08
                                            duration: 60
                                            easing.type: Easing.OutQuad
                                        }
                                        NumberAnimation {
                                            target: historyDelegate
                                            property: "closeXScale"
                                            from: 1.08
                                            to: 0.20
                                            duration: 150
                                            easing.type: Easing.InQuad
                                        }
                                    }

                                    SequentialAnimation {
                                        PauseAnimation {
                                            duration: 45
                                        }
                                        NumberAnimation {
                                            target: historyDelegate
                                            property: "closeDimension"
                                            from: 1.0
                                            to: 0.0
                                            duration: 175
                                            easing.type: Easing.InOutQuad
                                        }
                                    }

                                    SequentialAnimation {
                                        PauseAnimation {
                                            duration: 60
                                        }
                                        NumberAnimation {
                                            target: historyDelegate
                                            property: "closeOpacity"
                                            from: 1.0
                                            to: 0.0
                                            duration: 145
                                            easing.type: Easing.OutQuad
                                        }
                                    }

                                    onFinished: {
                                        var targetId = model.id;
                                        historyModel.remove(index);
                                        ProfileManager.deleteHistoryItem(targetId);
                                    }
                                }

                                Item {
                                    id: visualContent
                                    anchors.fill: parent
                                    opacity: historyDelegate.isClosing ? historyDelegate.closeOpacity : 1.0

                                    transform: [
                                        Translate {
                                            y: historyDelegate.isClosing ? historyDelegate.closeY : historyDelegate.shockwaveOffset
                                        },
                                        Scale {
                                            origin.x: visualContent.width / 2
                                            origin.y: visualContent.height / 2
                                            xScale: historyDelegate.isClosing ? historyDelegate.closeXScale : 1.0
                                            yScale: historyDelegate.isClosing ? historyDelegate.closeYScale : 1.0
                                        }
                                    ]

                                    CielSquircle {
                                        id: rowBg
                                        anchors.fill: parent
                                        color: Theme.surface
                                        borderWidth: 1
                                        borderColor: Theme.border
                                    }

                                    RowLayout {
                                        anchors.fill: parent
                                        anchors.leftMargin: 14
                                        anchors.rightMargin: 14
                                        spacing: 12

                                        CielIcon {
                                            icon: "globe"
                                            size: Theme.SMALL
                                            color: historyDelegate.hovered ? Theme.accent : Theme.textSecondary
                                            opacity: historyDelegate.hovered ? 1.0 : 0.6
                                            Layout.alignment: Qt.AlignVCenter

                                            Behavior on color {
                                                ColorAnimation {
                                                    duration: 120
                                                }
                                            }

                                            Behavior on opacity {
                                                NumberAnimation {
                                                    duration: 100
                                                }
                                            }
                                        }

                                        ColumnLayout {
                                            Layout.fillWidth: true
                                            spacing: 2
                                            Layout.alignment: Qt.AlignVCenter

                                            Text {
                                                text: model.title || model.url
                                                font.pixelSize: 13
                                                font.weight: Font.Medium
                                                color: Theme.textPrimary
                                                elide: Text.ElideRight
                                                Layout.fillWidth: true
                                            }

                                            Text {
                                                text: model.url
                                                font.pixelSize: 11
                                                color: Theme.textSecondary
                                                elide: Text.ElideRight
                                                Layout.maximumWidth: Math.min(380, historyDelegate.width * 0.55)
                                            }
                                        }

                                        Text {
                                            text: {
                                                var d = new Date(model.timestamp * 1000);
                                                return d.toLocaleDateString(Qt.locale(), Locale.ShortFormat) + "  " + d.toLocaleTimeString(Qt.locale(), Locale.ShortFormat);
                                            }
                                            font.pixelSize: 11
                                            color: Theme.textSecondary
                                            opacity: 0.7
                                            Layout.alignment: Qt.AlignVCenter
                                        }

                                        CielIconButton {
                                            icon: "trash"
                                            Layout.alignment: Qt.AlignVCenter
                                            onClicked: historyDelegate.requestClose()
                                        }
                                    }

                                    MouseArea {
                                        id: rowMouse
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        cursorShape: Qt.PointingHandCursor
                                        z: -1
                                        onClicked: {
                                            if (!historyDelegate.isClosing) {
                                                root.openTabRequested(model.url);
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
    }

    CielPopup {
        id: clearHistoryPopup
        contentWidth: 420
        contentHeight: 350

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 24
            spacing: 16

            Text {
                text: "Clear Browsing Data"
                font.pixelSize: 17
                font.weight: Font.DemiBold
                color: Theme.textPrimary
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 12

                Text {
                    text: "When:"
                    font.pixelSize: 13
                    font.weight: Font.Medium
                    color: Theme.textPrimary
                    Layout.alignment: Qt.AlignVCenter
                }

                CielSelect {
                    id: rangeSelect
                    Layout.fillWidth: true
                    model: ["Last hour", "Today", "Last 7 days", "Everything"]
                    currentIndex: 0
                }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: Theme.border
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 14

                CielCheckBox {
                    id: checkHistory
                    Layout.fillWidth: true
                    text: "Browsing & download history"
                    description: "Clears visited pages and download history"
                    checked: true
                }

                CielCheckBox {
                    id: checkCookies
                    Layout.fillWidth: true
                    text: "Cookies and site data"
                    description: "Signs you out of most sites"
                    checked: false
                }

                CielCheckBox {
                    id: checkCache
                    Layout.fillWidth: true
                    text: "Cached images and files"
                    description: "Frees up temporary cache disk space"
                    checked: false
                }
            }

            Item {
                Layout.fillHeight: true
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                Item {
                    Layout.fillWidth: true
                }

                CielButton {
                    text: "Cancel"
                    onClicked: clearHistoryPopup.close()
                }

                CielButton {
                    text: "Clear"
                    destructive: true
                    onClicked: {
                        if (checkHistory.checked) {
                            ProfileManager.clearHistory(rangeSelect.currentIndex);
                        }
                        clearHistoryPopup.close();
                        root.reloadHistory();
                    }
                }
            }
        }
    }
}
