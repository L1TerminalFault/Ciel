import QtQuick
import Ciel.Ui
import QtQuick.Effects

Item {
    id: root

    property string icon: ""
    property color color: Theme.textPrimary

    property int size: Theme.MEDIUM

    width: root.size
    height: root.size

    readonly property string localCielPath: root.icon.length > 0 ? ("file://" + AppPaths.dataDir("Ciel") + "/icons/" + root.icon + ".svg") : ""

    Image {
        id: sourceImage
        anchors.fill: parent

        width: root.width
        height: root.height

        source: root.localCielPath

        sourceSize.width: root.width
        sourceSize.height: root.height

        fillMode: Image.PreserveAspectFit
        smooth: true
        visible: false
    }

    MultiEffect {
        anchors.fill: parent
        source: sourceImage

        width: root.width
        height: root.height

        colorization: 1.0
        colorizationColor: root.color
    }
}
