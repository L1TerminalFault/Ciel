import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Ciel.Ui

import "ui/toolbar"
import "ui/ProcessView"
import "ui/PerformanceView"
import "ui/StartupView"
import "ui/BootTimeView"
import "ui/SettingsView"

ApplicationWindow {
    id: root

    width: 860
    height: 560
    minimumWidth: 640
    minimumHeight: 420
    visible: true
    title: "Task Monitor"

    color: Theme.background
    Behavior on color {
        enabled: Theme.transitionMs > 0
        ColorAnimation {
            duration: Theme.transitionMs
            easing.type: Easing.OutCubic
        }
    }

    CielNavView {
        id: navView
        anchors.fill: parent
        header: Toolbar {
            showProcessActions: !navView.isBottomPageActive && navView.currentIndex === 0
        }

        showBottomAction: true
        bottomActionTitle: "Settings"
        bottomActionIcon: "gear-six"
        bottomActionPage: Component {
            SettingsView {}
        }

        CielNavItem {
            title: "Processes"
            icon: "squares-four"
            page: Component {
                Item {
                    anchors.fill: parent
                    anchors.margins: Theme.metrics.spacingLg

                    CielSquircle {
                        id: processAreaBody
                        anchors.fill: parent
                        color: "transparent"

                        ProcessView {
                            anchors.fill: parent
                        }
                    }
                }
            }
        }

        CielNavItem {
            title: "Performance"
            icon: "speedometer"
            page: Component {
                Item {
                    anchors.fill: parent
                    anchors.margins: Theme.metrics.spacingLg

                    CielSquircle {
                        id: performanceAreaBody
                        anchors.fill: parent
                        color: "transparent"

                        PerformanceView {
                            anchors.fill: parent
                        }
                    }
                }
            }
        }

        CielNavItem {
            title: "Startup"
            icon: "rocket-launch"
            page: Component {
                Item {
                    anchors.fill: parent
                    anchors.margins: Theme.metrics.spacingLg

                    CielSquircle {
                        id: startupAreaBody
                        anchors.fill: parent
                        color: "transparent"

                        StartupView {
                            anchors.fill: parent
                        }
                    }
                }
            }
        }

        CielNavItem {
            title: "Boot Time"
            icon: "timer"
            page: Component {
                Item {
                    anchors.fill: parent
                    anchors.margins: Theme.metrics.spacingLg

                    CielSquircle {
                        id: bootAreaBody
                        anchors.fill: parent
                        color: "transparent"

                        BootTimeView {
                            anchors.fill: parent
                        }
                    }
                }
            }
        }
    }
}
