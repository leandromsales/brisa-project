import bb.cascades.multimedia 1.0
import bb.multimedia 1.0
import bb.cascades 1.0
import bb.system 1.0
import Flashlight 1.0

Page {
    id: mainPage
    property bool activeLamp: _app.activeLamp
    property alias imgLampSrc: lampImg.imageSource //"asset:///images/offlight.png"
    property alias background: maincontainer.background
    property string imgConection: "asset:///images/ic_no_connections.png"
    Menu.definition: MenuDefinition {
        settingsAction: SettingsActionItem {
            onTriggered: {
                settingsSheet.createObject().open();
            }
        }
        helpAction: HelpActionItem {
            onTriggered: {
                helpSheet.createObject().open();
            }
        }
        actions: [
            ActionItem {
                title: "About"
                imageSource: "asset:///images/ic_info.png"
                onTriggered: {
                    aboutSheet.createObject().open()
                }
            },
            ActionItem {
                imageSource: "asset:///images/ic_bbm.png"
                title: "Share in BBM"
                onTriggered: {
                    _app.callBBMCard();
                }
            }
        ]
    }
    onActiveLampChanged: {
        if (activeLamp) {
            actionStatus.title = "Turn off";
            actionStatus.imageSource = "asset:///images/offlighticon.png";
            if (_app.lampAndFlash) {
                imgLampSrc = "asset:///images/onlight.png";
                flashlight.enabled = true;
                maincontainer.background = Color.White;
            } else if (_app.flash) {
                imgLampSrc = "";
                flashlight.enabled = true;
                maincontainer.background = Color.DarkGray;
            } else if (_app.lamp) {
                imgLampSrc = "asset:///images/onlight.png";
                maincontainer.background = Color.White;
                flashlight.enabled = false;
            } else {
                imgLampSrc = "";
                flashlight.enabled = false;
                maincontainer.background = Color.White;
            }
        } else {
            actionStatus.title = "Turn on";
            actionStatus.imageSource = "asset:///images/onlighticon.png";
            if (_app.lampAndFlash) {
                imgLampSrc = "asset:///images/offlight.png";
                flashlight.enabled = false;
                maincontainer.background = Color.DarkGray;
            } else if (_app.flash) {
                imgLampSrc = "";
                flashlight.enabled = false;
                maincontainer.background = Color.DarkGray;
            } else if (_app.lamp) {
                flashlight.enabled = false;
                imgLampSrc = "asset:///images/offlight.png";
                maincontainer.background = Color.DarkGray;
            } else {
                imgLampSrc = "";
                flashlight.enabled = false;
                maincontainer.background = Color.DarkGray;
            }
        }
    }
    Container {
        property bool connection: _app.isConnected
        id: maincontainer
        objectName: "maincontainer"
        topPadding: 20
        leftPadding: 20
        rightPadding: 20
        bottomPadding: 20
        background: Color.DarkGray
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill
        layout: DockLayout {
        }
        Container {
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Left
            ImageView {
                imageSource: imgConection
                horizontalAlignment: HorizontalAlignment.Center
            }
            Label {
                text: "Connected"
                textStyle.fontSize: FontSize.XSmall
                horizontalAlignment: HorizontalAlignment.Center
            }
        }
        ImageView {
            id: lampImg
            imageSource: _app.initImage
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
        }
        onConnectionChanged: {
            if (connection) {
                imgConection = "asset:///images/ic_connections.png";
                systemToast.body = "You are connected!";
                systemToast.show();
            } else {
                imgConection = "asset:///images/ic_no_connections.png";
                systemToast.body = "You aren't connected";
                systemToast.show();
            }
        }
    }
    actions: [
        ActionItem {
            id: actionStatus
            title: "Turn on"
            imageSource: "asset:///images/onlighticon.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                _app.activeLamp = ! _app.activeLamp;
            }
        }
    ]
    attachedObjects: [
        SystemToast {
            id: systemToast
        },
        ComponentDefinition {
            id: aboutSheet
            About {
            }
        },
        ComponentDefinition {
            id: settingsSheet
            Settings {
            }
        },
        ComponentDefinition {
            id: helpSheet
            Help {
            }
        },
        Flashlight {
            id: flashlight
        }
    ]
}