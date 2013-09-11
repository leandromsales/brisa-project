import QtQuick 2.0

Rectangle {
    id: panel
    clip: true
    property int currentPosition: panel.width
    function changeLabel(newLabel) {
        mediaTitle.text = newLabel
        mediaTitle.color = "#00000000"
        mediaTitle.color = "#179c78"
    }
    function setFrame(x, y, width, height){
        panel.x = x
        panel.y = y
        panel.width = width
        panel.height = height
    }
    color: "#00FFFFFF"
    Text {
        id: mediaTitle
        font.family: "helvetica"
        font.pixelSize: 18
        text: ""
        color: "#179c78"
        Behavior on color {
            ColorAnimation { from: "#00000000"; to: "#179c78"; duration: 400 }
        }
        x: currentPosition
        anchors.verticalCenter: parent.verticalCenter
    }
    Timer {
        id: timer
        interval: 300; repeat: true
        running: false
        triggeredOnStart: false

        onTriggered: {
            if(currentPosition <= -mediaTitle.width)
                currentPosition = panel.width
            else
                currentPosition = currentPosition - 15;
        }
    }
    states: State {
            name: "moving"
            when: (mediaTitle.text!=="")
            PropertyChanges {
                target: timer
                running: true
            }

    }
}
