import QtQuick 2.3
import QtQuick.Controls 1.2

Rectangle {
    Item {
        anchors.fill: parent
        Row {
            anchors.fill: parent
            Item {
                width: parent.width/2
                height: parent.height
                Column {
                    anchors { fill: parent; margins: parent.height*0.05 }
                    Text {
                        height:parent.height/6; width: parent.width
                        text:"Red"; color:text
                        font.pixelSize: height/2
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    Slider {
                        id:r
                        minimumValue: 0; maximumValue: 255
                        height:parent.height/6; width: parent.width
                    }
                    Text {
                        height:parent.height/6; width: parent.width
                        text:"Green"; color:text
                        font.pixelSize: height/2
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    Slider {
                        id:g
                        minimumValue: 0; maximumValue: 255
                        height:parent.height/6; width: parent.width
                    }
                    Text {
                        height:parent.height/6; width: parent.width
                        text:"Blue"; color:text
                        font.pixelSize: height/2
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    Slider {
                        id:b
                        minimumValue: 0; maximumValue: 255
                        height:parent.height/6; width: parent.width
                    }
                }
            }
            Item {
                width: parent.width/2
                height: parent.height
                Rectangle {
                    id:colorPreview
                    function rgbToHex(r, g, b) {
                        return "#" + ((1 << 24) + (r << 16) + (g << 8) + b).toString(16).slice(1);
                    }
                    anchors.centerIn: parent
                    width: parent.width*0.9; height: width
                    color: rgbToHex(r.value,g.value,b.value)
                }
            }
        }
    }
}
