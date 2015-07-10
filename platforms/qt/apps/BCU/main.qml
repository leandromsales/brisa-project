import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0

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

        Rectangle {
            height: 1
            width: parent.width
            anchors.bottom: parent.bottom

            LinearGradient {
                anchors.fill: parent
                start: Qt.point(0, 0)
                end: Qt.point(parent.width, 0)
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "white" }
                    GradientStop { position: 0.5; color: "black" }
                    GradientStop { position: 1.0; color: "white" }
                }
            }
        }
    }

    GridView {
        id: grid
        cellWidth: 90
        cellHeight: 110
        width: parent.width
        height: Screen.desktopAvailableHeight*0.9
        anchors.top: topBar.bottom
        model: myModel
        delegate: Loader {
            AppIcon{}
            property variant modelData: grid.model.get(index)
        }
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
