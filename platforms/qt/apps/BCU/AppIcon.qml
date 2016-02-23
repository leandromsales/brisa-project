import QtQuick 2.3

Rectangle {
    height: 50
    width: parent.width

    Image {
        id: image
        source: iconURL
        sourceSize.height: 48
        anchors.left: parent.left
        anchors.leftMargin: 5
    }

    Text {
        text: "    " + name
        anchors.left: image.right
        anchors.verticalCenter: parent
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
