import QtQuick
import Ciel.Ui
import Ciel.Browser 1.0

CielDropDown {
    id: root

    property BrowserConfig config: null
    property WorkspaceModel workspaceModel: null

    signal newTabRequested
    signal newWindowRequested
    signal newPrivateWindowRequested
    signal restoreTabRequested
    signal historyRequested
    signal downloadsRequested
    signal passwordsRequested
    signal bookmarksRequested
    signal printRequested
    signal savePageRequested
    signal translateRequested
    signal findInPageRequested
    signal settingsRequested

    CielMenuItem {
        text: "New Tab"
        icon: "plus"
        shortcut: "Ctrl+T"
        onTriggered: root.newTabRequested()
    }

    CielMenuItem {
        text: "New Window"
        icon: "browsers"
        shortcut: "Ctrl+N"
        onTriggered: root.newWindowRequested()
    }

    CielMenuItem {
        text: "New Private Window"
        icon: "detective"
        shortcut: "Ctrl+Shift+N"
        onTriggered: root.newPrivateWindowRequested()
    }

    CielMenuItem {
        text: "Restore Closed Tab"
        icon: "arrow-counter-clockwise"
        shortcut: "Ctrl+Shift+T"
        onTriggered: root.restoreTabRequested()
    }

    CielMenuSeparator {}

    CielMenuItem {
        text: "History"
        icon: "clock-counter-clockwise"
        shortcut: "Ctrl+H"
        onTriggered: root.historyRequested()
    }

    CielMenuItem {
        text: "Downloads"
        icon: "download-simple"
        shortcut: "Ctrl+J"
        onTriggered: root.downloadsRequested()
    }

    CielMenuItem {
        text: "Passwords"
        icon: "key"
        onTriggered: root.passwordsRequested()
    }

    CielMenuItem {
        text: "Bookmarks"
        icon: "bookmark-simple"
        shortcut: "Ctrl+Shift+O"
        onTriggered: root.bookmarksRequested()
    }

    CielMenuSeparator {}

    CielMenuItem {
        text: "Print…"
        icon: "printer"
        shortcut: "Ctrl+P"
        onTriggered: root.printRequested()
    }

    CielMenuItem {
        text: "Save Page As…"
        icon: "floppy-disk"
        shortcut: "Ctrl+S"
        onTriggered: root.savePageRequested()
    }

    CielMenuItem {
        text: "Translate Page…"
        icon: "translate"
        onTriggered: root.translateRequested()
    }

    CielMenuItem {
        text: "Find in Page"
        icon: "magnifying-glass"
        shortcut: "Ctrl+F"
        onTriggered: root.findInPageRequested()
    }

    CielMenuSeparator {}

    CielMenuItem {
        text: "Settings"
        icon: "gear-six"
        shortcut: "Ctrl+,"
        onTriggered: root.settingsRequested()
    }

    CielMenuItem {
        text: "Quit"
        icon: "power"
        shortcut: "Ctrl+Q"
        destructive: true
        onTriggered: Qt.quit()
    }
}
