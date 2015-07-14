import QtQuick 2.4
import QtQuick.Controls 1.3

ApplicationWindow {
    id:appRoot
    title: qsTr("BCA")
    width: 640
    height: 480
    visible: true

    Rectangle {
        id:mainRoot

        anchors.fill: parent

        states: [
            State {
                name: "app"
                PropertyChanges {
                    target: menuFrame
                    visible:false
                }
            },
            State {
                name: "menu"
                PropertyChanges {
                    target: menuFrame
                    visible:true
                }
            }
        ]

        state:"menu"


        Rectangle {
            id:appFrame
            anchors.fill: parent

            property string iconPath
            property string title
            property string description
            property var servicesModel

            BRisaApplicationView {
                iconPath: appFrame.iconPath
                title : appFrame.title
                description: appFrame.description
                servicesModel: appFrame.servicesModel
            }
        }

        Rectangle {
            id:menuFrame
            anchors.fill: parent

            Grid {
                id:layout
                columns:6
                anchors.fill: parent
                anchors.margins: 5

                spacing:10

                Repeater {
                    model:manager.getNumOfApps();

                    delegate: BRisaApplication {

                        height: appRoot.height/6
                        width: appRoot.width/7

                        iconPath:manager.getListApps()[index].getIconPath();
                        title:manager.getListApps()[index].getTitle();
                        description:manager.getListApps()[index].getDescription();

                        servicesModel: manager.getListApps()[index].getString();

                    }
                }
            }
        }
    }
}
