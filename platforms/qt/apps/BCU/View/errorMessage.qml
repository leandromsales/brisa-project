import QtQuick 2.0
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0

Rectangle {
    color: "white"
    opacity: 0.75
    focus: true
    Keys.onPressed: {
        if (event.key == Qt.Key_Backspace) {
            loader.source = ""
        }
    }

    Rectangle {
        id: mainRec
        width: msg.width*1.2
        height:msg.height*4
        anchors.centerIn: parent
        border.width: 1

        Rectangle {
            id: rec1
            width: msg.width*1.2
            height:msg.height*2
            anchors.top: mainRec.top

            Text {
                id: msg
                text: "Houve um erro ao apagar o aplicativo."
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Rectangle {
            width: msg.width*1.2
            height:msg.height*2
            anchors.top: rec1.bottom

            MouseArea {
                anchors.fill: parent
                onClicked: loader.source = ""

                Text {
                    text: "OK"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }

    Rectangle {
        width: mainRec.width
        height: mainRec.height
        x: mainRec.x + 2
        y: mainRec.y + 2
        z: -1
        color: "black"
    }
}
