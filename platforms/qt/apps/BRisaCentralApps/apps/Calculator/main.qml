import QtQuick 2.4
import QtQuick.Controls 1.3

import "." as Comp

Rectangle {
    id: appWindow
	anchors.fill:parent

    color: "black"

    Rectangle {

        anchors.bottomMargin: 2.26*parent.height/3
        anchors.bottom: parent.bottom

        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top

        color:"transparent"

        Text{
            id:textDisplay
            text:""

            anchors.bottom: parent.bottom
            anchors.right: parent.right

            color:"white"
            font.pixelSize: (parent.height/4) - text.length*1.4
        }
    }

    Comp.NormalPanel{ id:normalPanel }
    Comp.ComputacaoPanel{ id:compPanel  ; visible:false}

}




