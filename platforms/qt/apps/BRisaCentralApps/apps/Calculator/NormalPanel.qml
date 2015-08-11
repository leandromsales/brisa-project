import QtQuick 2.0

Item {
    ListModel {
        id: btnModel

        ListElement {
            elements: [
                ListElement { name: "C" ; type:2 ; src:"" },
                ListElement { name: "1" ; type:0 ; src:"" },
                ListElement { name: "2" ; type:0 ; src:"" },
                ListElement { name: "3" ; type:0 ; src:"" }
            ]
        }
        ListElement {
            elements: [
                ListElement { name: "R" ; type:1 ; src:"" },
                ListElement { name: "4" ; type:0 ; src:"" },
                ListElement { name: "5" ; type:0 ; src:"" },
                ListElement { name: "6" ; type:0 ; src:"" }
            ]
        }
        ListElement {
            elements: [
                ListElement { name: "^" ; type:1 ; src:"" },
                ListElement { name: "7" ; type:0 ; src:"" },
                ListElement { name: "8" ; type:0 ; src:"" },
                ListElement { name: "9" ; type:0 ; src:"" }
            ]
        }
        ListElement {
            elements: [
                ListElement { name: "GEO" ; type:1 ; src:"" },
                ListElement { name: "." ; type:0 ; src:"" },
                ListElement { name: "0" ; type:0 ; src:"" },
                ListElement { name: "=" ; type:0 ; src:""  }
            ]
        }
    }


    ListModel {
        id: operatorsModel

        ListElement {
            elements: [
                ListElement { name: "+" ; type:1 ; src:""},
                ListElement { name: "-" ; type:1 ; src:"" },
                ListElement { name: "*" ; type:1 ; src:"" },
                ListElement { name: "/" ; type:1 ; src:"" }
            ]
        }
    }

    Component {

        id: btnDelegate

        Row{

            //            spacing:2

            Repeater {
                model: elements
                PrimaryBtn {
                    texto: name
                    tipo:type
                    imageSource:src
                    width: appWindow.width/4
                    height: appWindow.height/8
                }
            }
        }
    }

    Component {

        id: operatorDelegate

        Repeater {
            model: elements
            PrimaryBtn {
                texto: name
                tipo:type
                imageSource:src
                width: appWindow.width/4
                height: appWindow.height/8
            }
        }
    }

    Rectangle {

        anchors.top: parent.top
        anchors.topMargin: appWindow.height/4

        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom

        color:"transparent"

        Column {
            id:buttons
            anchors.fill: parent

            spacing:2

            Repeater {
                model:btnModel
                delegate: btnDelegate
            }
            Row{
                //            spacing:2
                id:operators
                Repeater {
                    model:operatorsModel
                    delegate: operatorDelegate
                }
            }
        }

        GeometryPanel{
            id:geometryPanel

            panelY: buttons.anchors.topMargin
            panelSpacing: buttons.spacing

            visible: false
        }
    }
}
