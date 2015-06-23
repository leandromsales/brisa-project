import QtQuick 2.0
import QtQuick.Dialogs 1.2

Rectangle {
    color: "transparent"

    Rectangle {
        id: mainRec
        width: 240 // Screen.desktopAvailableWidth*0.5
        height: 80
        anchors.centerIn: parent

        Rectangle {
            id: about
            width: parent.width
            height: parent.height/2
            anchors.top: mainRec.top

            Text {
                text: "sobre o app"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Rectangle {
            id: load
            width: parent.width
            height: parent.height/2
            anchors.top: about.bottom

            Text {
                text: "carregar o app"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }
}
