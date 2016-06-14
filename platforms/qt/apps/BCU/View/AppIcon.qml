import QtQuick 2.3

Rectangle {
    width: 90
    height: 90

    Image {
        id: image
        source: iconURL
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
        onClicked: {
            stackPages.push(appLoaderComponent)
            manager.run(appURL,name)
            dtS.name = name;
            dtS.info = info;
            dtS.iconURL = iconURL;
            dtS.appURL = appURL;
        }
    }

    Component {
        id:appLoaderComponent
        AppLoader {}
    }
}

