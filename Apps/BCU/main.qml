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

    GridLayout {
        id: grid
        columns: parent.width / 75 // pensei no tamanho de cada ícone em 72 px +/-
        anchors.top: parent.top
        width: parent.fill
        height: parent.fill

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
            text: "Pesquisando por dispositivos..."
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
