import QtQuick 2.5
import QtQuick.Controls 1.4
import "qrc:/components" as C
import "qrc:/components/functions.js" as JS

Rectangle {
    id:root
    property alias iconPath : appIcon.source
    property alias title : appTitle.text
    property alias description : descriptionText.text
    property alias servicesModel : servicesTable.model
    property var type
    Item {
        id:item
        width: JS.wpercent(90,root); height: JS.hpercent(90,root)
        anchors.centerIn:parent
        Image {
            id: appIcon
            anchors { top : parent.top ;left : parent.left }
            width: JS.hpercent(25,parent); height: width
        }
        Item {
            anchors { top : appIcon.top; left : appIcon.right; rightMargin: JS.hpercent(15,parent) }
            width: JS.wpercent(50,parent); height: JS.hpercent(25,parent)
            Text {
                id: appTitle
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: JS.hpercent(25,parent); color: "#444"
                verticalAlignment: Text.AlignVCenter; horizontalAlignment: Text.AlignLeft
            }
            C.Button {
                anchors { top : appTitle.bottom; horizontalCenter: appTitle.horizontalCenter; margins: JS.hpercent(10,parent) }
                pixelSize: JS.hpercent(40,this); text:"Run"; textColor:"#444"
                width: JS.wpercent(50,parent)
                height: JS.hpercent(40,parent)
                action.onClicked: {
                    if(type==0) stackPages.push(qmlLoaderFileComponent)
                    else if(type==1) stackPages.push(webLoaderFileComponent)
                    manager.run(title,type)
                }
            }
        }
        Text {
            id: descriptionLabel
            anchors { top : appIcon.bottom; left: parent.left }
            text: "Description:"; color: "#444"
            font { underline: true; pixelSize: JS.hpercent(5,parent) }
        }
        ScrollView{
            id: descriptionScrollView
            width:JS.wpercent(100,parent); height:JS.hpercent(20,parent)
            frameVisible: true
            anchors { top : descriptionLabel.bottom; left: parent.left ; topMargin: JS.hpercent(2,parent) }
            contentItem:Text {
                id: descriptionText
                text: qsTr(description); wrapMode: Text.WordWrap
                font.pixelSize: JS.hpercent(3.33,root); color:"#222"
            }
        }
        Text {
            id: servicesLabel
            anchors { top : descriptionScrollView.bottom; left: parent.left; topMargin:JS.hpercent(2,parent) }
            text: qsTr("Services:"); font { underline: true; pixelSize: parent.height/20 } color:"#444"
        }
        TableView {
            id:servicesTable
            anchors { top : servicesLabel.bottom; left: parent.left; topMargin:JS.hpercent(2,parent) }
            width: JS.wpercent(100,parent); height:JS.hpercent(35,parent)
            TableViewColumn {
                role: "title"
                title: "Title"
                width: JS.wpercent(20,parent)
            }
            TableViewColumn {
                role: "description"
                title: "Description"
                width: JS.wpercent(70,parent)
            }
        }
    }
    Component {
        id:qmlLoaderFileComponent
        QMLLoaderFile {}
    }
    Component {
        id:webLoaderFileComponent
        WebLoaderFile {}
    }
}

