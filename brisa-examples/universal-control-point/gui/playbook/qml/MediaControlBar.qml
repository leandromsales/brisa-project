import QtQuick 2.0
import "globals.js" as Globals

Rectangle {
    id: controlBar
    property int volume: 10

    signal play(bool isPlay)
    signal stop
    signal changePlayStatus(bool toPlay)

    onChangePlayStatus: {
        if(toPlay)
            playButton.state = "playing"
        else
            playButton.state = ""
    }

    width: 510
    height: 75
    color: "#00000000"
    anchors {
        horizontalCenter: parent.horizontalCenter
        bottom: parent.bottom
    }
    Image {
        source: Globals.mediaControlBarImage
    }
    Rectangle {
        id: volumeLevel
        x:53
        y:27
        width: 121
        height: 3
        color: "#179c78"
    }

    /*Rectangle {
        x:339
        y:27
        width: 99
        height: 3
        color: "#179c78"
    }*/
    /*Text {
        x:452
        y:24
        color: "#8e8d8d"
        font.family: "helvetica"
        text: qsTr("03:20")
    }*/
    Image {
        id: volumeButton
        x: 60
        y: 12
        source: Globals.volumeButton
        MouseArea {
            anchors.fill: parent
            drag.target: volumeButton
            drag.axis: Drag.XAxis
            drag.minimumX: 45
            drag.maximumX: 145
            onPositionChanged: {
                volumeLevel.width = 10 + volumeButton.x - volumeLevel.x
                if (volumeButton.x - 45 !== controlBar.volume) {
                    controlBar.volume = volumeButton.x - 45
//                    mainViewController.changeVolume(controlBar.volume)
                }
            }
            onReleased: {
                mainViewController.changeVolume(controlBar.volume)
            }
        }
        Component.onCompleted: {
            volumeLevel.width = 10 + volumeButton.x - volumeLevel.x
        }
    }
    Image {
        id: playButton
        y: 2
        x: 250
        source: Globals.playButton
        //anchors.horizontalCenter: parent.horizontalCenter
        states: [
            State {
                name: "playing"
                PropertyChanges {
                    target: playButton
                    source: Globals.pauseButton
                }
            }
        ]
        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (playButton.state === "playing") {
                    playButton.state = ""
                    play(0)
                    //videoView.state = "invisible"
                } else {
                    playButton.state = "playing"
                    play(1)
                    //videoView.state = ""
                }
            }
        }
    }
    Image {
        id: stopButton
        y: 7
        x: 200
        source: Globals.stopButtonDefault
        MouseArea {
            anchors.fill: parent
            onPressed: {
                stop()
                stopButton.state = "pressed"
            }
            onReleased: {
                stopButton.state = ""
				videoView.state = "invisible"
            }
        }
        states: State {
            name: "pressed"
            PropertyChanges {
                target: stopButton
                source: Globals.stopButtonPressed
            }
        }
    }
}
