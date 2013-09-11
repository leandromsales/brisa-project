import QtQuick 2.0
import QtMultimedia 5.0

Rectangle {
    id: videoView
    objectName: "videoView"
    x: 342
    y: 200
    width: 342
    height: 200
    color: "black" //"#00000000" //Qt.tint("lightsteelblue", "#10FF0000") // Qt.rgba(255,255,255,255) //"black" //"transparent"

    Behavior on x {
      PropertyAnimation { duration: 500 }
    }

    Behavior on y {
      PropertyAnimation { duration: 500 }
    }

    Behavior on width {
      PropertyAnimation { duration: 500 }
    }

    Behavior on height {
      PropertyAnimation { duration: 500 }
    }

    VideoOutput {
        id: videoOutput
        source: _mediaPlayerModel
        anchors.fill: parent
        width: parent.width
        height: parent.width
        fillMode: "Stretch"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (videoView.state == "") {
                videoView.state = "fullscreen"
            } else {
                videoView.state = ""
            }
        }
    }

  states: [
    State {
        name: "fullscreen"
        PropertyChanges {
            target: videoView
            x: 0
            y: 0
            width: mainView.width 
            height: mainView.height
        }
    },

    State {
        name: "invisible"
        PropertyChanges {
            target: videoView
            x: 512
            y: 300
            width: 0
            height: 0
        }
    },

    State {
        name: ""
        PropertyChanges {
            target: videoView
            x: 342
            y: 200
            width: 342
            height: 200
        }
    }
  ]
}
