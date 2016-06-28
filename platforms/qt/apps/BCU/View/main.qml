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
        id:loader
        visible: false
        anchors.fill: parent
    }

    StackView {
        id:stackPages
        anchors.fill: parent
        initialItem: inicialPage

        Column {
            id: inicialPage

            Rectangle {
                id: topBar
                width: parent.width; height: root.height*0.05
                z:1

                Text {
                    text: "Dispositivos disponíveis"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                }

                Rectangle {
                    height: 1
                    width: parent.width

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
                width: parent.width; height: root.height*0.9
                GridView {
                    id: grid                    
                    property int columns : 5
                    property int rows : 5
                    property real verticalSpacing : parent.height*0.05
                    property real horizontalSpacing : parent.width*0.05
                    anchors {
                        fill:parent; leftMargin: horizontalSpacing; topMargin: verticalSpacing
                    }
                    cellWidth: width/columns
                    cellHeight: height/rows
                    model: manager.apps
                    delegate: AppIcon {
                        width:grid.cellWidth - grid.horizontalSpacing
                        height: grid.cellHeight - grid.verticalSpacing
                    }
                }
            }

            Rectangle {
                id: statusBar
                width: parent.width
                height: root.height*0.05

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


                Rectangle {
                    id: exit
                    width: parent.width*0.2
                    anchors.left: statusBar.left
                    anchors.verticalCenter: parent.verticalCenter

                    Image {
                        source: "qrc:/pics/quit.png"
                        sourceSize.height: backButtonBar.height*0.6
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter

                        MouseArea {
                            anchors.fill: parent
                            onClicked: Qt.quit();
                        }
                    }
                }

                Rectangle {
                    id: searching
                    width: parent.width*0.6
                    anchors.left: exit.right
                    anchors.verticalCenter: parent.verticalCenter

                    Text {
                        text: "Pesquisando por outros dispositivos"
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }

                Rectangle {
                    id: settings
                    width: parent.width*0.2
                    anchors.left: searching.right
                    anchors.verticalCenter: parent.verticalCenter

                    Image {
                        source: "qrc:/pics/delete.png"
                        sourceSize.height: backButtonBar.height*0.6
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter

                        MouseArea {
                            anchors.fill: parent
                            onClicked: stackPages.push("qrc:/Settings.qml");
                        }
                    }
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
            height: root.height*0.9
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

            Image {
                source: "qrc:/pics/back.png"
                sourceSize.height: backButtonBar.height*0.6
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
