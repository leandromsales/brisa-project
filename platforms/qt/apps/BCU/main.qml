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

    StackView {
        id:stackPages
        objectName: "stack"
        anchors.fill: parent

        function pushObject() {
            appRunnable.visible = true
            stackPages.push(appRunnable);
        }

        initialItem: inicialPage

        Rectangle {
            id: inicialPage
            height:root.height
            width:root.width

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

            Rectangle {
                id: containerGrid
                width: parent.width
                height: Screen.desktopAvailableHeight*0.9
                anchors.top: topBar.bottom

                Flickable {
                    id: flickArea
                    anchors.fill: parent
                    contentWidth: parent.width
                    contentHeight: parent.height
                    // testar essa parte melhor, parece que o stop at bounds não está ok
                    boundsBehavior: Flickable.StopAtBounds
                    flickableDirection: Flickable.VerticalFlick
                    clip: true

                    GridView {
                        id: grid
                        cellWidth: 90
                        cellHeight: 110
                        width: parent.width
                        height: parent.height
                        model: myModel
                        delegate: AppIcon {}
                    }
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
    }

    Rectangle {
        id:appRunnable

        height:root.height
        width:root.width

        visible:false

        Rectangle {
            objectName: "appExec"
            anchors.fill: parent
        }

        Rectangle {
            id: backButtonBar
            width: parent.width
            height: Screen.desktopAvailableHeight*0.05
            anchors.bottom: parent.bottom
            color:"transparent"

            Button {
                text: "Voltar"
                height: parent.height
                width: parent.width/5
                onClicked: stackPages.pop();
            }
        }
    }
}
