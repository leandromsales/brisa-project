import bb.cascades 1.0

Sheet {
    id: settingsSheet
    property bool flag: false
    property bool settingsChanged: false
    property string imgLampSrc
    onCreationCompleted: {
        if (_app.lamp) {
            radioOptions.selectedIndex = 0;
        } else if (_app.flash) {
            radioOptions.selectedIndex = 1;
        } else if (_app.lampAndFlash) {
            radioOptions.selectedIndex = 2;
        } else {
            radioOptions.selectedIndex = 3;
        }
        flag = true;
    }
    Page {
        titleBar: TitleBar {
            title: "Settings"
            acceptAction: ActionItem {
                title: "Ok"
                onTriggered: {
                    if (settingsChanged) {
                        _app.activeLamp = false;
                        mainPage.imgLampSrc = imgLampSrc;
                        mainPage.background = Color.DarkGray;
                        _app.updateSettings();
                    }
                    settingsSheet.close();
                }
            }
        }
        ScrollView {
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            Container {
                Header {
                    title: "App"
                }
                Container {
                    leftPadding: 20
                    rightPadding: 20
                    Label {
                        multiline: true
                        textStyle.fontSize: FontSize.Large
                        text: "Choose how to be show the reaction of this device."
                    }
                }
                RadioGroup {
                    id: radioOptions
                    dividersVisible: true
                    Option {
                        text: "Lamp on screen"
                        description: "Use a lamp image with response"
                    }
                    Option {
                        text: "Camera flash"
                        description: "Use camera flash with response"
                    }
                    Option {
                        text: "Lamp and Flash"
                        description: "Use lamp image and flash with response"
                    }
                    Option {
                        text: "Only screen"
                        description: "Use only screen with response"
                    }
                    onSelectedIndexChanged: {
                        if (flag) {
                            settingsChanged = true;
                            if (selectedIndex == 0) {
                                imgLampSrc = "asset:///images/offlight.png";
                                _app.lamp = true;
                                _app.flash = false;
                                _app.lampAndFlash = false;
                                _app.screen = false;
                            } else if (selectedIndex == 1) {
                                imgLampSrc = "";
                                _app.lamp = false;
                                _app.flash = true;
                                _app.lampAndFlash = false;
                                _app.screen = false;
                            } else if (selectedIndex == 2) {
                                imgLampSrc = "asset:///images/offlight.png";
                                _app.lamp = false;
                                _app.flash = false;
                                _app.lampAndFlash = true;
                                _app.screen = false;
                            } else if (selectedIndex == 3) {
                                imgLampSrc = "";
                                _app.lamp = false;
                                _app.flash = false;
                                _app.lampAndFlash = false;
                                _app.screen = true;
                            }
                        }
                    }
                }
                Container {
                    Divider {
                    }
                }
                Header {
                    title: "System"
                }
                StandardListItem {
                    property bool touchIsDown: false
                    title: "Display"
                    description: "For settings of brightness screen"
                    imageSource: "asset:///images/ic_brightness_screen.png"
                    onTouch: {
                        if (event.touchType == TouchType.Up && touchIsDown) {
                            _app.callSettingsCard("settings://display");
                            touchIsDown = false
                        } else if (event.touchType == TouchType.Down) {
                            touchIsDown = true
                        }
                    }
                }
                StandardListItem {
                    property bool touchIsDown: false
                    title: "Network Connections"
                    description: "For settings of network connection"
                    imageSource: "asset:///images/ic_network_connections.png"
                    onTouch: {
                        if (event.touchType == TouchType.Up && touchIsDown) {
                            _app.callSettingsCard("settings://networkconnections");
                            touchIsDown = false
                        } else if (event.touchType == TouchType.Down) {
                            touchIsDown = true
                        }
                    }
                }
            }
        }
    }
}