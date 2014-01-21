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
                    multiline: true
                    textStyle {
                        fontSize: FontSize.Large
                        fontWeight: FontWeight.Bold
                        textAlign: TextAlign.Justify
                    }
                }
                Label {
                    text: "In the first list displayed when you open the app (will show the devices on your local network) choose a device from the list."
                    textStyle.textAlign: TextAlign.Justify
                    multiline: true
                }
                Label {
                    text: "After you have chosen a device will be shown a list of services and embedded devices of the selected device. Choose a service or device embedded in this list."
                    textStyle.textAlign: TextAlign.Justify
                    multiline: true
                }
                Label {
                    text: "If you choose a service, you are shown a list of actions that service. Choose an action and an action invocation screen appears."
                    textStyle.textAlign: TextAlign.Justify
                    multiline: true
                }
                Label {
                    text: "On invocation of action screen will display the name of the device (or embedded device), service and action chosen."
                    textStyle.textAlign: TextAlign.Justify
                    multiline: true
                }
                Label {
                    text: "If the action requires input arguments will aprensentado algumns places for arguments seram inserted. Enter the arguments and invoke the action and wait for their response. If the action does not require input arguments only invoke the action and wait for their response."
                    textStyle.textAlign: TextAlign.Justify
                    multiline: true
                }
            }
        }
    }
}