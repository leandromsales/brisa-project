import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2

import "qrc:/components" as C

Rectangle {
    Text {
        id:nameAppLabel
        anchors {
            top : parent.top
            left: parent.left
            leftMargin: parent.width/20
            topMargin: parent.height/25
        }

        text:"Name of the App:"
        color:"#444"

        font.pixelSize: parent.height/20
    }

    TextField {
        id:nameAppTextField

        font {
            pixelSize: height*(0.6)
        }

        height: nameAppLabel.contentHeight
        textColor: "#444"

        anchors{
            right: parent.right
            rightMargin: parent.width*(0.05)
            top : parent.top
            left: nameAppLabel.right
            leftMargin: parent.width/20
            topMargin: parent.height/25
        }
    }

    Text {
        id:iconLabel
        anchors {
            top : nameAppLabel.bottom
            left: parent.left
            leftMargin: parent.width/20
            topMargin: parent.height/25
        }

        text:"Icon:"
        color:"#444"

        font.pixelSize: parent.height/20
    }

    TextField {
        id:iconTextField

        font {
            pixelSize: height*(0.6)
        }

        height: iconLabel.contentHeight
        textColor: "#444"

        anchors{
            right: fileDialogImage.right
            rightMargin: parent.width*(0.05)
            top : nameAppLabel.bottom
            left: nameAppLabel.right
            leftMargin: parent.width/20
            topMargin: parent.height/25
        }
    }

    Image {
        id:fileDialogImage
        anchors {
            right: parent.right
            rightMargin: parent.width/40

            verticalCenter: iconTextField.verticalCenter
        }

        width:parent.width/25
        height: width
        source: "qrc:/img/folder.png"

        MouseArea {
            anchors.fill: parent
            onClicked: fileDialog.open()
        }
    }

    Text {
        id:folderLabel
        anchors {
            top : iconLabel.bottom
            left: parent.left
            leftMargin: parent.width/20
            topMargin: parent.height/25
        }

        text:"App Folder:"
        color:"#444"

        font.pixelSize: parent.height/20
    }

    TextField {
        id:folderTextField

        font {
            pixelSize: height*(0.6)
        }

        height: folderLabel.contentHeight
        textColor: "#444"

        anchors{
            right: folderDialogImage.right
            rightMargin: parent.width*(0.05)
            top : iconLabel.bottom
            left: nameAppLabel.right
            leftMargin: parent.width/20
            topMargin: parent.height/25
        }
    }

    Image {
        id:folderDialogImage
        anchors {
            right: parent.right
            rightMargin: parent.width/40

            verticalCenter: folderTextField.verticalCenter
        }

        width:parent.width/25
        height: width
        source: "qrc:/img/folder.png"

        MouseArea {
            anchors.fill: parent
            onClicked: folderDialog.open()
        }
    }

    Image {
        id:imgPreview
        anchors {
            top: folderTextField.bottom
            topMargin: parent.height/8

            bottom:parent.bottom
            right:parent.right
            left:parent.left

            bottomMargin: parent.height/8
            rightMargin: parent.width*(0.75)
            leftMargin: parent.width*(0.05)
        }

        source: "qrc:/img/icon.png"
        fillMode: Image.PreserveAspectFit
    }

    C.Button {
        anchors {
            right:parent.right
            bottom:parent.bottom

            rightMargin:parent.width/20
            bottomMargin: parent.height/25
        }

        text: "Next"

        action.onClicked: {
            textTopBar.text = "Create an App (2/3)"
            centerRectStackPages.push(secondPageComponent)
        }
    }

    Component.onCompleted: textTopBar.text = "Create an App (1/3)"

    //Components
    Component {
        id:secondPageComponent
        SecondPageAppCreation {}
    }

    //Dialogs
    FileDialog {
        id:fileDialog

        title:"Choose an Icon to your App"

        selectFolder: false
        selectMultiple: false
        nameFilters: [ "Image files (*.png)", "All files (*)" ]

        onAccepted: {
            iconTextField.text = fileUrl
            imgPreview.source = fileUrl
        }
    }
    FileDialog {
        id:folderDialog

        title:"Select the main QML of your Application"

        selectFolder: false
        selectMultiple: false

        nameFilters: [ "QML files (*.qml)", "All files (*)" ]

        onAccepted: {
            folderTextField.text = folder
        }
    }
}

