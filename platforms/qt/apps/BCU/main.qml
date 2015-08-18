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

                Rectangle {
                    height: 1
                    width: parent.width
                    anchors.bottom: parent.top

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
            id: topBarApp
            width: parent.width
            anchors.top: parent.top
            height: Screen.desktopAvailableHeight*0.05

            Text {
                text: "Aplicativo " + dtS.name
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
            objectName: "appExec"
            width: root.width
            height: Screen.desktopAvailableHeight*0.9
            anchors.top: topBarApp.bottom
            color: "white"
        }

        Rectangle {
            id: backButtonBar
            width: parent.width
            anchors.bottom: parent.bottom
            height: Screen.desktopAvailableHeight*0.05

            Rectangle {
                height: 1
                width: parent.width
                anchors.bottom: parent.top

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

            Text {
                text: "Voltar"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            MouseArea {
                anchors.fill: parent
                onClicked: stackPages.pop();
            }
        }
    }

}
