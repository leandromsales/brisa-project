//.pragma library

var backgroundImage = "images/bg.png"
var mediaServerIconList = "images/ic_server_default.png"
var mediaServerIconListSel = "images/ic_server_selected.png"
var mediaRendererIconList = "images/ic_renderer_default.png"
var mediaRendererIconListSel = "images/ic_renderer_selected.png"

var folderIcon = "images/ic_folder.png"
var backIcon = "images/ic_back.png"
var backPressedIcon = "images/ic_back_pressed.png"

var playButton = "images/bt_play.png"
var stopButtonDefault = "images/bt_stop_default.png"
var stopButtonPressed = "images/bt_stop_pressed.png"
var pauseButton = "images/bt_pause.png"
var viewButton = "images/bt_view.png"
var volumeButton = "images/bt_volume.png"

var mediaControlBarImage = "images/controls_player.png"

function createContentList() {
    var component = Qt.createComponent("ContentList.qml");
    if (component.status === Component.Ready)
        component.createObject(parent, {"id": newContentList, "x": 100, "y": 100});
        component.setFrame(264, 106, 491, 401)
        component.setModel(contentListModel)

}
