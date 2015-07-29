import QtQuick 2.0

Rectangle {

    property int panelY
    property int panelSpacing

    color:"transparent"

    ListModel {
        id:listBtns

        ListElement{ name:"Sen" }
        ListElement{ name:"Cos" }
        ListElement{ name:"Tan" }
    }

    Column {

        y:panelY
        spacing: panelSpacing

        Repeater {
            model:listBtns
            delegate: GeometryBtn {
                texto: name
                tipo:1
                imageSource:""
                width: appWindow.width/4
                height: appWindow.height/8
            }
        }
    }
}

