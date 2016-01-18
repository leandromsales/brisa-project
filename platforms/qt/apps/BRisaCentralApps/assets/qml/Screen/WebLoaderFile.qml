import QtQuick 2.0
import QtWebEngine 1.1

import "qrc:/components/functions.js" as JS

Item {
    objectName: "webInterpreter"
    property var webSource
    Connections {
        target:manager
        onWebSourceUrlSetted : navigator.url = webSource
    }

    WebEngineView {
        id:navigator
        anchors.fill: parent
    }
}

