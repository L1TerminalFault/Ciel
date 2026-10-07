import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import QtWebEngine
import Ciel.Ui
import Ciel.Browser 1.0
import QtQuick.Dialogs

Item {
    id: root

    required property BrowserConfig config
    required property WebEngineView activeView

    property real entranceProgress: 0.0

    Component.onCompleted: {
        entranceProgress = 1.0;
    }

    Behavior on entranceProgress {
        CielSpring {
            damping: 0.32
            spring: 5.0
            mass: 1.0
            epsilon: 0.002
        }
    }

    Item {
        id: centerCard
        anchors.centerIn: parent
        width: parent.width - 48 // Math.min(580, parent.width - 48)
        height: profileRow.contentHeight + 32
        scale: 0.94 + (root.entranceProgress * 0.06)
        opacity: Math.min(1.0, root.entranceProgress * 1.5)
        y: (1.0 - root.entranceProgress) * 24.0

        property bool editMode: false

        ListModel { id: profileModel }

        function indexOfProfile(pid) {
            for (var i = 0; i < profileModel.count; ++i) {
                if (profileModel.get(i).profileId === pid)
                    return i
            }
            return -1
        }

        function syncProfiles() {
            var fresh = ProfileManager.listProfiles()
            var incoming = ({})
            for (var i = 0; i < fresh.length; ++i)
                incoming[fresh[i].id] = fresh[i]

            // --- deleted ---
            for (var i = profileModel.count - 1; i >= 0; --i) {
                var pid = profileModel.get(i).profileId
                if (!incoming[pid]) {
                    var r = Math.floor(i / profileRow.maxColumns)
                    var c = i % profileRow.maxColumns
                    profileModel.remove(i)
                    profileRow.rippleFrom(r, c)
                }
            }

            // --- added / updated ---
            for (var i = 0; i < fresh.length; ++i) {
                var p = fresh[i]
                var img = p.profileImage || ""
                var col = p.color || ""
                var active = !!p.isActive
                var idx = indexOfProfile(p.id)

                if (idx === -1) {
                    profileModel.append({
                        profileId: p.id,
                        displayName: p.displayName,
                        profileImage: img,
                        color: col,
                        isActive: active
                    })
                    continue
                }

                var row = profileModel.get(idx)
                var visualChanged = row.displayName !== p.displayName
                                || row.profileImage !== img
                                || row.color !== col
                var becameActive = !row.isActive && active

                if (row.displayName !== p.displayName)
                    profileModel.setProperty(idx, "displayName", p.displayName)
                if (row.profileImage !== img)
                    profileModel.setProperty(idx, "profileImage", img)
                if (row.color !== col)
                    profileModel.setProperty(idx, "color", col)
                if (row.isActive !== active)
                    profileModel.setProperty(idx, "isActive", active)

                // no entry animation — just a pulse + ripple on the changed cell
                if (visualChanged || becameActive) {
                    var item = profileRepeater.itemAt(idx)
                    if (item && item.playPulse)
                        item.playPulse()
                    if (item)
                        profileRow.rippleFrom(item.rowIndex, item.colIndex)
                }
            }
        }

        Component.onCompleted: syncProfiles()

        Connections {
            target: ProfileManager
            function onActiveProfileChanged() { centerCard.syncProfiles() }
            function onProfilesChanged() { centerCard.syncProfiles() }
        }

        Item {
            id: profileRow
            anchors.centerIn: parent
            width: parent.width
            height: contentHeight

            readonly property real itemWidth: 100
            readonly property real itemHeight: 95
            readonly property real itemSpacing: 24
            // readonly property int totalCount: profileModel.count + 1
            readonly property int totalCount: profileModel.count + (centerCard.editMode ? 1 : 0)
            readonly property int maxColumns: Math.max(1, Math.floor((width + itemSpacing) / (itemWidth + itemSpacing)))
            readonly property int totalRows: Math.ceil(totalCount / maxColumns)
            readonly property real contentHeight: (totalRows * itemHeight) + (Math.max(0, totalRows - 1) * itemSpacing)
            readonly property int entryStaggerMs: 100
            readonly property real entrySlide: 18
            readonly property real entryScale: 1.1

            property int rippleGen: 0
            property bool introPlayed: false
            property int releaseRow: 0
            property int releaseCol: 0
            property int releaseGen: 0

            function setRipple(senderRow, senderCol, on) {
                function apply(item) {
                    if (!item || !item.entryTriggered)
                        return
                    if (item.rowIndex === senderRow && item.colIndex === senderCol)
                        return
                    var dr = item.rowIndex - senderRow
                    var dc = item.colIndex - senderCol
                    var dist = Math.sqrt(dr * dr + dc * dc)
                    if (dist < 0.01)
                        return
                    if (on) {
                        var mag = 6.5 / (dist * dist + 0.55)
                        item.localBounceOffsetX = (dc / dist) * mag
                        item.localBounceOffsetY = (dr / dist) * mag
                    } else {
                        item.localBounceOffsetX = 0
                        item.localBounceOffsetY = 0
                    }
                }
                for (var i = 0; i < profileRepeater.count; ++i)
                    apply(profileRepeater.itemAt(i))
                apply(addProfileDelegate)
            }

            function rippleFrom(row, col) {
                rippleGen++
                setRipple(row, col, true)
                releaseRow = row
                releaseCol = col
                releaseGen = rippleGen
                rippleRelease.restart()
            }

            Timer {
                id: rippleRelease
                interval: 180
                onTriggered: {
                    if (profileRow.releaseGen === profileRow.rippleGen)
                        profileRow.setRipple(profileRow.releaseRow, profileRow.releaseCol, false)
                }
            }

            Timer {
                id: introGate
                interval: 200
                onTriggered: profileRow.introPlayed = true
            }

            Repeater {
                id: profileRepeater
                model: profileModel

                delegate: ColumnLayout {
                    id: profileDelegate
                    spacing: 8
                    width: profileRow.itemWidth
                    height: profileRow.itemHeight

                    readonly property int rowIndex: Math.floor(index / profileRow.maxColumns)
                    readonly property int colIndex: index % profileRow.maxColumns
                    readonly property int itemsInThisRow: (rowIndex === profileRow.totalRows - 1)
                        ? (profileRow.totalCount - (rowIndex * profileRow.maxColumns))
                        : profileRow.maxColumns
                    readonly property real rowWidth: (itemsInThisRow * profileRow.itemWidth)
                        + ((itemsInThisRow - 1) * profileRow.itemSpacing)
                    readonly property real startX: (profileRow.width - rowWidth) / 2
                    readonly property real targetX: startX + (colIndex * (profileRow.itemWidth + profileRow.itemSpacing))
                    readonly property real baseTargetY: rowIndex * (profileRow.itemHeight + profileRow.itemSpacing)

                    property bool entryTriggered: false
                    property bool hasSettled: false
                    property int myRippleGen: -1
                    property real localBounceOffsetY: 0
                    property real localBounceOffsetX: 0
                    property real avatarScale: 1

                    function playPulse() {
                        avatarScale = 1.04
                        pulseReset.restart()
                    }

                    Timer {
                        id: pulseReset
                        interval: 80
                        onTriggered: profileDelegate.avatarScale = 1
                    }

                    Timer {
                        running: true
                        repeat: false
                        interval: profileRow.introPlayed ? 0 : (index * profileRow.entryStaggerMs)
                        onTriggered: {
                            profileDelegate.entryTriggered = true
                            profileRow.rippleGen++
                            profileDelegate.myRippleGen = profileRow.rippleGen
                            profileRow.setRipple(profileDelegate.rowIndex, profileDelegate.colIndex, true)
                            if (index === profileModel.count - 1)
                                introGate.restart()
                        }
                    }

                    x: targetX
                    y: entryTriggered ? baseTargetY : (baseTargetY + profileRow.entrySlide)
                    opacity: entryTriggered ? 1 : 0
                    scale: entryTriggered ? 1 : profileRow.entryScale
                    transformOrigin: Item.Center

                    transform: Translate {
                        x: profileDelegate.localBounceOffsetX
                        y: profileDelegate.localBounceOffsetY
                    }

                    Behavior on localBounceOffsetX {
                        CielSpring { damping: 0.68; spring: 18.0; mass: 2.2; epsilon: 0.002 }
                    }
                    Behavior on localBounceOffsetY {
                        CielSpring { damping: 0.68; spring: 18.0; mass: 2.2; epsilon: 0.002 }
                    }
                    Behavior on avatarScale {
                        CielSpring { damping: 0.52; spring: 18.0; mass: 2.2; epsilon: 0.002 }
                    }
                    Behavior on x {
                        enabled: profileDelegate.entryTriggered
                        CielSpring { damping: 0.85; spring: 16.4; mass: 4.0; epsilon: 0.002 }
                    }
                    Behavior on y {
                        CielSpring {
                            damping: 0.85
                            spring: 16.4
                            mass: 4.0
                            epsilon: 0.002
                            onRunningChanged: {
                                if (running || !profileDelegate.entryTriggered || profileDelegate.hasSettled)
                                    return
                                profileDelegate.hasSettled = true
                                if (profileDelegate.myRippleGen === profileRow.rippleGen)
                                    profileRow.setRipple(profileDelegate.rowIndex, profileDelegate.colIndex, false)
                            }
                        }
                    }
                    Behavior on opacity {
                        CielSpring { damping: 0.85; spring: 8.4; mass: 4.0; epsilon: 0.002 }
                    }
                    Behavior on scale {
                        CielSpring { damping: 0.85; spring: 14.4; mass: 4.0; epsilon: 0.002 }

                    }
                    Rectangle {
                        id: avatarContainer
                        Layout.alignment: Qt.AlignHCenter
                        width: 82
                        height: 82
                        radius: width / 2
                        color: model.color ? model.color : Theme.surface
                        border.width: model.isActive ? 2 : 0
                        border.color: Theme.accent
                        scale: profileDelegate.avatarScale

                        Image {
                            id: delegateImg
                            anchors.fill: parent
                            source: model.profileImage
                            fillMode: Image.PreserveAspectCrop
                            visible: false   
                        }

                        Rectangle {
                            id: delegateMaskSource
                            anchors.fill: parent
                            anchors.margins: avatarContainer.border.width 
                            radius: width / 2
                            visible: false
                            layer.enabled: true   
                        }

                        MultiEffect {
                            anchors.fill: delegateMaskSource
                            source: delegateImg
                            maskEnabled: true
                            maskSource: delegateMaskSource
                            visible: model.profileImage !== ""
                        }

                        Text {
                            anchors.centerIn: parent
                            text: model.displayName.charAt(0).toUpperCase()
                            visible: model.profileImage === ""
                            font.bold: true
                            font.pixelSize: 18
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                              if (!centerCard.editMode) {
                                  if (!model.isActive) {
                                    ProfileManager.switchProfile(model.profileId);
                                  }
                                } else {
                                  if (model.displayName !== "Default") {
                                    // editProfileDialog.openForProfile(model.profileId);
                                    profileDialog.openForProfile(model.profileId)
                                  }
                                }
                            }
                        }

// Item {
//     id: profileEditButton
//     width: 28
//     height: 28
//     visible: model.displayName !== "Default"
//     anchors.left: parent.left
//     anchors.bottom: parent.bottom
//     anchors.leftMargin: -4
//     anchors.bottomMargin: -5
//     z: 10
//
//     Rectangle {
//         anchors.fill: parent
//         radius: width / 2
//         color: Theme.surface
//         border.width: 1
//         border.color: Theme.border
//
//         Item {
//             anchors.fill: parent
//             anchors.margins: 6
//         CielIcon {
//             anchors.centerIn: parent
//             icon: "edit"
//             size: Theme.SMALL
//             color: Theme.textPrimary
//         }
//         }
//
//         MouseArea {
//             anchors.fill: parent
//             enabled: centerCard.editMode
//             onClicked: editProfileDialog.openForProfile(model.profileId)
//         }
//     }
//
//     opacity: centerCard.editMode ? 1 : 0
//     scale: centerCard.editMode ? 1 : 0
//     transformOrigin: Item.Center
//
//     Behavior on opacity {
//         CielSpring {
//             damping: 0.85
//             spring: 12.0
//             mass: 2.2
//             epsilon: 0.002
//         }
//     }
//
//     Behavior on scale {
//         CielSpring {
//             damping: 0.65
//             spring: 14.0
//             mass: 2.2
//             epsilon: 0.002
//         }
//     }
// }

Item {
    id: profileDeleteButton
    width: 28
    height: 28
    visible: model.displayName !== "Default"
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    anchors.rightMargin: -4
    anchors.bottomMargin: -5
    z: 10

    Rectangle {
        anchors.fill: parent
        radius: width / 2
        color: Theme.surface
        border.width: 1
        border.color: Theme.border

        Item {
            anchors.fill: parent
            anchors.margins: 6
        CielIcon {
            anchors.centerIn: parent
            icon: "trash"
            size: Theme.SMALL
            color: "#ff2589" // Theme.textPrimary
        }
      }

        MouseArea {
            anchors.fill: parent
            enabled: centerCard.editMode
            onClicked: ProfileManager.deleteProfile(model.profileId)
        }
    }

    opacity: centerCard.editMode ? 1 : 0
    scale: centerCard.editMode ? 1 : 0
    transformOrigin: Item.Center

    Behavior on opacity {
        CielSpring {
            damping: 0.85
            spring: 12.0
            mass: 2.2
            epsilon: 0.002
        }
    }

    Behavior on scale {
        CielSpring {
            damping: 0.65
            spring: 14.0
            mass: 2.2
            epsilon: 0.002
        }
    }
}
                    }
                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        Layout.maximumWidth: 80
                        text: model.displayName
                        horizontalAlignment: Text.AlignHCenter
                        elide: Text.ElideRight
                        
                        color: Theme.textPrimary // Keeps your original theme color mapping intact
                        font.pixelSize: 13 
                    }
                }
            }

            ColumnLayout {
                id: addProfileDelegate
                spacing: 6
                width: profileRow.itemWidth
                height: profileRow.itemHeight

                readonly property int finalIndex: profileRow.totalCount - 1
                readonly property int rowIndex: Math.floor(finalIndex / profileRow.maxColumns)
                readonly property int colIndex: finalIndex % profileRow.maxColumns
                readonly property int itemsInThisRow: profileRow.totalCount - (rowIndex * profileRow.maxColumns)
                readonly property real rowWidth: (itemsInThisRow * profileRow.itemWidth)
                    + ((itemsInThisRow - 1) * profileRow.itemSpacing)
                readonly property real startX: (profileRow.width - rowWidth) / 2
                readonly property real targetX: startX + (colIndex * (profileRow.itemWidth + profileRow.itemSpacing))
                readonly property real baseTargetY: rowIndex * (profileRow.itemHeight + profileRow.itemSpacing)

                property bool entryTriggered: false
                property bool hasSettled: false
                property int myRippleGen: -1
                property real localBounceOffsetY: 0
                property real localBounceOffsetX: 0

                Timer {
                    running: true
                    repeat: false
                    interval: profileRow.introPlayed ? 0 : (addProfileDelegate.finalIndex * profileRow.entryStaggerMs)
                    onTriggered: {
                        addProfileDelegate.entryTriggered = true
                        profileRow.rippleGen++
                        addProfileDelegate.myRippleGen = profileRow.rippleGen
                        profileRow.setRipple(addProfileDelegate.rowIndex, addProfileDelegate.colIndex, true)
                    }
                }

                x: targetX
                y: entryTriggered ? baseTargetY : (baseTargetY + profileRow.entrySlide)
                // opacity: entryTriggered ? 1 : 0

opacity: (entryTriggered && centerCard.editMode) ? 1 : 0
scale: (entryTriggered && centerCard.editMode) ? 1 : 0
transformOrigin: Item.Center

Behavior on scale {
    CielSpring {
        damping: 0.85
        spring: 10.0
        mass: 2.2
        epsilon: 0.002
    }
}

                transform: Translate {
                    x: addProfileDelegate.localBounceOffsetX
                    y: addProfileDelegate.localBounceOffsetY
                }

                Behavior on localBounceOffsetX {
                    CielSpring { damping: 1.5; spring: 8.4; mass: 1.8; epsilon: 0.002 }
                }
                Behavior on localBounceOffsetY {
                    CielSpring { damping: 1.5; spring: 8.4; mass: 1.8; epsilon: 0.002 }
                }
                Behavior on x {
                    enabled: addProfileDelegate.entryTriggered
                    CielSpring { damping: 2.5; spring: 8.4; mass: 1.8; epsilon: 0.002 }
                }
                Behavior on y {
                    CielSpring {
                        damping: 2.5
                        spring: 8.4
                        mass: 1.8
                        epsilon: 0.002
                        onRunningChanged: {
                            if (running || !addProfileDelegate.entryTriggered || addProfileDelegate.hasSettled)
                                return
                            addProfileDelegate.hasSettled = true
                            if (addProfileDelegate.myRippleGen === profileRow.rippleGen)
                                profileRow.setRipple(addProfileDelegate.rowIndex, addProfileDelegate.colIndex, false)
                        }
                    }
                }
                Behavior on opacity {
                    CielSpring { damping: 0.85; spring: 8.4; mass: 4.0; epsilon: 0.002 }
                }

                Rectangle {
                    Layout.alignment: Qt.AlignHCenter
                    width: 82
                    height: 82
                    radius: width / 2
                    color: Theme.surface
                    CielIcon {
                        anchors.centerIn: parent
                        icon: "plus"
                        size: Theme.MEDIUM
                    }
                    MouseArea {
                        anchors.fill: parent
                        enabled: centerCard.editMode
                        onClicked: profileDialog.openForCreate() // addProfileDialog.open()
                    }
                }
                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Add profile"
                    horizontalAlignment: Text.AlignHCenter
                    color: Theme.textPrimary
                }
            }
        }

    }

Item {
    id: editModeButton
    // 1. Let the container height stay 46, but dynamically calculate width based on the text container
    width: 46 + editTextContainer.width
    height: 46
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    anchors.rightMargin: 24
    anchors.bottomMargin: 24
    z: 20

    // Smoothly spring animate the parent width change
    Behavior on width {
        CielSpring {
            damping: 0.62
            spring: 10.0
            mass: 2.8
            epsilon: 0.002
        }
    }

    Rectangle {
        anchors.fill: parent
        // 2. Fix the radius to half of the height (23) so the capsule ends remain perfectly circular
        radius: 23 
        color: Theme.surface
        border.width: 1
        border.color: Theme.border
        clip: true

        Item {
            anchors.fill: parent

            CielIcon {
                anchors.centerIn: parent
                icon: "pencil-simple"
                size: Theme.MEDIUM
                color: Theme.textPrimary

                opacity: centerCard.editMode ? 0 : 1
                scale: centerCard.editMode ? 0.65 : 1

                Behavior on opacity {
                    CielSpring {
                        damping: 0.42
                        spring: 18.0
                        mass: 2.2
                        epsilon: 0.002
                    }
                }

                Behavior on scale {
                    CielSpring {
                        damping: 0.42
                        spring: 18.0
                        mass: 2.2
                        epsilon: 0.002
                    }
                }
            }

            RowLayout {
                anchors.left: parent.left
                anchors.leftMargin: 12
                anchors.verticalCenter: parent.verticalCenter
                // FIXED: Removed absolute height: parent.height here to let RowLayout manage its items cleanly
                spacing: 6

                Item {
                    // Tightened: Shrunk from 40 to 24 to wrap tightly around the icon, closing the center gap
                    Layout.preferredWidth: 24
                    Layout.preferredHeight: 24 // FIXED: Force explicit height matching the icon bounds
                    Layout.alignment: Qt.AlignVCenter

                    CielIcon {
                        anchors.centerIn: parent
                        icon: "x"
                        size: Theme.MEDIUM
                        color: Theme.textPrimary

                        opacity: centerCard.editMode ? 1 : 0
                        scale: centerCard.editMode ? 1 : 0.65

                        Behavior on opacity {
                            CielSpring {
                                damping: 0.42
                                spring: 18.0
                                mass: 2.2
                                epsilon: 0.002
                            }
                        }

                        Behavior on scale {
                            CielSpring {
                                damping: 0.42
                                spring: 18.0
                                mass: 2.2
                                epsilon: 0.002
                            }
                        }
                    }
                }

                Item {
                    id: editTextContainer
                    Layout.alignment: Qt.AlignVCenter
                    Layout.preferredWidth: centerCard.editMode ? (editText.implicitWidth + 12) : 0 
                    Layout.preferredHeight: editText.implicitHeight // FIXED: Size directly matches text block height
                    clip: true

                    Text {
                        id: editText
                        text: "Exit"
                        color: Theme.textPrimary
                        font.pixelSize: 14
                        // FIXED: Anchored clean inside its tight container bounding box
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter 
                    }
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: centerCard.editMode = !centerCard.editMode
        }
    }
}

ProfileMutateDialog {
    id: profileDialog
}
    // AddProfileDialog {
    //     id: addProfileDialog
    // }

    // EditProfileDialog {
    //     id: editProfileDialog
    // }
}
