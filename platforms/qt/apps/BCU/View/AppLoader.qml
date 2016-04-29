import QtQuick 2.0

Item {
    objectName: "appLoader"
    property var appPath
    Connections {
        target:manager
        onDecompressed: {
            qmlLoader.setSource(appPath)
            console.log("DECOMPRESS RECEIVED!")
        }
    }
    Loader {
        id:qmlLoader
        anchors.fill: parent
    }
}
