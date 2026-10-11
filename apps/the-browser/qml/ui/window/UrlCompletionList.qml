import QtQuick
import QtQuick.Layouts
import Ciel.Ui
import Ciel.Browser 1.0

// History completion list. Drop under a CielSearch; do not fold this into CielSearch.
Item {
    id: root

    property string query: ""
    property int maxItems: 40            // total rows kept; the list scrolls
    property int maxBookmarksWhenEmpty: 5
    property int rowHeight: 44
    property int maxVisibleHeight: 280   // list never grows past this
    property int selectedIndex: -1

    readonly property int count: suggestionModel.count

    signal activated(string url)
    readonly property real headerHeight: headerLabel.implicitHeight + 2
    readonly property real targetHeight: count > 0
        ? headerHeight + Math.min(list.contentHeight, maxVisibleHeight) : 0
    property real shownHeight: targetHeight
    Behavior on shownHeight {
        CielSpring { damping: 0.5; spring: 5.0; mass: 1.0; epsilon: 0.01 }
    }

    // Clamped so the spring can never overshoot the cap.
    implicitHeight: Math.max(0, Math.min(shownHeight, headerHeight + maxVisibleHeight))
    clip: true
    // implicitHeight: count > 0 ? (headerLabel.implicitHeight + 2 + list.height) : 0
    //
    // // implicitHeight: listCol.implicitHeight

    function reload() {
        suggestionModel.clear()
        var seen = ({})

        function add(entry, kind) {
            var u = entry && entry.url ? ("" + entry.url) : ""
            if (u === "" || seen[u] || suggestionModel.count >= maxItems)
                return false
            seen[u] = true
            // Only uniform fields: ListModel rejects a role whose type differs between rows.
            suggestionModel.append({
                title: entry.title ? ("" + entry.title) : "",
                url: u,
                kind: kind
            })
            return true
        }

        // Bookmarks first.
        var marks = ProfileManager.fetchBookmarks(query) || []
        var bookmarkCap = query === "" ? maxBookmarksWhenEmpty : maxItems
        var added = 0
        for (var i = 0; i < marks.length && added < bookmarkCap; ++i) {
            if (add(marks[i], "bookmark"))
                added++
        }

        // Then history.
        var items = ProfileManager.fetchHistory(query) || []
        for (var j = 0; j < items.length && suggestionModel.count < maxItems; ++j)
            add(items[j], "history")

        if (selectedIndex >= suggestionModel.count)
            selectedIndex = suggestionModel.count - 1

        list.positionViewAtBeginning()
    }

    function move(delta) {
        if (count === 0) {
            selectedIndex = -1
            return
        }
        var next = selectedIndex + delta
        if (next < -1)
            next = -1
        if (next > count - 1)
            next = count - 1
        selectedIndex = next
        if (selectedIndex >= 0)
            list.positionViewAtIndex(selectedIndex, ListView.Contain)
        else
            list.positionViewAtBeginning()
    }

    function activateCurrent() {
        if (selectedIndex < 0 || selectedIndex >= suggestionModel.count)
            return
        var row = suggestionModel.get(selectedIndex)
        if (row && row.url)
            activated("" + row.url)
    }

    onQueryChanged: reload()

    ListModel {
        id: suggestionModel
    }

    Text {
        id: headerLabel
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        leftPadding: 8
        bottomPadding: 2
        text: root.query === "" ? "Recent" : "Suggestions"
        font.pixelSize: 11
        font.weight: Font.Medium
        color: Theme.textSecondary
        visible: root.count > 0
    }

    ListView {
        id: list
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: headerLabel.bottom
        anchors.topMargin: 2
        height: Math.max(0, root.implicitHeight - root.headerHeight)
        clip: true
        spacing: 2
        boundsBehavior: Flickable.StopAtBounds
        model: suggestionModel

        delegate: Item {
            id: suggestionRow
            width: list.width
            height: root.rowHeight

            readonly property bool isSelected: root.selectedIndex === index
            readonly property bool isBookmark: model.kind === "bookmark"

            Rectangle {
                anchors.fill: parent
                radius: 8
                color: suggestionRow.isSelected
                      ? Qt.alpha(Theme.accent, 0.16)
                      : (rowMouse.containsMouse ? Qt.alpha(Theme.textPrimary, 0.05) : "transparent")

                Behavior on color {
                    ColorAnimation { duration: 120 }
                }
            }

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 8
                anchors.rightMargin: 8
                spacing: 8

                CielIcon {
                    icon: suggestionRow.isBookmark ? "bookmark-simple" : "clock-counter-clockwise"
                    size: Theme.XSMALL
                    color: (suggestionRow.isSelected || suggestionRow.isBookmark)
                           ? Theme.accent : Theme.textSecondary
                    Layout.alignment: Qt.AlignVCenter
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 0

                    Text {
                        text: model.title !== "" ? model.title : model.url
                        font.pixelSize: 13
                        font.weight: Font.Medium
                        color: Theme.textPrimary
                        elide: Text.ElideRight
                        Layout.fillWidth: true
                    }

                    Text {
                        text: model.url
                        visible: model.title !== "" && model.title !== model.url
                        font.pixelSize: 11
                        color: Theme.textSecondary
                        elide: Text.ElideRight
                        Layout.fillWidth: true
                    }
                }
            }

            MouseArea {
                id: rowMouse
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                // Real pointer movement only. With containsMouseChanged, wheel-scrolling
                // would move rows under a still cursor and steal the selection.
                onPositionChanged: root.selectedIndex = index
                onClicked: root.activated("" + model.url)
            }
        }
    }
}
