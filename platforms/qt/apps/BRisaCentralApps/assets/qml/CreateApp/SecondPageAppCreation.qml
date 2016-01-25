import QtQuick 2.0
import QtQuick.Controls 1.4

import "qrc:/components" as C
import "qrc:/components/functions.js" as JS


Rectangle {
    Text {
        id:descriptionLabel
        anchors {
            top : parent.top; left: parent.left
            leftMargin: JS.wpercent(3,parent)
            topMargin: JS.hpercent(4,parent)
        }
        text:"Description:"; color:"#444"
        font.pixelSize: JS.hpercent(4,parent)
    }
    Rectangle {
        id:descriptionTextInputRect
        anchors {
            left:descriptionLabel.right; leftMargin: JS.wpercent(2,parent)
            right:parent.right; rightMargin: JS.wpercent(2,parent)
            bottom :nextButton.top; bottomMargin: JS.hpercent(5,parent)
            top:descriptionLabel.top
        }
        color:"#DDD"
        border { color: Qt.darker(color); width: 1 }
        TextInput {
            id:descriptionTextInput
            anchors { fill: parent; margins: JS.wpercent(1,parent) }
            wrapMode: TextEdit.Wrap; clip:true
            color: "#444"
            font.pixelSize: JS.hpercent(6,parent)
        }
    }
    C.Button {
        id:backButton
        anchors {
            left:descriptionTextInputRect.left;
            bottom:parent.bottom; bottomMargin: JS.hpercent(5,parent)
        }
        height: JS.hpercent(12,parent); width: JS.wpercent(18,parent)
        pixelSize: JS.hpercent(35,this); color:"#CDCDCD"
        textColor: "#212121"; bold: true; wave: true
        text:"Back"
        action.onClicked: {
            topBarFrame.stack.pop()
        }
    }
    C.Button {
        id:nextButton
        anchors {
            right:parent.right; bottom:parent.bottom
            rightMargin: JS.hpercent(5,parent); bottomMargin: JS.hpercent(5,parent)
        }
        height: JS.hpercent(12,parent); width: JS.wpercent(18,parent)
        pixelSize: JS.hpercent(35,this); color:"#4CAF50"
        textColor: "white"; bold: true; wave: true
        text:"Next"
        action.onClicked: {
            topBarFrame.stack.push(thirdPageAppCreationComponent);
        }
    }
    Component {
        id:thirdPageAppCreationComponent
        ThirdPageAppCreation {}
    }
}

