import QtQuick

QtObject {
    id: root

    property string title: "new tab"
    property string icon: ""
    property bool hasCloseBtn: true
    property Component page: null
}
