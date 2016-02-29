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
        width: load.width
        height: load.height
        anchors.centerIn: parent

        Rectangle {
            id: load
            border.width: 1
            width: image.width
            height: image.height

            Image {
                id: image
                source: "qrc:/pics/qtlogo.png"
            }

            Text {
                id: loadingText
                text: "Carregando aplicativo " + dtS.name
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: load.bottom
            }
        }
    }
}
