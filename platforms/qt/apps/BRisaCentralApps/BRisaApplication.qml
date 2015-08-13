import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

Rectangle {

    id:root
    property string iconPath: "qrc:/image/icon.png"
    property string title
    property string description
    property var servicesModel
    property ListModel list : ListModel{}

    color:"transparent"

    Image {
        anchors.fill:parent
        source: iconPath
        antialiasing: true
        smooth: true

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
                root.scale = 1.5
            }
            onExited: {
                root.scale = 1
            }
        }
    }

    Component {

        id:appPageComponent

        Rectangle {
            property string iconPath
            property string title
            property string description
            property var servicesModel

            BRisaApplicationView {
                iconPath: root.iconPath
                title : root.title
                description: root.description
                servicesModel: list
            }
        }
    }
}

