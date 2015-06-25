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
        height: 240 // Screen.desktopAvailableheight*0.5
        anchors.centerIn: parent

        Rectangle {
            id: about
            border.width: 1
            anchors.fill: parent

            Flickable {
                id: flickArea
                anchors.fill: parent
                contentWidth: aboutText.width;
                contentHeight: aboutText.height
                flickableDirection: Flickable.HorizontalAndVerticalFlick
                clip: true

                TextEdit{
                    id: aboutText
                    wrapMode: TextEdit.Wrap
                    width:helpScreen.width;
                    readOnly:true

                    text: "Aqui haverão informações sobre o app \nSe houver muito texto, aparecerá uma barra de rolagem"
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
