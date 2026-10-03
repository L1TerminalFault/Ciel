import QtQuick
import QtQuick.Layouts
import Ciel.Ui

Item {
    id: root

    default property list<CielNavItem> items
    property Item header: null
    property int currentIndex: 0
    property real headerHeight: 52

    RowLayout {
        anchors.fill: parent
        spacing: 0

        CielSidebar {
            id: sidebar
            Layout.fillHeight: true
            Layout.preferredWidth: sidebar.implicitWidth
            headerHeight: root.header ? root.headerHeight : 0
            model: root.items
            currentIndex: root.currentIndex
            onItemSelected: idx => {
                root.currentIndex = idx;
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            Item {
                id: headerSlot
                Layout.fillWidth: true
                Layout.preferredHeight: root.header ? root.headerHeight : 0
                visible: !!root.header
                data: root.header ? [root.header] : []

                Binding {
                    target: root.header
                    property: "anchors.fill"
                    value: headerSlot
                    when: !!root.header
                }
            }

            Item {
                id: contentArea
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true

                Repeater {
                    model: root.items

                    Loader {
                        id: pageLoader
                        anchors.fill: parent

                        readonly property bool isCurrent: index === root.currentIndex
                        active: isCurrent || status === Loader.Ready
                        visible: opacity > 0.001
                        sourceComponent: modelData.page

                        transformOrigin: Item.Center

                        opacity: isCurrent ? 1.0 : 0.0
                        scale: isCurrent ? 1.0 : 0.985

                        layer.enabled: opacityAnim.running || scaleSpring.running
                        layer.smooth: true

                        Behavior on opacity {
                            NumberAnimation {
                                id: opacityAnim
                                duration: 140
                                easing.type: Easing.OutQuad
                            }
                        }

                        Behavior on scale {
                            CielSpring {
                                id: scaleSpring
                                damping: 4.85
                                spring: 8.0
                                mass: 4.6
                                epsilon: 0.001
                            }
                        }
                    }
                }
            }
        }
    }
}
