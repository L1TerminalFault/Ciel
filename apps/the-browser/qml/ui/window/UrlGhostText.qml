import QtQuick
import Ciel.Ui
import Ciel.Browser 1.0

Item {
    id: root

    property var target: null
    property bool active: false
    property bool blocked: false

    property string completion: ""
    property string lastShown: ""

    readonly property var field: target ? target.inputField : null
    readonly property bool showing: active && !blocked && completion !== "" && field !== null && field.cursorPosition === field.length && field.selectedText === ""

    property int curLen: 0
    property int oldLen: 0
    property bool edited: false

    parent: field
    width: ghostText.implicitWidth
    height: ghostText.implicitHeight
    x: field ? field.cursorRectangle.x : 0
    y: field ? field.cursorRectangle.y + Math.round((field.cursorRectangle.height - height) / 2) : 0

    property real shownP: showing ? 1.0 : 0.0
    Behavior on shownP {
        CielSpring {
            damping: 0.6
            spring: 6.0
            mass: 0.9
            epsilon: 0.001
        }
    }
    visible: shownP > 0.01
    opacity: Math.max(0.0, Math.min(1.0, shownP))

    onCompletionChanged: if (completion !== "")
        lastShown = completion
    onActiveChanged: {
        completion = "";
        edited = false;
    }

    function accept() {
        if (!showing)
            return false;
        var add = completion;
        completion = "";
        field.insert(field.length, add);
        return true;
    }

    function completed(text) {
        return showing ? text + completion : text;
    }

    function handleKey(event) {
        if ((event.key === Qt.Key_Tab || event.key === Qt.Key_Right) && event.modifiers === Qt.NoModifier && accept())
            event.accepted = true;
    }

    function evaluate() {
        if (!edited)
            return;
        edited = false;
        var f = field;
        if (!active || !f || curLen < oldLen || f.text === "") {
            completion = "";
            return;
        }
        completion = ProfileManager.completeUrl(f.text);
    }

    Connections {
        target: root.field
        ignoreUnknownSignals: true
        function onTextChanged() {
            root.oldLen = root.curLen;
            root.curLen = root.field.length;
        }
        function onTextEdited() {
            root.edited = true;
            Qt.callLater(root.evaluate);
        }
    }

    Text {
        id: ghostText
        text: root.lastShown
        font: root.field ? root.field.font : Qt.font({})
        color: Theme.textSecondary
        opacity: 0.6
    }
}
