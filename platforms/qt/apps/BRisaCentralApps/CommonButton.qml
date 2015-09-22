import QtQuick 2.0

Rectangle {

    property string btnText
    property alias mouseArea : btnMouseArea

    color:"#0099FF"
    radius:height/5

    width:nextButtonText.contentWidth*(1.2)
    height: nextButtonText.contentHeight*(1.2)

    Text {
        id:nextButtonText
        text:btnText

        anchors.centerIn: parent
        color:"white"

        font.pixelSize: parent.parent.height/15
        font.bold: true
    }

    MouseArea {
        id:btnMouseArea
        anchors.fill: parent
    }
}
