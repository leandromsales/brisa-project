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

                appFrame.iconPath=iconPath
                appFrame.title=title
                appFrame.description=description

                var i;
                for(i = 0; i < servicesModel.length; i++) {

                    list.append({
                                    "title": servicesModel[i],
                                    "description" : servicesModel[++i]
                                });

                }

                appFrame.servicesModel=list
                mainRoot.state="app"

            }
            onEntered: {

                root.width=layout.width/5
                root.height=layout.height/4

            }
            onExited: {

                root.width=layout.width/7
                root.height=layout.height/6

            }
        }
    }
}

