import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

ApplicationWindow {
    id:appRoot
    title: qsTr("BCA")
    width: 640
    height: 480
    visible: true

    StackView {
        id:stackPages
        objectName: "stack"

        anchors.fill: parent

        function pushObject() {
            appRunnable.visible = true
            stackPages.push(appRunnable);
        }

        initialItem: Rectangle {

            Grid {
                id:gridApps
                columns:4
                spacing:width/15

                anchors.fill : parent
                anchors.margins: parent.width/20

                Repeater {
                    model:manager.getNumOfApps();

                    delegate: BRisaApplication {

                        iconPath:manager.getListApps()[index].getIconPath();
                        title:manager.getListApps()[index].getTitle();
                        description:manager.getListApps()[index].getDescription();

                        servicesModel: manager.getListApps()[index].getString();

                    }
                }
            }
        }
    }

    Rectangle {
        id:appRunnable

        color:"transparent"

        height:parent.height
        width:parent.width

        visible:false

        Rectangle{
            objectName: "appExec"
            anchors.fill: parent
        }

        Rectangle {
            id:bottomBar
            width:parent.width
            height:parent.height/10

            color:"transparent"

            anchors.bottom: parent.bottom
            Button{
                text:"Back"

                height:parent.height
                width:parent.width/5

                onClicked: stackPages.pop();
            }
        }
    }
}
