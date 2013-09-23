import bb.cascades 1.2

Sheet {
    property alias labelDevice: deviceLabel.text
    property alias labelService: serviceLabel.text
    property alias labelAction: actionLabel.text
    id: controlSheet
    content: Page {
        onCreationCompleted: {
            actionController.getArgumentsMap()
        }
        titleBar: TitleBar {
            title: "Action Invocation"
            dismissAction: ActionItem {
                title: qsTr("Close")
                onTriggered: {
                    onClicked: controlSheet.close()
                }
            }
            acceptAction: ActionItem {
                title: qsTr("Invoke")
                onTriggered: {
                    saveSheet();
                }
            }
        }
        Container {
            objectName: "root_container"
            Label {
                id: deviceLabel
//                text: "Device"
            }
            Label {
                id: serviceLabel
//                text: "Service"
            }
            Label {
                id: actionLabel
//                text: "Action"
            }
        }
    }
}
