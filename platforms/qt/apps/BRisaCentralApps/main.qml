import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3
import QtQuick.Window 2.0

import "qrc:/brisaComponents"
import "qrc:/components" as C
import "qrc:/components/functions.js" as JS

ApplicationWindow {
    id:appRoot

    property var dialog
    function destroyDialog() { dialog.destroy(); createAppBtn.wasClicked = false; console.log("DIALOG DESTROYED!"); }

    title: qsTr("BCA")
    width: 640
    height: 480
    visible: true

    onWidthChanged: gridApps.calculateColumnCount(width,height)
    onHeightChanged: console.log(width + " x " + height)

    C.Frame {
        anchors.fill: parent
        bottomBarHeightPercent: 0; topBarHeightPercent:8
        topBarColor: "#795548"
        topBarContent: Item {
            anchors.fill: parent
            Row {
                anchors.fill: parent
                C.ImageButton {
                    visible: stackPages.depth == 1
                    height:JS.hpercent(100,parent); width: height
                    source:"qrc:/img/refresh.png"; color: "transparent"
                    action.onClicked: {
                        manager.refreshAppList()
                        gridApps.model = manager.apps
                    }
                }
                C.ImageButton {
                    visible: stackPages.depth > 1
                    height:JS.hpercent(100,parent); width: height
                    source:"qrc:/img/back.png"; color: "transparent"
                    action.onClicked: stackPages.pop()
                }
            }
        }
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

                    function calculateColumnCount(w,h) {
                        if((w/h)>=1) gridApps.cellWidth = JS.wpercent(20,this);
                        else if((w/h)>=0.7) gridApps.cellWidth = JS.wpercent(25,this);
                        else if((w/h)>=0.5) gridApps.cellWidth = JS.wpercent(33,this);
                        else if((w/h)>=0.3) gridApps.cellWidth = JS.wpercent(50,this);
                        else gridApps.cellWidth = JS.wpercent(100,this);
                    }

                    anchors { fill : parent; margins: JS.wpercent(5,parent); leftMargin:JS.wpercent(7.5,parent) }
                    cellWidth: JS.wpercent(20,this); cellHeight: cellWidth
                    model:manager.apps; clip:true
                    delegate: BRisaApplication {
                        width:gridApps.cellWidth*0.75; height:width
                        iconPath:"file:///" + modelData.iconPath;
                        title:modelData.title;
                        description:modelData.description;
                        servicesModel:modelData.services
                        type:modelData.type
                    }
                }
                C.ImageButton {
                    id:createAppBtn
                    property bool wasClicked: false
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
                        if(!wasClicked) {
                            dialog = Qt.createComponent("qrc:/createApp/AppCreateDialog.qml");
                            dialog.createObject(parent).open();
                        }
                        wasClicked = true
                    }
                }
            }
        }
    }
    C.NotificationSystem {
        id:notificationSystem
        centralized: true; isOnTheTop: true
    }
}
