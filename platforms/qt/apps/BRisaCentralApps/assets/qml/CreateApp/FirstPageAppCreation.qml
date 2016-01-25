import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1

import "qrc:/components" as C
import "qrc:/components/functions.js" as JS

Rectangle {
    Column {
        id:mainColumn
        anchors {
            top: parent.top; right:parent.right;
            left:parent.left; margins: JS.hpercent(10,parent)
        }
        height: JS.hpercent(50,parent)
        spacing: JS.hpercent(8,parent)
        Row {
            width:JS.wpercent(90,parent); height: JS.hpercent(15,parent)
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: JS.wpercent(5,parent)
            C.TextLine {
                id:nameAppTextField
                width:JS.wpercent(65,parent); height: JS.hpercent(100,parent)
                anchors.verticalCenter: parent.verticalCenter
                placeholder: "Name";
            }
            GroupBox {
                title: "App Type"
                RowLayout {
                    ExclusiveGroup {
                        id: appTypeGroup
                        onCurrentChanged: {
                            if(current.text == "Web")
                                mainQMLtextLine.text = "https://"
                            else
                                mainQMLtextLine.text = ""
                        }
                    }
                    RadioButton {
                        text: "Web"
                        exclusiveGroup: appTypeGroup
                    }
                    RadioButton {
                        text: "QML"
                        checked: true
                        exclusiveGroup: appTypeGroup
                    }
                }
            }
        }
        Row {
            width:JS.wpercent(90,parent); height: JS.hpercent(15,parent)
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: JS.wpercent(5,parent)
            C.TextLine {
                id:iconTextLine
                width:JS.wpercent(85,parent); height: JS.hpercent(100,parent)
                anchors.verticalCenter: parent.verticalCenter
                placeholder: "Icon"
            }
            C.ImageButton {
                width: JS.wpercent(10,parent); height: width
                source: "qrc:/img/folder.png"; radius: width/2
                color: "transparent"; responsive: true
                action.onClicked: fileDialog.open()
            }
        }
        Row {
            width:JS.wpercent(90,parent); height: JS.hpercent(15,parent)
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: JS.wpercent(5,parent)
            C.TextLine {
                id:mainQMLtextLine
                width:appTypeGroup.current.text == "QML"?JS.wpercent(85,parent):JS.wpercent(100,parent)
                height: JS.hpercent(100,parent)
                anchors.verticalCenter: parent.verticalCenter
                placeholder: appTypeGroup.current.text == "QML"?"Main QML":"WebSite";
            }
            C.ImageButton {
                visible:appTypeGroup.current.text == "QML"
                width: JS.wpercent(10,parent); height: width
                source: "qrc:/img/folder.png"; radius: width/2
                color: "transparent"; responsive: true
                action.onClicked: folderDialog.open()
            }
        }
    }
    Item {
        anchors {
            right:parent.right; left:parent.left
            top:mainColumn.bottom; bottom: parent.bottom
            margins: JS.hpercent(5,parent); leftMargin: JS.wpercent(10,parent)
        }
        Image {
            id:imgPreview
            height: JS.hpercent(100,parent); width: height
            source: "qrc:/img/icon.png"
            anchors.verticalCenter: parent.verticalCenter
            fillMode: Image.PreserveAspectFit
        }
        C.Button {
            text: "Next"
            anchors { right:parent.right; bottom:parent.bottom }
            height: JS.hpercent(40,parent); width: JS.wpercent(20,parent)
            pixelSize: JS.hpercent(35,this); color:"#4CAF50"
            textColor: "white"; bold: true; wave: true
            action.onClicked: {
                if(nameAppTextField.text.length != 0 &&
                        iconTextLine.text.length != 0 &&
                        mainQMLtextLine.text.length != 0)
                    topBarFrame.stack.push(secondPageComponent)
                else
                    notificationSystem.coloredNotify("Fill all the fields","Red")
            }
        }
    }
    //Components
    Component {
        id:secondPageComponent
        SecondPageAppCreation {}
    }
    //Dialogs
    FileDialog {
        id:fileDialog
        title:"Choose an Icon to your App"
        selectFolder: false; selectMultiple: false
        nameFilters: [ "Image files (*.png)", "All files (*)" ]
        onAccepted: {
            iconTextLine.text = fileUrl
            iconTextLine.edited = true
            imgPreview.source = fileUrl
        }
    }
    FileDialog {
        id:folderDialog
        title:"Select the main QML of your Application"
        selectFolder: false; selectMultiple: false
        nameFilters: [ "QML files (*.qml)", "All files (*)" ]
        onAccepted: {
            mainQMLtextLine.text = folder
            mainQMLtextLine.edited = true
        }
    }
}

