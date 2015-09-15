import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0
import Qt.labs.folderlistmodel 2.1

Rectangle {
    id: settingsPage
    height:root.height
    width:root.width

    Rectangle {
        id: topBar
        width: parent.width
        anchors.top: parent.top
        height: root.height*0.05

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
        height: root.height*0.9
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
                    // folder: Qt.resolvedUrl("../BCU/files/Calculator"); CORRIGIR AQUI
                    showDirs: true
                    showDotAndDotDot: true
                }

                Component {
                    id: fileDelegate
                    Text {
                        text: "    "  + fileName + "\n"
                    }
                }
            }
        }
    }

    Rectangle {
        id: backButtonBar
        width: parent.width
        anchors.bottom: parent.bottom
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
