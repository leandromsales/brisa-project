import QtQuick 2.0

Item {
    objectName: "qmlInterpreter"
    property var mainQMLSource
    Connections {
        target:manager
        onMainQMLPathSetted : qmlLoader.source = mainQMLSource
    }

    Loader {
        id:qmlLoader
        anchors.fill: parent
    }
}

