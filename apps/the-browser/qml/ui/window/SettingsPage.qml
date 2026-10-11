import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Ciel.Ui

Item {
    id: root

    property var settings: null
    property string searchQuery: ""

    function matches(title, detail) {
        if (!searchQuery)
            return true;
        var q = searchQuery.toLowerCase();
        return title.toLowerCase().indexOf(q) !== -1
            || detail.toLowerCase().indexOf(q) !== -1;
    }

    // component SettingSwitch: Item {
    //     id: sw
    //     property bool on: false
    //     implicitWidth: 44
    //     implicitHeight: 26
    //
    //     Rectangle {
    //         anchors.fill: parent
    //         radius: 13
    //         color: sw.on ? Theme.accent : Qt.alpha(Theme.textSecondary, 0.35)
    //         Behavior on color { ColorAnimation { duration: 140 } }
    //     }
    //
    //     Rectangle {
    //         width: 20
    //         height: 20
    //         radius: 10
    //         y: 3
    //         x: sw.on ? 21 : 3
    //         color: "#FFFFFF"
    //         Behavior on x {
    //             CielSpring { damping: 0.45; spring: 6.0; mass: 0.7; epsilon: 0.001 }
    //         }
    //     }
    // }

    component ToggleRow: Item {
        id: row
        property string title: ""
        property string detail: ""
        property bool on: false
        signal toggled
        readonly property bool shown: root.matches(title, detail)

        width: parent ? parent.width : 0
        height: shown ? 62 : 0
        visible: shown

        Rectangle {
            anchors.fill: parent
            color: rowMouse.containsMouse ? Qt.alpha(Theme.textPrimary, 0.04) : "transparent"
        }

        // Under the content, so clicks on the switch reach the switch itself.
        MouseArea {
            id: rowMouse
            anchors.fill: parent
            z: -1
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: row.toggled()
        }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 16
            anchors.rightMargin: 16
            spacing: 12

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 2

                Text {
                    text: row.title
                    font.pixelSize: 13
                    font.weight: Font.Medium
                    color: Theme.textPrimary
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                }

                Text {
                    text: row.detail
                    font.pixelSize: 11
                    color: Theme.textSecondary
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                }
            }

            CielSwitch {
                id: sw
                checked: row.on
                Layout.alignment: Qt.AlignVCenter
                onToggled: {
                    row.toggled()
                    // The switch flips itself on click, which breaks the binding.
                    // Restore it so the settings value stays the source of truth.
                    sw.checked = Qt.binding(function () { return row.on })
                }
            }
        }

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: row.shown ? 1 : 0
            color: Theme.border
        }
    }

    component ChoiceRow: Item {
        id: row
        property string title: ""
        property string detail: ""
        property var model: []        // [{ label, value }]
        property var value
        property bool numeric: false
        signal picked(var value)
        readonly property bool shown: root.matches(title, detail)

        width: parent ? parent.width : 0
        height: shown ? Math.max(62, textCol.implicitHeight + 24) : 0
        visible: shown

        function same(a, b) {
            if (row.numeric)
                return Math.abs(Number(a) - Number(b)) < 0.001;
            return a === b;
        }

        function indexOfValue() {
            for (var i = 0; i < row.model.length; ++i) {
                if (row.same(row.model[i].value, row.value))
                    return i;
            }
            return -1;
        }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 16
            anchors.rightMargin: 16
            spacing: 12

            ColumnLayout {
                id: textCol
                Layout.fillWidth: true
                spacing: 2

                Text {
                    text: row.title
                    font.pixelSize: 13
                    font.weight: Font.Medium
                    color: Theme.textPrimary
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                }

                Text {
                    text: row.detail
                    font.pixelSize: 11
                    color: Theme.textSecondary
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                }
            }

            CielSelect {
                id: select
                Layout.preferredWidth: 200
                Layout.alignment: Qt.AlignVCenter
                model: row.model.map(function (c) { return c.label })
                currentIndex: row.indexOfValue()

                onCurrentIndexChanged: {
                    if (currentIndex < 0 || currentIndex === row.indexOfValue())
                        return;               // programmatic sync, not a user pick
                    row.picked(row.model[currentIndex].value);
                    // Re-bind so the select follows the settings value again.
                    Qt.callLater(function () {
                        select.currentIndex = Qt.binding(function () { return row.indexOfValue() })
                    });
                }
            }
        }

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: row.shown ? 1 : 0
            color: Theme.border
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

                    ColumnLayout {
                        spacing: 2

                        Text {
                            text: "Settings"
                            font.pixelSize: 22
                            font.weight: Font.DemiBold
                            color: Theme.textPrimary
                        }

                        Text {
                            text: "Saved on this device"
                            font.pixelSize: 12
                            color: Theme.textSecondary
                        }
                    }

                    Item { Layout.fillWidth: true }

                    CielSearch {
                        id: searchBar
                        Layout.preferredWidth: 240
                        placeholder: "Search settings…"
                        onTextChanged: root.searchQuery = text
                    }
                }

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Item {
                        anchors.centerIn: parent
                        visible: searchQuery.length > 0 && !generalBlock.visible && !contentBlock.visible && !privacyBlock.visible

                        ColumnLayout {
                            anchors.centerIn: parent
                            spacing: 8

                            CielIcon {
                                icon: "faders-horizontal"
                                size: Theme.MEDIUM
                                color: Theme.textSecondary
                                opacity: 0.35
                                Layout.alignment: Qt.AlignHCenter
                            }

                            Text {
                                text: "No matching settings"
                                font.pixelSize: 13
                                color: Theme.textSecondary
                                Layout.alignment: Qt.AlignHCenter
                            }
                        }
                    }

                    CielScrollView {
                        anchors.fill: parent

                        Flickable {
                            anchors.fill: parent
                            contentWidth: width
                            contentHeight: settingsCol.implicitHeight
                            clip: true

                            ColumnLayout {
                                id: settingsCol
                                width: parent.width
                                spacing: 18

                                ColumnLayout {
                                    id: generalBlock
                                    Layout.fillWidth: true
                                    spacing: 8
                                    visible: engineRow.shown || homeRow.shown || zoomRow.shown
                                             || confirmRow.shown || suggestRow.shown || sidebarRow.shown

                                    Text {
                                        text: "General"
                                        font.pixelSize: 13
                                        // font.weight: Font.DemiBold
                                        color: Theme.textSecondary
                                        visible: generalBlock.visible
                                    }

                                    CielSquircle {
                                        Layout.fillWidth: true
                                        Layout.preferredHeight: generalCol.implicitHeight
                                        color: Theme.surface
                                        borderWidth: 1
                                        borderColor: Theme.border

                                        Column {
                                            id: generalCol
                                            width: parent.width

                                            ChoiceRow {
                                                id: engineRow
                                                title: "Search engine"
                                                detail: "Used for address-bar text that is not a URL."
                                                model: settings ? settings.engineChoices() : []
                                                value: settings ? settings.searchEngine : ""
                                                onPicked: value => { if (settings) settings.searchEngine = value }
                                            }

                                            // Item {
                                            //     id: homeRow
                                            //     readonly property string title: "Homepage"
                                            //     readonly property string detail: "Opened by a new tab. Leave empty for a blank page."
                                            //     readonly property bool shown: root.matches(title, detail)
                                            //     width: parent.width
                                            //     height: shown ? 74 : 0
                                            //     visible: shown
                                            //
                                            //     RowLayout {
                                            //         anchors.fill: parent
                                            //         anchors.leftMargin: 16
                                            //         anchors.rightMargin: 16
                                            //         spacing: 12
                                            //
                                            //         ColumnLayout {
                                            //             Layout.fillWidth: true
                                            //             spacing: 2
                                            //
                                            //             Text {
                                            //                 text: homeRow.title
                                            //                 font.pixelSize: 13
                                            //                 font.weight: Font.Medium
                                            //                 color: Theme.textPrimary
                                            //                 elide: Text.ElideRight
                                            //                 Layout.fillWidth: true
                                            //             }
                                            //
                                            //             Text {
                                            //                 text: homeRow.detail
                                            //                 font.pixelSize: 11
                                            //                 color: Theme.textSecondary
                                            //                 wrapMode: Text.WordWrap
                                            //                 Layout.fillWidth: true
                                            //             }
                                            //         }
                                            //
                                            //         CielSearch {
                                            //             id: homeField
                                            //             Layout.preferredWidth: 220
                                            //             Layout.preferredHeight: 32
                                            //             placeholder: "Blank page"
                                            //             showBorder: true
                                            //             preContent: Item {}
                                            //             postContent: Item {}
                                            //             Component.onCompleted: text = settings ? settings.homepage : ""
                                            //             onTextChanged: {
                                            //                 if (settings && settings.homepage !== text)
                                            //                     settings.homepage = text;
                                            //             }
                                            //         }
                                            //     }
                                            // }

                                            ChoiceRow {
                                                id: zoomRow
                                                title: "Default zoom"
                                                detail: "Page zoom for every tab. Changing it updates open pages."
                                                numeric: true
                                                model: settings ? settings.zoomChoices() : []
                                                value: settings ? settings.zoomFactor : 1
                                                onPicked: value => { if (settings) settings.zoomFactor = value }
                                            }

                                            ToggleRow {
                                                id: confirmRow
                                                title: "Confirm before closing"
                                                detail: "Ask when this window still has more than one tab."
                                                on: settings ? settings.confirmClose : false
                                                onToggled: { if (settings) settings.confirmClose = !settings.confirmClose }
                                            }

                                            // ToggleRow {
                                            //     id: suggestRow
                                            //     title: "History suggestions"
                                            //     detail: "Show recent history under the location bar."
                                            //     on: settings ? settings.showLocationSuggestions : true
                                            //     onToggled: { if (settings) settings.showLocationSuggestions = !settings.showLocationSuggestions }
                                            // }

                                            ToggleRow {
                                                id: sidebarRow
                                                title: "Collapsed sidebar"
                                                detail: "Keep the tab sidebar collapsed. Applies immediately."
                                                on: settings ? settings.sidebarCollapsed : false
                                                onToggled: { if (settings) settings.sidebarCollapsed = !settings.sidebarCollapsed }
                                            }
                                        }
                                    }
                                }

                                ColumnLayout {
                                    id: contentBlock
                                    Layout.fillWidth: true
                                    spacing: 8
                                    visible: jsRow.shown || imagesRow.shown || storageRow.shown
                                             || popupsRow.shown || pdfRow.shown || scrollRow.shown

                                    Text {
                                        text: "Content"
                                        font.pixelSize: 13
                                        // font.weight: Font.DemiBold
                                        color: Theme.textSecondary
                                        visible: contentBlock.visible
                                    }

                                    CielSquircle {
                                        Layout.fillWidth: true
                                        Layout.preferredHeight: contentCol.implicitHeight
                                        color: Theme.surface
                                        borderWidth: 1
                                        borderColor: Theme.border

                                        Column {
                                            id: contentCol
                                            width: parent.width

                                            ToggleRow {
                                                id: jsRow
                                                title: "JavaScript"
                                                detail: "Allow pages to run scripts."
                                                on: settings ? settings.javascriptEnabled : true
                                                onToggled: { if (settings) settings.javascriptEnabled = !settings.javascriptEnabled }
                                            }

                                            ToggleRow {
                                                id: imagesRow
                                                title: "Load images"
                                                detail: "Download images on web pages."
                                                on: settings ? settings.autoLoadImages : true
                                                onToggled: { if (settings) settings.autoLoadImages = !settings.autoLoadImages }
                                            }

                                            ToggleRow {
                                                id: storageRow
                                                title: "Local storage"
                                                detail: "Let sites store data in local storage."
                                                on: settings ? settings.localStorageEnabled : true
                                                onToggled: { if (settings) settings.localStorageEnabled = !settings.localStorageEnabled }
                                            }

                                            ToggleRow {
                                                id: popupsRow
                                                title: "Scripts may open windows"
                                                detail: "Allow JavaScript to open tabs and windows."
                                                on: settings ? settings.javascriptCanOpenWindows : true
                                                onToggled: { if (settings) settings.javascriptCanOpenWindows = !settings.javascriptCanOpenWindows }
                                            }

                                            ToggleRow {
                                                id: pdfRow
                                                title: "PDF viewer"
                                                detail: "Open PDF files in the browser."
                                                on: settings ? settings.pdfViewerEnabled : true
                                                onToggled: { if (settings) settings.pdfViewerEnabled = !settings.pdfViewerEnabled }
                                            }

                                            ToggleRow {
                                                id: scrollRow
                                                title: "Smooth scrolling"
                                                detail: "Animate scrolling inside web pages."
                                                on: settings ? settings.smoothScrolling : true
                                                onToggled: { if (settings) settings.smoothScrolling = !settings.smoothScrolling }
                                            }
                                        }
                                    }
                                }

                                ColumnLayout {
                                    id: privacyBlock
                                    Layout.fillWidth: true
                                    spacing: 8
                                    visible: auditRow.shown || spellRow.shown || uaRow.shown

                                    Text {
                                        text: "Privacy"
                                        font.pixelSize: 13
                                        // font.weight: Font.DemiBold
                                        color: Theme.textSecondary
                                        visible: privacyBlock.visible
                                    }

                                    CielSquircle {
                                        Layout.fillWidth: true
                                        Layout.preferredHeight: privacyCol.implicitHeight
                                        color: Theme.surface
                                        borderWidth: 1
                                        borderColor: Theme.border

                                        Column {
                                            id: privacyCol
                                            width: parent.width

                                            ToggleRow {
                                                id: auditRow
                                                title: "Hyperlink auditing"
                                                detail: "Let sites know which links you follow."
                                                on: settings ? settings.hyperlinkAuditingEnabled : false
                                                onToggled: { if (settings) settings.hyperlinkAuditingEnabled = !settings.hyperlinkAuditingEnabled }
                                            }

                                            ToggleRow {
                                                id: spellRow
                                                title: "Spell checking"
                                                detail: "Underline misspellings in text fields."
                                                on: settings ? settings.spellCheckEnabled : true
                                                onToggled: { if (settings) settings.spellCheckEnabled = !settings.spellCheckEnabled }
                                            }

                                            ChoiceRow {
                                                id: uaRow
                                                title: "User agent"
                                                detail: "Sent with requests. The open page reloads when this changes."
                                                model: settings ? settings.userAgentChoices() : []
                                                value: settings ? settings.userAgentPreset : "default"
                                                onPicked: value => { if (settings) settings.userAgentPreset = value }
                                            }
                                        }
                                    }
                                }

                                Item {
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: searchQuery === "" ? 36 : 0
                                    visible: searchQuery === ""

                                    CielButton {
                                        anchors.left: parent.left
                                        text: "Restore defaults"
                                        onClicked: if (settings) settings.resetDefaults()
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Connections {
    //     target: settings
    //     function onHomepageChanged() {
    //         if (homeField.text !== settings.homepage)
    //             homeField.text = settings.homepage;
    //     }
    // }
}
