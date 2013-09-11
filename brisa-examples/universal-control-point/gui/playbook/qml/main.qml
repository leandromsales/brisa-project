import QtQuick 2.0
import "globals.js" as Globals

Rectangle {
    id: mainView
    objectName: "mainView"
    property bool setupMediaRendererDone: false
    property bool setupMediaServerDone: false

    width: 1024
    height: 600

    Image {
        id: background
        source: Globals.backgroundImage
        objectName: "bg"
    }

    Mygrid {
        id: gridMediaServer
        backgroundColor: "#00FFFFFF"
        isMediaServer: true
        Component.onCompleted: {
            gridMediaServer.setFrame(22, 105, 198, 405)
            gridMediaServer.setModel(mediaServerModel);
            /* Signals and slot connections */
            gridMediaServer.selectionChanged.connect(leftPanel.changeLabel)
            gridMediaServer.selectionChangedSendUUID.connect(mainViewController.onChangeSelectionForServerList)
        }
    }

    Mygrid {
        id: gridMediaRenderer
        backgroundColor: "#00FFFFFF"
        Component.onCompleted: {
            gridMediaRenderer.setFrame(790, 105, 218, 405)
            gridMediaRenderer.setModel(mediaRendererModel)
            /* Signals and slot connections */
            gridMediaRenderer.selectionChanged.connect(rightPanel.changeLabel)
            gridMediaRenderer.selectionChangedSendUUID.connect(mainViewController.onChangeSelectionForRendererList)
        }
    }

    ContentList {
        id: serverContent
        Component.onCompleted: {
            serverContent.setFrame(264, 106, 491, 401)
            serverContent.setModel(mediaServerContentModel)
            /* Signals and slot connections */
            serverContent.backButtonTouched.connect(mainViewController.onBackButtonPressed)
            serverContent.selectionChanged.connect(mainViewController.onChangeSelectionForServerContentList)
            mainViewController.emptyMediaServerContentList.connect(serverContent.emptyContentList)
//            serverContent.playWithObject.connect(mainViewController.onPlayPausePressed)
        }
    }
    MediaControlBar {
        id: controlBar
        Component.onCompleted: {
            controlBar.play.connect(mainViewController.onPlayPausePressed)
            controlBar.stop.connect(mainViewController.onStopButtonPressed)
            mainViewController.changeStatusToPlay.connect(controlBar.changePlayStatus)
        }
    }

    VideoView {
        id: videoView
        visible: _mediaPlayerModel.visibility
    }

    MediaPanel {
        id: rightPanel
        Component.onCompleted: {
            rightPanel.setFrame(523,19,130,55);
        }
    }

    MediaPanel {
        id: leftPanel
        Component.onCompleted: {
            leftPanel.setFrame(365, 19, 130,55)
        }
    }
}
