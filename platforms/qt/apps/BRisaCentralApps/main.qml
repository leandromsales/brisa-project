import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3
import QtQuick.Window 2.0

import "qrc:/brisaComponents"
import "qrc:/components" as C
import "qrc:/components/functions.js" as JS

ApplicationWindow {
    id:appRoot

    property Component dialog
    function destroyDialog() { dialog.destroy(); console.log("DIALOG DESTROYED!"); }

    title: qsTr("BCA")
    width: 640
    height: 480
    visible: true

    C.Frame {
        anchors.fill: parent
        bottomBarHeightPercent: 0; topBarHeightPercent:10
        topBarColor: "#795548"
        content: StackView {
            id:stackPages
            function pushObject() {
                appRunnable.visible = true
                stackPages.push(appRunnable);
            }
            objectName: "stack"
            anchors.fill: parent
            initialItem: Rectangle {
                GridView {
                    id:gridApps
                    anchors { fill : parent; margins: JS.wpercent(5,parent); leftMargin:JS.wpercent(7.5,parent) }
                    cellWidth: JS.wpercent(20,this); cellHeight: cellWidth
                    model:manager.numOfApps;
                    delegate: BRisaApplication {
                        property var app: manager.apps[index]
                        width:JS.wpercent(15,parent); height:width
                        iconPath:app.iconPath;
                        title:app.title;
                        description:app.description;
                        servicesModel: app.services
                    }
                }
                C.ImageButton {
                    width: JS.minorThan(JS.wpercent(10,parent),JS.hpercent(10,parent)); height:width
                    radius:width/2
                    source:"qrc:/img/plus.png"; color: "#009688"
                    anchors {
                        bottom: parent.bottom
                        right: parent.right
                        bottomMargin: parent.height*(0.04)
                        rightMargin: parent.width*(0.04)
                    }
                    responsive: true; zoom: 1.5
                    action.onClicked : {
                        dialog = Qt.createComponent("qrc:/createApp/AppCreateDialog.qml");
                        dialog.createObject(parent).show();
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
