import QtQuick 2.3

Rectangle {
    height: 50
    width: parent.width

    Image {
        id: image
        source: iconURL
        sourceSize.height: 42
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 5
    }

    Text {
        text: "    " + name
        anchors.left: image.right
        anchors.verticalCenter: parent.verticalCenter
    }
    
    MouseArea {
        anchors.fill: parent
        onClicked: {
            loader.source = "AppOptions.qml"
            dtS.name = name;
            dtS.info = info;
            dtS.iconURL = iconURL;
            dtS.appURL = appURL;
            console.log (iconURL);
            console.log (dtS.iconURL);
        }
    }
}
