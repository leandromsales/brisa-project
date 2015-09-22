import QtQuick 2.0
import QtQuick.Controls 1.2

import "qrc:/pages"

Rectangle {
    id:acd
    width: parent.width
    height: parent.height

    x:-width
    color: "transparent"

    anchors.verticalCenter: parent.verticalCenter

    Rectangle {
        id:backRect
        anchors.fill: parent
        color:"black"
        opacity: 0.5

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true

            onClicked: hide();
        }
    }

    Rectangle {
        id:centerRect
        width:parent.width*(0.75)
        height:parent.height*(0.75)
        anchors.centerIn: parent

        Rectangle {
            id:topBar
            anchors {
                fill:parent
                bottomMargin: parent.height*(0.8)
            }

            color: "#0099FF"

            Text {
                id:textTopBar
                anchors.fill: parent
                anchors.leftMargin: parent.width/20

                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter

                color: "white"

                font {
                    pixelSize: height/2
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
        }

        StackView {
            id:centerRectStackPages
            anchors.fill: parent
            anchors.topMargin: topBar.height
            initialItem: firstPage

            delegate: StackViewDelegate {
                function transitionFinished(properties)
                {
                    properties.exitItem.scale = 1
                }

                pushTransition: StackViewTransition {

                    PropertyAnimation {
                        target: enterItem
                        property: "scale"
                        from: 0
                        to: 1
                    }
                    PropertyAnimation {
                        target: exitItem
                        property: "scale"
                        from: 1
                        to: 0
                    }

                }

                popTransition: StackViewTransition {

                    PropertyAnimation {
                        target: exitItem
                        property: "scale"
                        from: 1
                        to: 0
                    }
                    PropertyAnimation {
                        target: enterItem
                        property: "scale"
                        from: 0
                        to: 1
                    }

                }
            }
        }

        FirstPageAppCreation {
            id:firstPage
        }
    }
    //Animations
    SequentialAnimation {
        id:showAnimation
        running: false

        SmoothedAnimation {
            target:acd
            property: "x"
            to:(acd.parent.width/2 - acd.width/2)
            duration: 150
        }
        PropertyAnimation {
            target:backRect
            property: "opacity"
            to:0.5
            duration: 150
        }

    }

    SequentialAnimation {
        id:hideAnimation
        running: false

        PropertyAnimation {
            target:backRect
            property: "opacity"
            to:0
            duration: 150
        }
        SmoothedAnimation {
            target:acd
            property: "x"
            to:-acd.width
            duration: 150
        }
    }

    //Methods
    function hide(){
        hideAnimation.start();
        destroyDialog();
    }
    function show(){
        showAnimation.start();
    }
}

