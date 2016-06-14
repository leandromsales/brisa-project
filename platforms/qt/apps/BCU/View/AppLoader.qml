import QtQuick 2.0

Item {
    objectName: "appLoader"
    property var appPath
    Connections {
        target:manager
        onDecompressed: {
            console.log("++++++++++++++++++++++++++++++++++++++++++++++++++++")
            console.log("++++++++++++++++++++++++++++++++++++++++++++++++++++")
            console.log(appPath)
            console.log("++++++++++++++++++++++++++++++++++++++++++++++++++++")
            console.log("++++++++++++++++++++++++++++++++++++++++++++++++++++")
            qmlLoader.setSource(appPath)
            console.log("DECOMPRESS RECEIVED!")
        }
    }
    Loader {
        id:qmlLoader
        anchors.fill: parent
    }
}
