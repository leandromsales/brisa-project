import QtQuick 2.0
import "globals.js" as Globals

Rectangle {
    id: mygrid
    property color backgroundColor: "#000000"
    property bool isMediaServer: false
    property int selectedItem: -1

    signal selectionChanged(string newtext)
    signal selectionChangedSendUUID(string uuid)

    function setModel(newModel) {
        grid.model = newModel
    }
    function setFrame(x, y, width, height){
        grid.x = x
        grid.y = y
        grid.width = width
        grid.height = height
    }
    ListView {
        id: grid
        property int size: 0
        focus: true
        clip: true
        Component.onCompleted: {
            grid.currentIndex = -1;
        }
        Text {
            id: noDevices
            text: "No Devices"
            anchors {
                centerIn: parent
            }
            Behavior on opacity {
                PropertyAnimation{duration: 200}
            }
            font.family: "helvetica"
            font.pixelSize: 20
            color: "#505050"
            states: State {
                name: "selected"
                when: (grid.size > 0)
                PropertyChanges {
                    target: noDevices
                    opacity: 0
                }
            }
        }
        Rectangle {
            z: -1
            width: parent.width
            height: parent.height
            color : backgroundColor
        }
        delegate: Item {
            id: gridItem
            width: 150
            height: 60
            Component.onCompleted: {

            }
            Image {
                id: myIcon
                anchors.verticalCenter: gridItem.verticalCenter
                width: 26
                height: 26
                fillMode: Image.PreserveAspectFit
                smooth: true
                Component.onCompleted: {
                    if (model.url !== "") {
                        myIcon.source = model.iconUrl
                    } else {
                        myIcon.source = (isMediaServer ? Globals.mediaServerIconList : Globals.mediaRendererIconList)
                    }
                }
            }
            Text {
                id: label
                anchors {
                    left: myIcon.right
                    verticalCenter: gridItem.verticalCenter
                    leftMargin: 8
                }
                Behavior on color {
                    ColorAnimation { duration: 100 }
                }
                font.family: "helvetica"
                font.pixelSize: 20
                text: name
                color: "#505050"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if(grid.currentIndex !== index) {
                        grid.currentIndex = index
                        selectionChanged(label.text)
                        selectionChangedSendUUID(model.uuid)
                    }
                }
            }
            states:
                State {
                name: "selected"
                when: (gridItem === grid.currentItem)
                PropertyChanges {
                    target: label
                    color: "#179C78"
                }
                PropertyChanges {
                    target: myIcon
                    source:(model.iconUrl === ""
                              ? (isMediaServer
                                     ? Globals.mediaServerIconListSel
                                     : Globals.mediaRendererIconListSel)
                              : model.iconUrl)
                }
            }
            ListView.onAdd: {
                grid.size++
            }

            ListView.onRemove: {
                grid.size--
            }
        }
    }


    //    function getWidth(){
    //        return grid.width
    //    }
    //    function getX(){
    //        return grid.x
    //    }
    //    function setCallBackFunction(func) {
    //        mygrid.callback = func
    //    }
}
