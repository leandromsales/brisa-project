import QtQuick 2.0
import QtGraphicalEffects 1.0

Rectangle {
    id:fab

    property string mainRectColor
    property real iconScale : 3.5
    property string icon: ""
    property real raio : 50

    property alias mouseArea : mouseFAB

    radius: raio
    width: raio
    height: raio

    Behavior on scale {
        PropertyAnimation {
            duration: 100
        }
    }

    DropShadow {
        anchors.fill: mainRect
        source:mainRect
        horizontalOffset: 0
        verticalOffset: 7
        radius: 8.0
        samples: 24
        color: "#90000000"
        transparentBorder: true
    }

    Rectangle {
        id:mainRect

        radius: raio
        width: raio
        height: raio

        color: mainRectColor
        anchors.fill: parent

        Image {
            id:fabIcon
            anchors{
                fill: parent
                topMargin: parent.height/iconScale
                leftMargin: parent.height/iconScale
                rightMargin: parent.height/iconScale
                bottomMargin: parent.height/iconScale
            }
            source: icon

            Behavior on rotation {
                NumberAnimation {
                    duration: 150
                }
            }
        }

        MouseArea {
            id:mouseFAB
            anchors.fill: parent
            hoverEnabled: true

            onEntered: fab.scale = 1.5
            onExited: fab.scale = 1

            onClicked: {
                fabIcon.rotation += 90
            }
        }
    }
}

