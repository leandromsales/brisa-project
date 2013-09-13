import QtQuick 2.0
import "globals.js" as Globals

Rectangle {
    id: contentList
    property int selectedItem: -1

    signal backButtonTouched
    signal selectionChanged(string itemObjectId)
    signal emptyContentList
//    signal playWithObject(string itemObjectId, bool isPlaying)

//    signal sendPlayWithObject(bool isPlaying)

//    onSendPlayWithObject: {
//        playWithObject(list.currentItem.objectId, isPlaying)
//    }

    function setModel(newModel) {
        list.model = newModel
    }
    function setFrame(x, y, width, height){
        contentList.x = x
        contentList.y = y
        list.y = 44
        contentList.width = list.width = width
        contentList.height = list.height = height
    }
    onEmptyContentList: {
        list.currentIndex = -1
    }
    clip: true
    color: "#00000000"
    Rectangle{
        width: parent.width
        height: 44
        color: "#00000000"
        Image {
            id: backButton
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
            }
            source: Globals.backIcon
            MouseArea {
                width: parent.width
                height: parent.height
                onPressed: {
                    backButton.state = "pressed"
                    backButtonTouched()
                }
                onReleased: {
                    backButton.state = ""
                }
            }
            states: State {
                name: "pressed"
                PropertyChanges {
                    target: backButton
                    source: Globals.backPressedIcon
                }
            }
            VerticalSeparator {}
        }
        Rectangle{
            width: parent.width - backButton.width
            height: parent.height
            color: "#00000000"
            anchors {
                left: backButton.right
                verticalCenter: parent.verticalCenter
            }
            FolderNavigator {
                id: folderNavigator
                Component.onCompleted: {
                    /*Connect signals and slots */
                    folderNavigator.selectionChanged.connect(mainViewController.onSelectionChangedFolderNavigator)
                    folderNavigator.setModel(navigationContentModel)
                }
            }
        }
        HorizontalSeparator {}
    }

    ListView {
//        id: list
        focus: true
        clip: true
        delegate: Item {
            id: contentItem
            width: 491
            height: 44
            Rectangle {
                id: selection
                width: parent.width
                height: parent.height
                color: "#00FFFFFF"
                Behavior on color {
                    ColorAnimation { duration: 100 }
                }
                anchors {
                    centerIn: parent.Center
                }
            }
            Image {
                id: itemIcon
                anchors.verticalCenter: contentItem.verticalCenter
                width: 26
                height: 26
                fillMode: Image.PreserveAspectFit
                source: (model.container ? Globals.folderIcon : "")
                smooth: true
            }
            Text {
                id: itemLabel
                anchors {
                    left: itemIcon.right
                    verticalCenter: contentItem.verticalCenter
                    leftMargin: 8
                }
                font.family: "helvetica"
                font.pixelSize: 16
                text: model.title
                color: "#5e5a5a"
            }
            opacity: 0.0
            Behavior on opacity {
                PropertyAnimation { duration: 500 }
            }
            HorizontalSeparator {}
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (list.currentIndex !== index) {
                        list.currentIndex = index
                        selectionChanged(model.objectId)
                    }
                }
            }
            states:
                State {
                    name: "selected"
                    when: (contentItem===list.currentItem)
                    PropertyChanges { target: selection; color: "#FFFFFF"}
                }
            ListView.onRemove: {
                contentItem.opacity = 0.0;
            }
            ListView.onAdd: {
                contentItem.opacity = 1.0;
            }
        }
    }
    Component.onCompleted: {
        list.currentIndex = -1;
    }
}
