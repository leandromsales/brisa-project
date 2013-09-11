// import Q // to target S60 5th Edition or Maemo 5
import QtQuick 2.0

ListView {
    id: list
    signal selectionChanged(string objectId)
    function setModel(newModel) {
        list.model = newModel
    }
//    Component.onCompleted: {
//        list.currentIndex = -1;
//    }
    delegate: Rectangle {
        id: navigatorItem
        color: "#00000000"
        height: parent.height
        Behavior on width {
            PropertyAnimation{duration: 300}
        }
        Text {
            id: folderItemLabel
            text: model.title
            font.family: "helvetica"
            font.pixelSize: 16
            font.bold: true
            anchors.centerIn: parent
            color: "#afafaf"
            Behavior on color {
                PropertyAnimation{duration: 300}
            }
        }
        width: 0 // folderItemLabel.width + 10
        VerticalSeparator {}
        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (list.currentIndex !== index) {
                    list.currentIndex = index
                    selectionChanged(model.objectId)
                }
            }
        }
        ListView.onAdd: {
            list.currentIndex = index
            navigatorItem.width = folderItemLabel.width + 10
        }
//        ListView.onRemove: {
//            navigatorItem.width = 0
//        }
        states:
            State {
            name: "selected"
            when: (navigatorItem==list.currentItem)
            PropertyChanges {
                target: folderItemLabel
                color: "#303030"
            }
        }
    }
    clip: true
    anchors.fill: parent
    orientation: ListView.Horizontal
}
