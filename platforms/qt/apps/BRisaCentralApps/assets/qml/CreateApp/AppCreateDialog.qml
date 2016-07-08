import QtQuick 2.7
import QtQuick.Controls 2.0

import "qrc:/screen"
import "qrc:/components" as C
import "qrc:/components/functions.js" as JS

C.PopUpWindow {
    color:"White"
    onImminentHide : destroyDialog();
    content: MouseArea {
        anchors.fill: parent
        C.Frame {
            id:topBarFrame
            anchors.fill: parent
            topBarHeightPercent: 10
            topBarColor: "#09A"
            bottomBarVisible: false
//            stack.delegate: StackViewDelegate {
//                function transitionFinished(properties) { properties.exitItem.opacity = 1 }
//                pushTransition: StackViewTransition {
//                    PropertyAnimation {
//                        target: enterItem
//                        property: "opacity"
//                        from: 0; to: 1; duration: 100
//                    }
//                    PropertyAnimation {
//                        target: exitItem
//                        property: "opacity"
//                        from: 1; to: 0; duration: 100
//                    }
//                }
//            }
            topBarContent: Item{
                anchors.fill: parent
                Row {
                    anchors.fill: parent
                    C.ImageButton {
                        height:JS.hpercent(70,parent); width: height
                        anchors.verticalCenter: parent.verticalCenter
                        source:"qrc:/img/close.png"; color: "transparent"
                        action.onClicked: closed()
                    }
                    Text {
                        height:JS.hpercent(70,parent); width: JS.wpercent(40,parent)
                        anchors.verticalCenter: parent.verticalCenter
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        text:"Create an App ("+topBarFrame.stack.depth+"/3)"; color:"White"
                        font { bold:true; pixelSize: JS.hpercent(70,this) }
                    }
                }
            }
            content: firstPageComponent
        }
    }
    Component {
        id:firstPageComponent
        FirstPageAppCreation {}
    }
}

