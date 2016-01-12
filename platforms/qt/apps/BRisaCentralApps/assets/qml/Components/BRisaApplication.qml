import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

import "qrc:/screen" as Screen

Image {

    id:imageApp

    property string iconPath
    property string title
    property string description
    property var servicesModel
    property ListModel list : ListModel{}

    width: parent.width/9
    height:width

    source: (manager.fileExists(iconPath.replace("file:///",""))) ? iconPath : "qrc:/assets/icon.png";
    antialiasing: true
    smooth: true

    Behavior on scale {
        NumberAnimation {
            duration: 100
        }
    }

    MouseArea{
        hoverEnabled: true
        anchors.fill:parent

        onClicked: {

            for(var i = 0; i < servicesModel.length; i++) {

                list.append({
                                "title": servicesModel[i],
                                "description" : servicesModel[++i]
                            });

            }

            stackPages.push(appPageComponent);

        }
        onEntered: {
            imageApp.scale = 1.4
        }
        onExited: {
            imageApp.scale = 1
        }
    }
    Component {

        id:appPageComponent

        Screen.BRisaApplicationView {
            iconPath: imageApp.iconPath
            title : imageApp.title
            description: imageApp.description
            servicesModel: list
        }
    }
}

