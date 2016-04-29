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
            manager.run(appURL,name)
            stackPages.push(appLoaderComponent)
            dtS.name = name;
            dtS.info = info;
            dtS.iconURL = iconURL;
            dtS.appURL = appURL;
            console.log (iconURL);
            console.log (dtS.iconURL);
        }
    }

    Component {
        id:appLoaderComponent
        AppLoader {}
    }
}

