import QtQuick 2.3

Item {
    Image {
        id: image
        source: iconURL
        sourceSize.height: parent.height
        sourceSize.width: parent.width
        anchors.centerIn: parent;
    }

    Text {
        text: name
        width: parent.width
        anchors.top: image.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        fontSizeMode: Text.Fit
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

