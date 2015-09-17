import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0
import Qt.labs.folderlistmodel 2.1

Rectangle {
    id: settingsPage
    width: 480 // Screen.desktopAvailableWidth
    height: 600 // Screen.desktopAvailableHeight*0.95

    Rectangle {
        id: topBar
        width: parent.width
        anchors.top: parent.top
        height: settingsPage.height*0.05

        Text {
            text: "Aplicativos Instalados"
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
        id: content
        width: parent.width
        height: settingsPage.height*0.9
        anchors.top: topBar.bottom

        Flickable {
            id: flickArea
            anchors.fill: parent
            contentWidth: parent.width
            contentHeight: parent.height
            boundsBehavior: Flickable.StopAtBounds
            flickableDirection: Flickable.VerticalFlick
            clip: true

            ListView {
                model: folderModel
                delegate: fileDelegate
                width: content.width
                height: content.height

                FolderListModel {
                    id: folderModel
                    folder: filesFolder
                    showDirs: true
                }

                Component {
                    id: fileDelegate

                    Rectangle {
                        height: nameApp.height*2
                        width: settingsPage.width

                        Text {
                            id: nameApp
                            text: fileName;
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            anchors.leftMargin: 20
                        }

                        Image {
                            source: "qrc:/pics/delete.png"
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            anchors.rightMargin: 20
                            sourceSize.height: nameApp.height*1.5

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    if (manager.deleteApp(nameApp.text.toString())) {
                                        loader.source = "okMessage.qml"
                                    } else {
                                        console.log("ERRO");
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        id: backButtonBar
        width: parent.width
        anchors.bottom: parent.bottom
        height: settingsPage.height*0.05

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
