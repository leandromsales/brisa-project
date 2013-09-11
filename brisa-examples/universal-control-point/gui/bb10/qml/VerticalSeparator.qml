import QtQuick 2.0

Rectangle {
    height: parent.height/2; width: 2; color: "#fafafa"
    anchors {
        right: parent.right
//                            top: parent.top
        verticalCenter: parent.verticalCenter
    }
    Rectangle {
        y: 0
        height: parent.height; width: 1; color:  "#d8d8d8"
        anchors{
//                    bottom: separator.top
//                                top: parent.top
            verticalCenter: parent.verticalCenter
        }
    }
}
