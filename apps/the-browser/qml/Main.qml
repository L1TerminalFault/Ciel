import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtWebEngine
import QtQuick.Effects
import Ciel.Ui
import Ciel.Browser 1.0

import "ui/tabs"
import "ui/startpage"
import "ui/history"

ApplicationWindow {
    id: window
    width: 1040
    height: 680
    minimumWidth: 640
    minimumHeight: 460
    visible: true
    title: "Browser"

    color: Theme.background
    property int railOrientation: Qt.Vertical

    // ─── Profile management (one active at a time, but switchable) ───
    property var profileCache: ({})

    function getOrCreateProfile(profileId) {
        if (profileCache[profileId])
            return profileCache[profileId]

        var storage = ProfileManager.webEngineStoragePathFor(profileId)
        var cache   = storage + "/cache"

        var qml = `
            import QtWebEngine
            WebEngineProfile {
                storageName: "ciel-${profileId}"
                offTheRecord: false
                persistentCookiesPolicy: WebEngineProfile.ForcePersistentCookies
                persistentStoragePath: "${storage}"
                cachePath: "${cache}"
                httpCacheType: WebEngineProfile.DiskHttpCache

                onDownloadRequested: function(download) {
                    var urlStr = download.url ? download.url.toString() : ""
                    if (urlStr.startsWith("blob:") || urlStr.startsWith("data:")) {
                        download.accept()
                    } else {
                        download.cancel()
                        browserConfig.startDownload(download.url, download.downloadFileName, download.mimeType)
                        tabRail.downloadsPopup.open()
                    }
                }
            }`

        var p = Qt.createQmlObject(qml, window, "profile-" + profileId)
        profileCache[profileId] = p
        return p
    }

    readonly property var currentWebProfile: getOrCreateProfile(ProfileManager.activeProfileId)

    Behavior on color {
        enabled: Theme.transitionMs > 0
        ColorAnimation {
            duration: Theme.transitionMs
            easing.type: Easing.OutCubic
        }
    }

    BrowserConfig {
        id: browserConfig
    }

    WorkspaceModel {
        id: workspaceModel
    }

    // No more static sharedProfile – everything goes through currentWebProfile

    TabBar {
        id: tabRail
        workspaceModel: workspaceModel
        getWorkspaceViews: idx => webContainer.getWorkspaceViews(idx)
        config: browserConfig
        activeView: webContainer.currentActiveView
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: window.railOrientation === Qt.Horizontal ? parent.right : undefined
        anchors.bottom: window.railOrientation === Qt.Vertical ? parent.bottom : undefined
        z: 10
        onHistoryRequested: {
            var model = workspaceModel.tabModel(workspaceModel.currentWorkspaceId)
            if (model) model.addTab("ciel://history")
        }
        onNewTabRequested: {
            var model = workspaceModel.tabModel(workspaceModel.currentWorkspaceId)
            if (model) model.addTab()
        }
    }

    Rectangle {
        id: railDivider
        color: Theme.border
        z: 9
        anchors.left: window.railOrientation === Qt.Vertical ? tabRail.right : parent.left
        anchors.right: window.railOrientation === Qt.Vertical ? undefined : parent.right
        anchors.top: window.railOrientation === Qt.Vertical ? parent.top : tabRail.bottom
        anchors.bottom: window.railOrientation === Qt.Vertical ? parent.bottom : undefined
        width: window.railOrientation === Qt.Vertical ? 1 : parent.width
        height: window.railOrientation === Qt.Vertical ? parent.height : 1
    }

    Item {
        id: webContainer
        anchors.left: window.railOrientation === Qt.Vertical ? railDivider.right : parent.left
        anchors.right: parent.right
        anchors.top: window.railOrientation === Qt.Vertical ? parent.top : railDivider.bottom
        anchors.bottom: parent.bottom
        clip: true

        property real animatedWorkspaceIndex: workspaceModel.currentIndex
        Behavior on animatedWorkspaceIndex {
            CielSpring {
                damping: 0.44
                spring: 3.8
                mass: 1.0
                epsilon: 0.001
            }
        }

        function getWorkspaceViews(idx) {
            var item = workspaceRepeater.itemAt(idx)
            return item ? item.viewRepeater : null
        }

        property var currentActiveView: null
        readonly property var activeTabModel: workspaceModel.tabModel(workspaceModel.currentWorkspaceId)

        property real animatedBackProgress: navFilter.backProgress
        property real animatedForwardProgress: navFilter.forwardProgress

        Behavior on animatedBackProgress {
            enabled: !navFilter.active
            CielSpring { damping: 0.32; spring: 5.2; mass: 1.0; epsilon: 0.001 }
        }
        Behavior on animatedForwardProgress {
            enabled: !navFilter.active
            CielSpring { damping: 0.32; spring: 5.2; mass: 1.0; epsilon: 0.001 }
        }

        SwipeGestureFilter {
            id: navFilter
            canGoBack: webContainer.currentActiveView ? webContainer.currentActiveView.canGoBack : false
            canGoForward: webContainer.currentActiveView ? webContainer.currentActiveView.canGoForward : false
            threshold: 90.0
            onBackTriggered: {
                if (webContainer.currentActiveView && webContainer.currentActiveView.canGoBack)
                    webContainer.currentActiveView.goBack()
            }
            onForwardTriggered: {
                if (webContainer.currentActiveView && webContainer.currentActiveView.canGoForward)
                    webContainer.currentActiveView.goForward()
            }
        }

        Repeater {
            id: workspaceRepeater
            model: workspaceModel
            Item {
                id: workspaceContainer
                width: parent.width
                height: parent.height
                readonly property int wsIndex: index
                readonly property var wsTabModel: workspaceModel.tabModel(model.id)
                readonly property real diff: wsIndex - webContainer.animatedWorkspaceIndex
                visible: Math.abs(diff) < 1.05
                enabled: Math.abs(diff) < 0.1
                z: (workspaceModel.currentIndex === wsIndex) ? 10 : 1

                transform: [
                    Translate { x: Math.round(workspaceContainer.diff * workspaceContainer.width) },
                    Scale {
                        origin.x: workspaceContainer.width / 2
                        origin.y: workspaceContainer.height / 2
                        xScale: 1.0 - (Math.min(1.0, Math.abs(workspaceContainer.diff)) * 0.08)
                        yScale: 1.0 - (Math.min(1.0, Math.abs(workspaceContainer.diff)) * 0.08)
                    }
                ]
                opacity: Math.max(0.0, 1.0 - (Math.abs(diff) * 1.25))

                property alias tabModel: workspaceContainer.wsTabModel
                property alias viewRepeater: tabViewRepeater

                Repeater {
                    id: tabViewRepeater
                    model: workspaceContainer.wsTabModel
                    Item {
                        id: pageContainer
                        anchors.fill: parent
                        readonly property bool isCurrentPage: (workspaceModel.currentIndex === wsIndex)
                                                              && (workspaceContainer.wsTabModel
                                                                  && workspaceContainer.wsTabModel.currentIndex === index)
                        visible: isCurrentPage

                        onIsCurrentPageChanged: {
                            if (isCurrentPage)
                                webContainer.currentActiveView = engineView
                        }
                        Component.onCompleted: {
                            if (isCurrentPage)
                                webContainer.currentActiveView = engineView
                        }

                        property alias engine: engineView
                        readonly property string urlStr: model.url ? model.url.toString() : ""
                        readonly property bool isBlank: urlStr === "about:blank" || urlStr === ""
                        readonly property bool isHistory: urlStr === "ciel://history" || urlStr === "about:history"

                        WebEngineView {
                            id: engineView
                            anchors.fill: parent
                            backgroundColor: Theme.background
                            visible: !pageContainer.isBlank && !pageContainer.isHistory

                            // ← uses the persistent, switchable profile
                            profile: window.currentWebProfile

                            url: model.url

                            onTitleChanged: {
                                if (workspaceContainer.wsTabModel && !pageContainer.isHistory)
                                    workspaceContainer.wsTabModel.updateTitle(index, title)
                            }
                            onUrlChanged: {
                                if (workspaceContainer.wsTabModel)
                                    workspaceContainer.wsTabModel.updateUrl(index, url)
                            }
                            onLoadingChanged: loadRequest => {
                                if (workspaceContainer.wsTabModel) {
                                    workspaceContainer.wsTabModel.updateLoading(index, loading)
                                    workspaceContainer.wsTabModel.updateNavigation(index, canGoBack, canGoForward)
                                }
                            }
                            onLoadProgressChanged: {
                                if (workspaceContainer.wsTabModel)
                                    workspaceContainer.wsTabModel.updateProgress(index, loadProgress)
                            }
                        }

                        StartPage {
                            anchors.fill: parent
                            visible: pageContainer.isBlank
                            config: browserConfig
                            activeView: engineView
                        }

                        HistoryPage {
                            anchors.fill: parent
                            visible: pageContainer.isHistory
                            onOpenTabRequested: targetUrl => {
                                if (workspaceContainer.wsTabModel)
                                    workspaceContainer.wsTabModel.addTab(targetUrl)
                            }
                        }
                    }
                }
            }
        }

        // … backIndicator + forwardIndicator stay exactly as you had them …
        Item {
            id: backIndicator
            anchors.verticalCenter: parent.verticalCenter
            width: 44; height: 44; z: 99
            readonly property real prog: webContainer.animatedBackProgress
            readonly property bool triggered: prog >= 1.0
            property real currentRadius: triggered ? 22 : 12
            Behavior on currentRadius {
                CielSpring { damping: 0.28; spring: 5.4; mass: 0.9; epsilon: 0.001 }
            }
            visible: prog > 0.001
            opacity: Math.min(1.0, prog * 3.5)
            scale: triggered ? 1.06 : (0.84 + (0.16 * Math.min(1.0, prog)))
            x: -width + (prog * (width + 24))
            Behavior on scale {
                CielSpring { damping: 0.28; spring: 5.4; mass: 0.9; epsilon: 0.001 }
            }
            CielSquircle {
                id: backCardBg
                anchors.fill: parent
                radius: backIndicator.currentRadius
                color: backIndicator.triggered ? Theme.accent : Theme.surface
                borderWidth: backIndicator.triggered ? 0 : 1
                borderColor: Theme.border
                visible: false
                Behavior on color { ColorAnimation { duration: 120 } }
            }
            MultiEffect {
                anchors.fill: backCardBg
                source: backCardBg
                shadowEnabled: true
                shadowColor: Qt.rgba(0, 0, 0, 0.12)
                shadowBlur: 0.7
                shadowVerticalOffset: 3
                shadowHorizontalOffset: 0
            }
            CielIcon {
                anchors.centerIn: parent
                icon: "arrow-left"
                size: Theme.MEDIUM
                color: backIndicator.triggered ? "#FFFFFF" : Theme.textPrimary
                Behavior on color { ColorAnimation { duration: 120 } }
            }
        }

        Item {
            id: forwardIndicator
            anchors.verticalCenter: parent.verticalCenter
            width: 44; height: 44; z: 99
            readonly property real prog: webContainer.animatedForwardProgress
            readonly property bool triggered: prog >= 1.0
            property real currentRadius: triggered ? 22 : 12
            Behavior on currentRadius {
                CielSpring { damping: 0.28; spring: 5.4; mass: 0.9; epsilon: 0.001 }
            }
            visible: prog > 0.001
            opacity: Math.min(1.0, prog * 3.5)
            scale: triggered ? 1.06 : (0.84 + (0.16 * Math.min(1.0, prog)))
            x: parent.width - (prog * (width + 24))
            Behavior on scale {
                CielSpring { damping: 0.28; spring: 5.4; mass: 0.9; epsilon: 0.001 }
            }
            CielSquircle {
                id: forwardCardBg
                anchors.fill: parent
                radius: forwardIndicator.currentRadius
                color: forwardIndicator.triggered ? Theme.accent : Theme.surface
                borderWidth: forwardIndicator.triggered ? 0 : 1
                borderColor: Theme.border
                visible: false
                Behavior on color { ColorAnimation { duration: 120 } }
            }
            MultiEffect {
                anchors.fill: forwardCardBg
                source: forwardCardBg
                shadowEnabled: true
                shadowColor: Qt.rgba(0, 0, 0, 0.12)
                shadowBlur: 0.7
                shadowVerticalOffset: 3
                shadowHorizontalOffset: 0
            }
            CielIcon {
                anchors.centerIn: parent
                icon: "arrow-right"
                size: Theme.MEDIUM
                color: forwardIndicator.triggered ? "#FFFFFF" : Theme.textPrimary
                Behavior on color { ColorAnimation { duration: 120 } }
            }
        }
    }
}

// ApplicationWindow {
//     id: window
//
//     width: 1040
//     height: 680
//     minimumWidth: 640
//     minimumHeight: 460
//     visible: true
//     title: "Ciel Browser"
//
//     color: Theme.background
//
//     property int railOrientation: Qt.Vertical
//
// // Inside ApplicationWindow
// property var profileCache: ({})   // id → WebEngineProfile
//
// function getOrCreateProfile(profileId) {
//     if (profileCache[profileId])
//         return profileCache[profileId]
//
//     // Ensure directories exist (even if this profile has never been switched to)
//     // You can also call a C++ helper that does QDir().mkpath
//     var storage = ProfileManager.webEngineStoragePathFor(profileId)
//     var cache   = storage + "/cache"
//
//     var qml = `
//         import QtWebEngine
//         WebEngineProfile {
//             storageName: "ciel-${profileId}"
//             offTheRecord: false
//             persistentCookiesPolicy: WebEngineProfile.ForcePersistentCookies
//             persistentStoragePath: "${storage}"
//             cachePath: "${cache}"
//             httpCacheType: WebEngineProfile.DiskHttpCache
//             // httpCacheMaximumSize: 150 * 1024 * 1024   // optional
//         }`
//
//     var p = Qt.createQmlObject(qml, window, "profile-" + profileId)
//     profileCache[profileId] = p
//     return p
// }
//
// // Convenience
// readonly property var currentWebProfile: getOrCreateProfile(ProfileManager.activeProfileId)
//
//     Behavior on color {
//         enabled: Theme.transitionMs > 0
//         ColorAnimation {
//             duration: Theme.transitionMs
//             easing.type: Easing.OutCubic
//         }
//     }
//
//     BrowserConfig {
//         id: browserConfig
//     }
//
//     WorkspaceModel {
//         id: workspaceModel
//     }
//
//     WebEngineProfile {
//         id: sharedProfile
//
//         onDownloadRequested: download => {
//             var urlStr = download.url ? download.url.toString() : "";
//             if (urlStr.startsWith("blob:") || urlStr.startsWith("data:")) {
//                 download.accept();
//             } else {
//                 download.cancel();
//                 browserConfig.startDownload(download.url, download.downloadFileName, download.mimeType);
//                 tabRail.downloadsPopup.open();
//             }
//         }
//     }
//
//     TabBar {
//         id: tabRail
//         workspaceModel: workspaceModel
//         getWorkspaceViews: idx => webContainer.getWorkspaceViews(idx)
//         config: browserConfig
//         activeView: webContainer.currentActiveView
//
//         anchors.left: parent.left
//         anchors.top: parent.top
//         anchors.right: window.railOrientation === Qt.Horizontal ? parent.right : undefined
//         anchors.bottom: window.railOrientation === Qt.Vertical ? parent.bottom : undefined
//         z: 10
//
//         onHistoryRequested: {
//             var model = workspaceModel.tabModel(workspaceModel.currentWorkspaceId);
//             if (model) {
//                 model.addTab("ciel://history");
//             }
//         }
//
//         onNewTabRequested: {
//             var model = workspaceModel.tabModel(workspaceModel.currentWorkspaceId);
//             if (model) {
//                 model.addTab();
//             }
//         }
//     }
//
//     Rectangle {
//         id: railDivider
//         color: Theme.border
//         z: 9
//
//         anchors.left: window.railOrientation === Qt.Vertical ? tabRail.right : parent.left
//         anchors.right: window.railOrientation === Qt.Vertical ? undefined : parent.right
//         anchors.top: window.railOrientation === Qt.Vertical ? parent.top : tabRail.bottom
//         anchors.bottom: window.railOrientation === Qt.Vertical ? parent.bottom : undefined
//
//         width: window.railOrientation === Qt.Vertical ? 1 : parent.width
//         height: window.railOrientation === Qt.Vertical ? parent.height : 1
//     }
//
//     Item {
//         id: webContainer
//         anchors.left: window.railOrientation === Qt.Vertical ? railDivider.right : parent.left
//         anchors.right: parent.right
//         anchors.top: window.railOrientation === Qt.Vertical ? parent.top : railDivider.bottom
//         anchors.bottom: parent.bottom
//         clip: true
//
//         property real animatedWorkspaceIndex: workspaceModel.currentIndex
//
//         Behavior on animatedWorkspaceIndex {
//             CielSpring {
//                 damping: 0.44
//                 spring: 3.8
//                 mass: 1.0
//                 epsilon: 0.001
//             }
//         }
//
//         function getWorkspaceViews(idx) {
//             var item = workspaceRepeater.itemAt(idx);
//             return item ? item.viewRepeater : null;
//         }
//
//         property var currentActiveView: null
//         readonly property var activeTabModel: workspaceModel.tabModel(workspaceModel.currentWorkspaceId)
//
//         property real animatedBackProgress: navFilter.backProgress
//         property real animatedForwardProgress: navFilter.forwardProgress
//
//         Behavior on animatedBackProgress {
//             enabled: !navFilter.active
//             CielSpring {
//                 damping: 0.32
//                 spring: 5.2
//                 mass: 1.0
//                 epsilon: 0.001
//             }
//         }
//
//         Behavior on animatedForwardProgress {
//             enabled: !navFilter.active
//             CielSpring {
//                 damping: 0.32
//                 spring: 5.2
//                 mass: 1.0
//                 epsilon: 0.001
//             }
//         }
//
//         SwipeGestureFilter {
//             id: navFilter
//             canGoBack: webContainer.currentActiveView ? webContainer.currentActiveView.canGoBack : false
//             canGoForward: webContainer.currentActiveView ? webContainer.currentActiveView.canGoForward : false
//             threshold: 90.0
//
//             onBackTriggered: {
//                 if (webContainer.currentActiveView && webContainer.currentActiveView.canGoBack)
//                     webContainer.currentActiveView.goBack();
//             }
//
//             onForwardTriggered: {
//                 if (webContainer.currentActiveView && webContainer.currentActiveView.canGoForward)
//                     webContainer.currentActiveView.goForward();
//             }
//         }
//
//         Repeater {
//             id: workspaceRepeater
//             model: workspaceModel
//
//             Item {
//                 id: workspaceContainer
//                 width: parent.width
//                 height: parent.height
//
//                 readonly property int wsIndex: index
//                 readonly property var wsTabModel: workspaceModel.tabModel(model.id)
//                 readonly property real diff: wsIndex - webContainer.animatedWorkspaceIndex
//
//                 visible: Math.abs(diff) < 1.05
//                 enabled: Math.abs(diff) < 0.1
//                 z: (workspaceModel.currentIndex === wsIndex) ? 10 : 1
//
//                 transform: [
//                     Translate {
//                         x: Math.round(workspaceContainer.diff * workspaceContainer.width)
//                     },
//                     Scale {
//                         origin.x: workspaceContainer.width / 2
//                         origin.y: workspaceContainer.height / 2
//                         xScale: 1.0 - (Math.min(1.0, Math.abs(workspaceContainer.diff)) * 0.08)
//                         yScale: 1.0 - (Math.min(1.0, Math.abs(workspaceContainer.diff)) * 0.08)
//                     }
//                 ]
//
//                 opacity: Math.max(0.0, 1.0 - (Math.abs(diff) * 1.25))
//
//                 property alias tabModel: workspaceContainer.wsTabModel
//                 property alias viewRepeater: tabViewRepeater
//
//                 Repeater {
//                     id: tabViewRepeater
//                     model: workspaceContainer.wsTabModel
//
//                     Item {
//                         id: pageContainer
//                         anchors.fill: parent
//
//                         readonly property bool isCurrentPage: (workspaceModel.currentIndex === wsIndex) && (workspaceContainer.wsTabModel && workspaceContainer.wsTabModel.currentIndex === index)
//                         visible: isCurrentPage
//
//                         onIsCurrentPageChanged: {
//                             if (isCurrentPage) {
//                                 webContainer.currentActiveView = engineView;
//                             }
//                         }
//
//                         Component.onCompleted: {
//                             if (isCurrentPage) {
//                                 webContainer.currentActiveView = engineView;
//                             }
//                         }
//
//                         property alias engine: engineView
//
//                         readonly property string urlStr: model.url ? model.url.toString() : ""
//                         readonly property bool isBlank: urlStr === "about:blank" || urlStr === ""
//                         readonly property bool isHistory: urlStr === "ciel://history" || urlStr === "about:history"
//
//                         WebEngineView {
//                             id: engineView
//                             anchors.fill: parent
//                             backgroundColor: Theme.background
//                             visible: !pageContainer.isBlank && !pageContainer.isHistory
//                             // profile: sharedProfile
//
//                             profile: window.currentWebProfile
//
//                             url: model.url
//
//                             onTitleChanged: {
//                                 if (workspaceContainer.wsTabModel && !pageContainer.isHistory)
//                                     workspaceContainer.wsTabModel.updateTitle(index, title);
//                             }
//                             onUrlChanged: {
//                                 if (workspaceContainer.wsTabModel)
//                                     workspaceContainer.wsTabModel.updateUrl(index, url);
//                             }
//                             onLoadingChanged: loadRequest => {
//                                 if (workspaceContainer.wsTabModel) {
//                                     workspaceContainer.wsTabModel.updateLoading(index, loading);
//                                     workspaceContainer.wsTabModel.updateNavigation(index, canGoBack, canGoForward);
//                                 }
//                             }
//                             onLoadProgressChanged: {
//                                 if (workspaceContainer.wsTabModel)
//                                     workspaceContainer.wsTabModel.updateProgress(index, loadProgress);
//                             }
//                         }
//
//                         StartPage {
//                             anchors.fill: parent
//                             visible: pageContainer.isBlank
//                             config: browserConfig
//                             activeView: engineView
//                         }
//
//                         HistoryPage {
//                             anchors.fill: parent
//                             visible: pageContainer.isHistory
//                             onOpenTabRequested: targetUrl => {
//                                 if (workspaceContainer.wsTabModel) {
//                                     workspaceContainer.wsTabModel.addTab(targetUrl);
//                                 }
//                             }
//                         }
//                     }
//                 }
//             }
//         }
//
//         Item {
//             id: backIndicator
//             anchors.verticalCenter: parent.verticalCenter
//             width: 44
//             height: 44
//             z: 99
//
//             readonly property real prog: webContainer.animatedBackProgress
//             readonly property bool triggered: prog >= 1.0
//
//             property real currentRadius: triggered ? 22 : 12
//
//             Behavior on currentRadius {
//                 CielSpring {
//                     damping: 0.28
//                     spring: 5.4
//                     mass: 0.9
//                     epsilon: 0.001
//                 }
//             }
//
//             visible: prog > 0.001
//             opacity: Math.min(1.0, prog * 3.5)
//             scale: triggered ? 1.06 : (0.84 + (0.16 * Math.min(1.0, prog)))
//             x: -width + (prog * (width + 24))
//
//             Behavior on scale {
//                 CielSpring {
//                     damping: 0.28
//                     spring: 5.4
//                     mass: 0.9
//                     epsilon: 0.001
//                 }
//             }
//
//             CielSquircle {
//                 id: backCardBg
//                 anchors.fill: parent
//                 radius: backIndicator.currentRadius
//                 color: backIndicator.triggered ? Theme.accent : Theme.surface
//                 borderWidth: backIndicator.triggered ? 0 : 1
//                 borderColor: Theme.border
//                 visible: false
//
//                 Behavior on color {
//                     ColorAnimation {
//                         duration: 120
//                     }
//                 }
//             }
//
//             MultiEffect {
//                 anchors.fill: backCardBg
//                 source: backCardBg
//                 shadowEnabled: true
//                 shadowColor: Qt.rgba(0, 0, 0, 0.12)
//                 shadowBlur: 0.7
//                 shadowVerticalOffset: 3
//                 shadowHorizontalOffset: 0
//             }
//
//             CielIcon {
//                 anchors.centerIn: parent
//                 icon: "arrow-left"
//                 size: Theme.MEDIUM
//                 color: backIndicator.triggered ? "#FFFFFF" : Theme.textPrimary
//
//                 Behavior on color {
//                     ColorAnimation {
//                         duration: 120
//                     }
//                 }
//             }
//         }
//
//         Item {
//             id: forwardIndicator
//             anchors.verticalCenter: parent.verticalCenter
//             width: 44
//             height: 44
//             z: 99
//
//             readonly property real prog: webContainer.animatedForwardProgress
//             readonly property bool triggered: prog >= 1.0
//
//             property real currentRadius: triggered ? 22 : 12
//
//             Behavior on currentRadius {
//                 CielSpring {
//                     damping: 0.28
//                     spring: 5.4
//                     mass: 0.9
//                     epsilon: 0.001
//                 }
//             }
//
//             visible: prog > 0.001
//             opacity: Math.min(1.0, prog * 3.5)
//             scale: triggered ? 1.06 : (0.84 + (0.16 * Math.min(1.0, prog)))
//             x: parent.width - (prog * (width + 24))
//
//             Behavior on scale {
//                 CielSpring {
//                     damping: 0.28
//                     spring: 5.4
//                     mass: 0.9
//                     epsilon: 0.001
//                 }
//             }
//
//             CielSquircle {
//                 id: forwardCardBg
//                 anchors.fill: parent
//                 radius: forwardIndicator.currentRadius
//                 color: forwardIndicator.triggered ? Theme.accent : Theme.surface
//                 borderWidth: forwardIndicator.triggered ? 0 : 1
//                 borderColor: Theme.border
//                 visible: false
//
//                 Behavior on color {
//                     ColorAnimation {
//                         duration: 120
//                     }
//                 }
//             }
//
//             MultiEffect {
//                 anchors.fill: forwardCardBg
//                 source: forwardCardBg
//                 shadowEnabled: true
//                 shadowColor: Qt.rgba(0, 0, 0, 0.12)
//                 shadowBlur: 0.7
//                 shadowVerticalOffset: 3
//                 shadowHorizontalOffset: 0
//             }
//
//             CielIcon {
//                 anchors.centerIn: parent
//                 icon: "arrow-right"
//                 size: Theme.MEDIUM
//                 color: forwardIndicator.triggered ? "#FFFFFF" : Theme.textPrimary
//
//                 Behavior on color {
//                     ColorAnimation {
//                         duration: 120
//                     }
//                 }
//             }
//         }
//     }
// }
