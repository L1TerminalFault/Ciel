import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import QtWebEngine
import Ciel.Ui
import Ciel.Browser 1.0
import QtQuick.Dialogs

CielPopup {
    id: editProfileDialog
    contentWidth: 320
    contentHeight: 300

    // property string profileId: ""
    // property string profileColor: ""
    // property string chosenImage: ""
    property string profileId: ""
    property string profileColor: ""
    property string chosenImage: ""
    property bool editMode: false

    function openForProfile(id) {
        editMode = true;
        profileId = id;

        const info = ProfileManager.profileInfo(id);

        nameField.text = info.displayName || "";
        profileColor = info.color || "";
        chosenImage = info.profileImage || "";

        open();
    }

    function openForCreate() {
        editMode = false;
        profileId = "";
        profileColor = "";
        chosenImage = "";
        nameField.text = "";

        open();
    }

    function reset() {
        profileId = "";
        profileColor = "";
        chosenImage = "";
        nameField.text = "";
        editMode = false;
    }

    onClosed: reset()

    // function openForProfile(id) {
    //     profileId = id;

    //     const info = ProfileManager.profileInfo(id);

    //     nameField.text = info.displayName || "";
    //     profileColor = info.color || "";
    //     chosenImage = info.profileImage || "";

    //     open();
    // }

    // function reset() {
    //     profileId = "";
    //     profileColor = "";
    //     chosenImage = "";
    //     nameField.text = "";
    // }

    // onClosed: reset()

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 20

        // Text {
        //     // text: "Edit Profile"
        //     text: editMode ? "Edit Profile" : "New Profile"
        //     font.pixelSize: 14
        //     font.weight: Font.DemiBold
        //     color: Theme.textPrimary
        //     Layout.fillWidth: true
        // }

        // Profile image + display name
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 24

            // Avatar
            Item {
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: 110
                Layout.preferredHeight: 110

                Rectangle {
                    anchors.fill: parent
                    radius: width / 2
                    color: profileColor // Theme.surface
                    border.width: 1
                    border.color: Theme.border
                }

                Image {
                    id: img

                    anchors.fill: parent
                    source: editProfileDialog.chosenImage
                    fillMode: Image.PreserveAspectCrop
                    visible: false
                }

                Rectangle {
                    id: maskSource

                    anchors.fill: parent
                    radius: width / 2
                    visible: false
                    layer.enabled: true
                }

                MultiEffect {
                    anchors.fill: parent
                    source: img
                    maskEnabled: true
                    maskSource: maskSource
                    visible: editProfileDialog.chosenImage !== ""
                }


                        Text {
                            anchors.centerIn: parent
                            text: nameField.text.charAt(0).toUpperCase() // model.displayName.charAt(0).toUpperCase()
                            visible: editProfileDialog.chosenImage === ""
                            font.bold: true
                            font.pixelSize: 18
                        }
                // CielIcon {
                //     anchors.centerIn: parent
                //     icon: "user"
                //     size: Theme.MEDIUM
                //     color: Theme.textSecondary
                //     visible: editProfileDialog.chosenImage === ""
                // }

                // Clickable avatar
                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor

                    onClicked: imagePicker.open()
                }

                // Optional camera/edit indicator
                // Rectangle {
                //     id: deleteIconContainer
                //     width: 26
                //     height: 26
                //     radius: 13
                //
                //     // Position it explicitly via a fixed anchor offset boundary
                //     anchors.right: parent.right
                //     anchors.bottom: parent.bottom
                //     anchors.margins: 4
                //
                //     color: Theme.surface
                //     border.width: 1
                //     border.color: Theme.border
                //     visible: editProfileDialog.chosenImage !== ""
                //
                //     CielIcon {
                //         // Enforces structural width and height dimensions to allow parent alignment
                //         width: 16
                //         height: 16
                //         anchors.centerIn: parent
                //
                //         icon: "trash"
                //         size: Theme.SMALL
                //         color: "#ff2589"
                //     }
                //
                //     MouseArea {
                //         anchors.fill: parent
                //         cursorShape: Qt.PointingHandCursor
                //         onClicked: editProfileDialog.chosenImage = ""
                //     }
                // }
                Rectangle {
                    width: 32
                    height: 32
                    radius: width / 2

                    anchors.right: parent.right
                    anchors.bottom: parent.bottom

                    color: Theme.surface
                    border.width: 1
                    border.color: Theme.border

                    // visible: editProfileDialog.chosenImage !== ""

                    CielIcon {
                        anchors.centerIn: parent
                        icon: editProfileDialog.chosenImage !== "" ? "trash" : "pencil-simple"
                        size: Theme.SMALL
                        color: editProfileDialog.chosenImage !== "" ? "#ff2589" : Theme.textSecondary
                        // color: Theme.textSecondary
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor

                        onClicked: editProfileDialog.chosenImage === "" ? imagePicker.open() : editProfileDialog.chosenImage = ""
                    }
                }
                // Rectangle {
                //     width: 22
                //     height: 22
                //     radius: 11
                //
                //     anchors.right: parent.right
                //     anchors.bottom: parent.bottom
                //
                //     color: Theme.surface
                //     border.width: 1
                //     border.color: Theme.border
                //
                //     visible: editProfileDialog.chosenImage !== ""
                //
                //     CielIcon {
                //         anchors.centerIn: parent
                //         icon: "trash"
                //         size: Theme.SMALL
                //         color: Theme.textSecondary
                //     }
                //
                //     MouseArea {
                //         anchors.fill: parent
                //         cursorShape: Qt.PointingHandCursor
                //
                //         onClicked: editProfileDialog.chosenImage = ""
                //     }
                // }
            }

            // Display name
            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: 42

                CielSquircle {
                    anchors.fill: parent

                    color: Theme.surface
                    borderWidth: 1
                    borderColor: Theme.border
                    radius: 15
                }

                TextInput {
                    id: nameField

                    anchors.fill: parent
                    anchors.leftMargin: 14
                    anchors.rightMargin: 14
                    clip: true

                    font.pixelSize: 13
                    color: Theme.textPrimary

                    verticalAlignment: TextInput.AlignVCenter
                    selectByMouse: true

                    Text {
                        text: "Display name"
                        color: Theme.textSecondary
                        font: parent.font
                        visible: parent.text === ""
                        enabled: false

                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }

        // Actions
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Item {
                Layout.fillWidth: true
            }

            CielButton {
                text: "Cancel"

                // onClicked: editProfileDialog.close()
                onClicked: close()
            }

            CielButton {
                // text: "Save"
                text: editMode ? "Save" : "Create"
                primary: true

                onClicked: {
                    const name = nameField.text.trim();

                    if (name.length === 0)
                        return;
                    if (editMode) {
                        const updated = ProfileManager.updateProfile(profileId, name, profileColor, chosenImage);

                        if (updated)
                            close();
                        else
                            console.warn("Failed to update profile");
                    } else {
                        const newId = ProfileManager.createProfile(name, "", chosenImage);

                        if (newId) {
                            ProfileManager.switchProfile(newId);
                            close();
                        } else {
                            console.warn("Failed to create profile");
                        }
                    }
                }
                // onClicked: {
                //     const name = nameField.text.trim();

                //     if (name.length === 0 || editProfileDialog.profileId === "")
                //         return;

                //     const updated = ProfileManager.updateProfile(
                //         editProfileDialog.profileId,
                //         name,
                //         editProfileDialog.profileColor,
                //         editProfileDialog.chosenImage
                //     );

                //     if (updated)
                //         editProfileDialog.close();
                //     else
                //         console.warn("Failed to update profile");
                // }
            }
        }
    }

    FileDialog {
        id: imagePicker

        title: "Select profile image"
        nameFilters: ["Images (*.png *.jpg *.jpeg *.webp)"]

        onAccepted: {
            editProfileDialog.chosenImage = selectedFile;
        }
    }
}
