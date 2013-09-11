import QtQuick 2.0

Rectangle {
    width: parent.width; height: 2; color: "#fafafa"
    anchors {
        bottom: parent.bottom
        left: parent.left
    }
    Rectangle {
        y: 0
        width: parent.width; height: 1; color: "#d8d8d8"
        anchors{
//                    bottom: separator.top
            left: parent.left
        }
    }
}
