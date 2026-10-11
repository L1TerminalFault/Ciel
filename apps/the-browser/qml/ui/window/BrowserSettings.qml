import QtQuick
import QtCore

Settings {
    id: root

    category: "ciel/browser"

    property string searchEngine: "google"
    property string homepage: ""
    property real zoomFactor: 1.0
    property bool confirmClose: false
    property bool showLocationSuggestions: true
    property bool sidebarCollapsed: false
    property bool javascriptEnabled: true
    property bool autoLoadImages: true
    property bool localStorageEnabled: true
    property bool javascriptCanOpenWindows: true
    property bool hyperlinkAuditingEnabled: false
    property bool pdfViewerEnabled: true
    property bool smoothScrolling: true
    property bool spellCheckEnabled: true
    property string userAgentPreset: "default"
    property string stockUserAgent: ""

    function engineChoices() {
        return [
            { value: "google", label: "Google", url: "https://www.google.com/search?q=" },
            { value: "duckduckgo", label: "DuckDuckGo", url: "https://duckduckgo.com/?q=" },
            { value: "bing", label: "Bing", url: "https://www.bing.com/search?q=" },
            { value: "brave", label: "Brave", url: "https://search.brave.com/search?q=" },
            { value: "wikipedia", label: "Wikipedia", url: "https://en.wikipedia.org/w/index.php?search=" }
        ];
    }

    function zoomChoices() {
        return [
            { value: 0.8, label: "80%" },
            { value: 0.9, label: "90%" },
            { value: 1.0, label: "100%" },
            { value: 1.1, label: "110%" },
            { value: 1.25, label: "125%" },
            { value: 1.5, label: "150%" },
            { value: 2.0, label: "200%" }
        ];
    }

    function userAgentChoices() {
        return [
            { value: "default", label: "Default" },
            { value: "chrome", label: "Chrome" },
            { value: "firefox", label: "Firefox" }
        ];
    }

    function engineLabel() {
        var choices = engineChoices();
        for (var i = 0; i < choices.length; ++i) {
            if (choices[i].value === searchEngine)
                return choices[i].label;
        }
        return "Google";
    }

    function searchUrl(query) {
        var choices = engineChoices();
        var base = choices[0].url;
        for (var i = 0; i < choices.length; ++i) {
            if (choices[i].value === searchEngine) {
                base = choices[i].url;
                break;
            }
        }
        return base + encodeURIComponent(query || "");
    }

    function isDirectNavigation(q) {
        if (/^[a-zA-Z][a-zA-Z0-9+.-]*:/.test(q))
            return true;
        if (/\s/.test(q))
            return false;
        if (/^localhost\b/i.test(q))
            return true;
        if (/^\d{1,3}(\.\d{1,3}){3}(?::\d+)?(?:\/.*)?$/.test(q))
            return true;
        if (/^[\w.-]+\.[a-z]{2,}(?::\d+)?(?:\/.*)?$/i.test(q))
            return true;
        return false;
    }

    function resolveInput(query, fallbackResolver) {
        var q = (query || "").trim();
        if (q === "")
            return "";
        if (!isDirectNavigation(q))
            return searchUrl(q);
        if (fallbackResolver)
            return fallbackResolver(q);
        if (!/^[a-zA-Z][a-zA-Z0-9+.-]*:/.test(q))
            return "https://" + q;
        return q;
    }

    function uaFor(preset) {
        if (preset === "chrome")
            return "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/131.0.0.0 Safari/537.36";
        if (preset === "firefox")
            return "Mozilla/5.0 (X11; Linux x86_64; rv:133.0) Gecko/20100101 Firefox/133.0";
        return stockUserAgent;
    }

    function resetDefaults() {
        searchEngine = "google";
        homepage = "";
        zoomFactor = 1.0;
        confirmClose = false;
        showLocationSuggestions = true;
        sidebarCollapsed = false;
        javascriptEnabled = true;
        autoLoadImages = true;
        localStorageEnabled = true;
        javascriptCanOpenWindows = true;
        hyperlinkAuditingEnabled = false;
        pdfViewerEnabled = true;
        smoothScrolling = true;
        spellCheckEnabled = true;
        userAgentPreset = "default";
    }
}
