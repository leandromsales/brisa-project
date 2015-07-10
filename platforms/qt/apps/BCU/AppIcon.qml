import QtQuick 2.3

Rectangle {
    width: 90
    height: 90

    Image {
        id: image
        source: "qrc:/pics/qtlogo.png"
        sourceSize.height: 72
        anchors.centerIn: parent;
    }

    Text {
        text: name
        anchors.top: image.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
    
    MouseArea {
        anchors.fill: parent
        onClicked: loader.source = "AppOptions.qml"
    }
}


