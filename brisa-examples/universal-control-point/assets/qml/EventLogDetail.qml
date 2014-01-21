import bb.cascades 1.0

Page {
    property variant event

    titleBar: TitleBar {
        title: "Event Log Detail"
    }
    ScrollView {
        Container {
            topPadding: 20
            leftPadding: 20
            rightPadding: 20
            bottomPadding: 20
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                Label {
                    text: "Event type:"
                    textStyle {
                        fontSize: FontSize.Large
                        fontWeight: FontWeight.Bold
                    }
                }
                Label {
                    id: type
                    text: event["type"]
                    textStyle {
                        fontSize: FontSize.Large
                    }
                }
            }
            Divider {

            }
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                Label {
                    text: "Device:"
                    textStyle {
                        fontSize: FontSize.Large
                        fontWeight: FontWeight.Bold
                    }
                }
                Label {
                    id: device
                    text: event["deviceName"]
                    textStyle {
                        fontSize: FontSize.Large
                    }
                }
            }
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                Label {
                    text: "Service:"
                    textStyle {
                        fontSize: FontSize.Large
                        fontWeight: FontWeight.Bold
                    }
                }
                Label {
                    id: service
                    text: event["service"]
                    textStyle {
                        fontSize: FontSize.Large
                    }
                }
            }
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                Label {
                    text: "Variable Name:"
                    textStyle {
                        fontSize: FontSize.Large
                        fontWeight: FontWeight.Bold
                    }
                }
                Label {
                    id: variable
                    text: event["variableName"]
                    textStyle {
                        fontSize: FontSize.Large
                    }
                }
            }
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                Label {
                    text: "New value:"
                    textStyle {
                        fontSize: FontSize.Large
                        fontWeight: FontWeight.Bold
                    }
                }
                Label {
                    id: value
                    text: event["newValue"]
                    textStyle {
                        fontSize: FontSize.Large
                    }
                }
            }
        }
    }
    actions: [
        InvokeActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
            title: "Share"

            query {
                mimeType: "text/plain"
                invokeActionId: "bb.action.SHARE"
            }

            onTriggered: {
                data = data = "I received a " + type.text + " event of the " + device.text + " device, using the Universal Control Point.\nService: " + service.text + ";\nVariable name: " + variable.text + ";\nNew value: " + value.text + "."
            }
        },
        DeleteActionItem {
            title: "Delete item"
            imageSource: "asset:///images/ic_delete.png"
            onTriggered: {
                _viewController.removeEvent(event);
                navPaneEvent.navigateTo(navPaneEvent.firstPage)
            }
        }
    ]
}
