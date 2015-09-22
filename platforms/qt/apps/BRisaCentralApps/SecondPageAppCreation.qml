import QtQuick 2.0
import QtQuick.Controls 1.4

import "qrc:/components"


Rectangle {

    Text {
        id:descriptionLabel
        anchors {
            top : parent.top
            left: parent.left
            leftMargin: parent.width/20
            topMargin: parent.height/25
        }

        text:"Description:"
        color:"#444"

        font.pixelSize: parent.height/20
    }

    TextArea {
        id:descriptionTextArea
        anchors {
            left:descriptionLabel.right
            leftMargin: parent.width/20
            right:parent.right
            rightMargin: parent.width/20
            top:descriptionLabel.top
            bottom :nextButton.top
            bottomMargin: parent.height/25
        }

        wrapMode: TextEdit.WrapAtWordBoundaryOrAnywhere
        font.pixelSize: parent.height/25
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
            textTopBar.text = "Create an App (1/3)"
            centerRectStackPages.pop()
        }
    }

    CommonButton {
        id:nextButton
        anchors {
            right:parent.right
            bottom:parent.bottom
            rightMargin: parent.width/20
            bottomMargin: parent.height/25
        }

        btnText:"Next"

        mouseArea.onClicked: {
            textTopBar.text = "Create an App (3/3)"
            centerRectStackPages.push(thirdPageAppCreationComponent);
        }
    }

    Component {
        id:thirdPageAppCreationComponent
        ThirdPageAppCreation {}
    }

}

