import QtQuick 2.0
import QtQuick.Controls 1.2

import "qrc:/components"

Rectangle {

    TextField {
        id:titleTextField
        placeholderText: "Title"

        height: addButton.height
        font.pixelSize: height/2

        anchors {
            top : parent.top
            left: parent.left
            right:serviceTable.left
            leftMargin: parent.width/20
            topMargin: parent.height/25
        }
    }
    TextField {
        id:descriptionTextField
        placeholderText: "Description"

        height: addButton.height
        font.pixelSize: height/2

        anchors {
            top : parent.top
            right:addButton.left
            rightMargin: parent.width/80
            left:titleTextField.right
            leftMargin: parent.width/80
            topMargin: parent.height/25
        }
    }

    CommonButton {
        id:addButton
        btnText: "Add"
        anchors {
            top : parent.top
            right: parent.right
            rightMargin: parent.width/20
            topMargin: parent.height/25
        }

        mouseArea.onClicked: {
            if(titleTextField.text != "" && descriptionTextField.text != "") {
                serviceTableListModel.append({
                                                 title:titleTextField.text,
                                                 description:descriptionTextField.text
                                             });
                titleTextField.text = "";
                descriptionTextField.text = "";
            }
        }
    }

    Text {
        id:serviceLabel
        anchors {
            top : addButton.bottom
            left: parent.left
            leftMargin: parent.width/20
            topMargin: parent.height/25
        }

        text:"Services:"
        color:"#444"

        font.pixelSize: parent.height/20
    }

    TableView {
        id:serviceTable
        anchors {
            top:addButton.bottom
            topMargin: parent.height/25
            left:serviceLabel.right
            leftMargin: parent.width/20
            right:parent.right
            rightMargin: parent.width/20
            bottom:createButton.top
            bottomMargin: parent.height/25
        }
        model:ListModel {   id:serviceTableListModel  }

        TableViewColumn {
            role: "title"
            title: "Title"
            width: serviceTable.width/3
        }
        TableViewColumn {
            role: "description"
            title: "Description"
            width: serviceTable.width*(0.658)
        }
    }

    CommonButton {
        id:backButton
        anchors {
            left:parent.left
            bottom:parent.bottom
            leftMargin: parent.width/20
            bottomMargin: parent.height/25
        }

        btnText:"Back"

        mouseArea.onClicked: {
            textTopBar.text = "Create an App (2/3)"
            centerRectStackPages.pop()
        }
    }

    CommonButton {
        id:createButton
        anchors {
            right:parent.right
            bottom:parent.bottom
            rightMargin: parent.width/20
            bottomMargin: parent.height/25
        }

        btnText:"Create"

        mouseArea.onClicked: {

            var theApp = {};
            theApp.name = nameAppTextField.text;
            theApp.icon = iconTextField.text.replace("file:///","");
            theApp.mainPath = folderTextField.text.replace("file:///","");
            theApp.description = descriptionTextArea.text;
            theApp.serviceTitles = [];
            theApp.serviceDescriptions = [];

            //Services
            var list = serviceTableListModel;

            for(var i = 0; i < list.count; i++) {
                theApp.serviceTitles.push(list.get(i).title);
                theApp.serviceDescriptions.push(list.get(i).description);
            }

            manager.createAnApp(theApp);
        }
    }
}

