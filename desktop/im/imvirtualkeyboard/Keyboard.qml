import QtQuick 2.0
import QtQuick 2.5

Rectangle{
    id: candidateWindow
    height: 60
    property string words: ""

    FontMetrics {
        id: fontMetrics
        font.bold: true
    }

    Text {
        id: spell
        x :3
        y: 3
        height: 30
        anchors.left: candidateWindow.left
        anchors.right:  candidateWindow.right
        anchors.top:  candidateWindow.top
        text: words
        color: "green"
    }

    ListView {
        id: view
        anchors.top: spell.bottom
        anchors.bottom:  candidateWindow.bottom
        anchors.left:  candidateWindow.left
        anchors.right:  candidateWindow.right
        orientation: ListView.Horizontal
        spacing :4
        height: 30
        delegate: Text {
            text: index+1+" "+ modelData
        }
    }

    Connections{
        target: imctx //C++ data instance.
        function onDataChange(model, spellString) {//c++ SIGNALS dataChange(QStrignList,QString)
            view.model =model
            words = spellString
            candidateWindow.width = fontMetrics.advanceWidth(model.join("") +"123456789123456789")+4*8
        }
    }
}
