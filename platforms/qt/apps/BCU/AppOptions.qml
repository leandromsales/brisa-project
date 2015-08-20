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
        width: 240 // Screen.desktopAvailableWidth*0.5
        height: 80
        anchors.centerIn: parent

        Rectangle {
            id: about
            width: parent.width
            height: parent.height/2
            border.width: 1
            anchors.top: mainRec.top
            color: maAbout.containsMouse ? "grey" : "white"

            Text {
                text: "Sobre o app"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            MouseArea {
                id: maAbout
                anchors.fill: about
                hoverEnabled : true
                onClicked: loader.source = "AboutApp.qml"
            }
        }

        Rectangle {
            id: load
            width: parent.width
            height: parent.height/2
            border.width: 1
            anchors.top: about.bottom
            color: maLoad.containsMouse ? "grey" : "white"

            Text {
                text: "Carregar o app"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            MouseArea {
                id: maLoad
                anchors.fill: load
                hoverEnabled : true
                onClicked: {
                    loader.source = "";
                    // 1. chamar a função de download usando como parametros
                    //    o nome do app e a url com final .compe
                    // 2. descomprimir o .compe
                    manager.run(dtS.appURL)
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
