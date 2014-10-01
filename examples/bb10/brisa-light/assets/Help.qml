import bb.cascades 1.0

Sheet {
    id: self
    Page {
        titleBar: TitleBar {
            title: "Help"
            dismissAction: ActionItem {
                title: "Close"
                onTriggered: {
                    self.close();
                }
            }
        }
        ScrollView {
            Container {
                topPadding: 20
                leftPadding: 20
                rightPadding: 20
                bottomPadding: 20
                Label {
                    text: "How To Use"
                    textStyle {
                        fontSize: FontSize.Large
                        fontWeight: FontWeight.Bold
                    }
                }
                Label {
                    text: "Click on this lamp in the action bar to toggle between on and off states icon."
                    textStyle.textAlign: TextAlign.Justify
                    multiline: true
                }
                Label {
                    text: "You can also change the states of the lamp using a univeral or specific UPnP controller for lamps."
                    textStyle.textAlign: TextAlign.Justify
                    multiline: true
                }
                Label {
                    text: "If you want to use UPnP controller, you need to be connected to your local network internet."
                    textStyle.textAlign: TextAlign.Justify
                    multiline: true
                }
                Label {
                    text: "Using UPnP controller you will need to access the device 'Binary Light Device', service 'SwitchPower'. There you will find the 'SetTarget', 'GetTarget' and 'GetStatus' actions."
                    textStyle.textAlign: TextAlign.Justify
                    multiline: true
                }
                Label {
                    text: "When using the 'SetTarget' action, use as input argument: 0 or false for turn off and 1 or true for turn on."
                    textStyle.textAlign: TextAlign.Justify
                    multiline: true
                }
                Label {
                    text: "When using the 'GetTarget' action or 'GetStatus', not haveram input argument. When the action is invoked the results will be: 0 or false for off and 1 or true to on."
                    textStyle.textAlign: TextAlign.Justify
                    multiline: true
                }
                Label {
                    text: "Settings"
                    textStyle {
                        fontSize: FontSize.Large
                        fontWeight: FontWeight.Bold
                    }
                }
                Label {
                    text: "In the settings you find Brinary Light application options as well as shortcuts to settings for internet connections and screen brightness."
                    textStyle.textAlign: TextAlign.Justify
                    multiline: true
                }
                Label {
                    text: "You can switch between the available settings in the settings menu, they are: Lamp on screen, Camera Flash, and Flash Lamp Only or screen."
                    textStyle.textAlign: TextAlign.Justify
                    multiline: true
                }
                Label {
                    text: "Lamp on screen - Turn on: Image of a lighted lamp on the screen; Turn off: Image of a cleared lamp on the screen."
                    textStyle.textAlign: TextAlign.Justify
                    multiline: true
                }
                Label {
                    text: "Camera flash - Turn on: Camera flash on; Turn off: Camera flash off."
                    textStyle.textAlign: TextAlign.Justify
                    multiline: true
                }
                Label {
                    text: "Lamp and Flash - Turn on: Image of a lighted lamp on the screen and camera flash on; Turn off: Image of a cleared lamp on the screen and camera flash off."
                    textStyle.textAlign: TextAlign.Justify
                    multiline: true
                }
                Label {
                    text: "Only screen - Turn on: White screen; Turn off: Gray screen."
                    textStyle.textAlign: TextAlign.Justify
                    multiline: true
                }
            }
        }
    }
}