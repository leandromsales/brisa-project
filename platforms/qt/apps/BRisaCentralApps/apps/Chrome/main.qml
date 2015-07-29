import QtQuick 2.4
import QtQuick.Controls 1.3

import "." as Comp

Rectangle {
    width: 640
    height: 480
    visible: true

    color:"#ADA"

    Text{
        anchors.centerIn: parent
        font.pixelSize: 60
        font.bold: true

        color: "white"

        text:"Chrome"
    }
}
