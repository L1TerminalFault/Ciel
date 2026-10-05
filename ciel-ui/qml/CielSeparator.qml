import QtQuick
import QtQuick.Layouts
import Ciel.Ui 1.0

Rectangle {
    property bool vertical: false
    Layout.fillWidth: vertical ? false : true
    Layout.fillHeight: vertical ? true : false
    Layout.preferredHeight: vertical ? 0 : 1
    Layout.preferredWidth: vertical ? 1 : 0
    color: Theme.border
    antialiasing: false
}
