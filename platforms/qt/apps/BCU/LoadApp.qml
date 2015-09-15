import QtQuick 2.0
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0

Rectangle {
    color: "white"
    opacity: 0.75

    Rectangle {
        id: load
        width: loadingText.width
        height: image.height + loadingText.height + 5
        color: "transparent"
        anchors.centerIn: parent

        Image {
            id: image
            source: "qrc:/pics/brisa-logo.png"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: load.top
        }

        Text {
            id: loadingText
            text: "Carregando aplicativo " + dtS.name
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: load.bottom
        }
    }
}
