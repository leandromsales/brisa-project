import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1

ApplicationWindow {
    id: root
    title: qsTr("BCU")
    width: 480 // Screen.desktopAvailableWidth
    height: 600 // Screen.desktopAvailableHeight*0.95
    visible: true
    color: "white"

    Loader {
        id: loader
        focus: true
        z: 100
        anchors.fill: parent
    }

    Rectangle {
        id: topBar
        width: parent.width
        anchors.top: parent.top
        height: Screen.desktopAvailableHeight*0.05

        Text {
            text: "Dispositivos disponíveis"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    GridLayout {
        id: grid
        columns: parent.width / 75
        anchors.top: topBar.bottom

        AppIcon {}
        AppIcon {}
        AppIcon {}
    }

    Rectangle {
        id: statusBar
        width: parent.width
        anchors.bottom: parent.bottom
        height: Screen.desktopAvailableHeight*0.05

        Text {
            text: "Pesquisando por outros dispositivos..."
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
