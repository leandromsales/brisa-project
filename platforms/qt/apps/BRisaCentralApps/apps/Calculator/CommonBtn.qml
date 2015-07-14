import QtQuick 2.0

Rectangle {
    id:btn

    property int tipo
    property string texto
    property string imageSource

    property alias mouseArea:mouse

    color:translateColor(tipo)
//    radius:200

    Loader {
        id:loader

        anchors.centerIn: parent
        sourceComponent: textOrImage(imageSource)
    }

    Component {
        id:textComponent

        Text {
            anchors.centerIn: parent
            font.pixelSize: btn.height*(0.7)
            font.bold: true
            text:texto
        }
    }

    Component{
        id:imageComponent

        Image {
            anchors.centerIn: parent

            width: smaller(btn.height, btn.width)
            height: smaller(btn.height, btn.width)

            scale:0.8
            source:imageSource
        }
    }
    MouseArea {
        id:mouse

        anchors.fill:parent

        onPressed: {
            btn.color=Qt.lighter(btn.color, 1.3);
        }

        onReleased: {
            btn.color=translateColor(tipo)
        }
    }
    function translateColor(tipo) {

        switch(tipo){

        case 0:
            return "#FFB833"
        case 1:
            return "lightblue"
        case 2:
            return "red"
        }
    }
    function smaller(height, width) {

        if(height > width)
            return width
        return height
    }
    function textOrImage(source) {
        if(source === "") {
            return textComponent
        } else {
            return imageComponent
        }
    }
}



