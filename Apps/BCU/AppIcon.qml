import QtQuick 2.3

Rectangle {
    width: 75
    height: 75

    Image {
        id: image
        source: "qrc:/pics/qtlogo.png"
        sourceSize.height: 72
        anchors.centerIn: parent;
    }
    
    MouseArea {
        anchors.fill: parent
        onClicked: loader.source = "AppOptions.qml"
    }
}


