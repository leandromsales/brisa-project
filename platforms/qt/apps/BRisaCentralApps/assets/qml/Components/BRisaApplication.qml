import QtQuick 2.5
import QtQuick.Controls 1.4

import "qrc:/screen"

Image {
    id:imageApp
    property string iconPath
    property string title
    property string description
    property var servicesModel
    source: (manager.fileExists(iconPath.replace("file:///",""))) ? iconPath : "qrc:/img/icon.png";
    antialiasing: true; smooth: true; scale: mouse.containsMouse ? 1.4 : 1
    Behavior on scale { NumberAnimation { duration: 100 } }
    MouseArea{
        id:mouse
        hoverEnabled: true
        anchors.fill:parent
        onClicked: stackPages.push(appPageComponent);
    }
    Component {
        id:appPageComponent
        BRisaApplicationView {
            iconPath: imageApp.iconPath
            title : imageApp.title
            description: imageApp.description
            servicesModel: imageApp.servicesModel
        }
    }
}

