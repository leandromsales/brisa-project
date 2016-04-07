import QtQuick 2.5
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.4

import "qrc:/components" as C
import "qrc:/components/functions.js" as JS

Rectangle {

    Row {
        id:mainRow
        anchors { top:parent.top; left:parent.left; right:parent.right; margins: JS.hpercent(8,parent) }
        width: JS.wpercent(100,parent); height: JS.hpercent(10,parent)
        spacing: JS.wpercent(5,parent)
        C.TextLine {
            id:titleTextField
            placeholder: "Title"; anchors.verticalCenter: parent.verticalCenter
            height: JS.hpercent(80,parent); width: JS.wpercent(30,parent)
        }
        C.TextLine {
            id:descriptionTextField
            placeholder: "Description"; anchors.verticalCenter: parent.verticalCenter
            height: JS.hpercent(80,parent); width: JS.wpercent(50,parent)
        }
        C.ImageButton {
            id:addButton
            height: JS.hpercent(120,parent); width: height
            source: "qrc:/img/plus.png"; radius: width/6;
            color:"#545454"
            action.onClicked: {
                if(titleTextField.text != "" && descriptionTextField.text != "") {
                    var item = Object();
                    item.title = titleTextField.text
                    item.description = descriptionTextField.text
                    serviceTableListModel.append(item);
                    titleTextField.text = "";
                    descriptionTextField.text = "";
                }
            }
        }
    }
    Text {
        id:serviceLabel
        anchors {
            top : mainRow.bottom; left: parent.left
            leftMargin: JS.wpercent(5,parent); topMargin: JS.hpercent(4,parent)
        }
        text:"Services:"; color:"#444"
        font.pixelSize: parent.height/20
    }
    TableView {
        id:serviceTable
        alternatingRowColors: false
        anchors {
            top:mainRow.bottom; topMargin: JS.hpercent(4,parent)
            left:serviceLabel.right; leftMargin: JS.wpercent(5,parent)
            right:parent.right; rightMargin: JS.wpercent(5,parent)
            bottom:createButton.top; bottomMargin: JS.hpercent(5,parent)
        }
        model:ListModel { id:serviceTableListModel }
        headerDelegate: Rectangle {
            height: textItem.implicitHeight * 1.5
            width: textItem.implicitWidth
            color: "#2196F3"
            Text {
                id: textItem
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: styleData.value
                color: "White"
                font { bold: true; pixelSize: JS.hpercent(8,serviceTable) }
            }
            Rectangle {
                anchors {
                    right: parent.right; top: parent.top; bottom: parent.bottom
                    bottomMargin: 1; topMargin: 1
                }
                width: 1; color: "#ccc"
            }
        }
        itemDelegate:  Item {
            Text {
                anchors { verticalCenter: parent.verticalCenter; horizontalCenter: parent.horizontalCenter }
                text: styleData.value; color: "#444"
                font.pixelSize: JS.hpercent(7,serviceTable)
            }
        }
        TableViewColumn {
            role: "title"; title: "Title"
            width: JS.wpercent(33,serviceTable)
        }
        TableViewColumn {
            role: "description"; title: "Description"
            width: JS.wpercent(66,serviceTable)
        }
    }
    C.Button {
        id:backButton
        anchors {
            left:serviceTable.left; bottom:parent.bottom
            bottomMargin: JS.hpercent(4,parent)
        }
        height: JS.hpercent(12,parent); width: JS.wpercent(18,parent)
        pixelSize: JS.hpercent(35,this); color:"#CDCDCD"
        textColor: "#212121"; bold: true; wave: true
        text:"Back"; action.onClicked: topBarFrame.stack.pop()
    }
    C.Button {
        id:createButton
        anchors {
            right:parent.right; bottom:parent.bottom
            rightMargin: JS.wpercent(5,parent); bottomMargin: JS.hpercent(4,parent)
        }
        height: JS.hpercent(12,parent); width: JS.wpercent(18,parent)
        pixelSize: JS.hpercent(35,this); color:"#4CAF50"
        textColor: "white"; bold: true; wave: true
        text:"Create"
        action.onClicked: {
            var theApp = {};
            theApp.name = nameAppTextField.text;
            theApp.icon = iconTextLine.text.length != 0 ?
                        iconTextLine.text.replace("file:///","") :
                        "";
            theApp.appType = appTypeGroup.current.text;
            theApp.execPath = theApp.appType == "QML" ?
                        mainQMLtextLine.text.replace("file:///","") : mainQMLtextLine.text;
            theApp.description = descriptionTextInput.text;
//            theApp.services = [];
//            //Services
//            var list = serviceTableListModel;
//            for(var i = 0; i < list.count; i++) {
//                var service = {};
//                service.title = list.get(i).title;
//                service.description = list.get(i).description;
//                theApp.services.push(service);
//            }
            manager.createAnApp(theApp);
            notificationSystem.coloredNotify("App created with Sucess","Green");
            closed();
        }
    }
}

