import bb.cascades 1.2

Page {
    Container {
        Label {
            text: "Actions"
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
        }
        ListView {
            id: listView
            objectName: "actions_listview"
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            dataModel: actionController.getDataModel()

            listItemComponents: ListItemComponent {
                StandardListItem {
                    title: ListItemData["name"]
                    //                    description: ListItemData["deviceType"]
                }
            }
            onTriggered: {
//                actionController.setCurrentAction(dataModel.data(indexPath)["name"])
                var openControlSheet = controlSheet.createObject()
                openControlSheet.labelDevice = serviceController.getDeviceName()
                openControlSheet.labelService = actionController.getServiceType()
                openControlSheet.labelAction = dataModel.data(indexPath)["name"]
                openControlSheet.open()
//                var openActions = controlSheet.createObject()
//                actionController.setDataModel(dataModel.data(indexPath))
//                navPane.push(controlSheet)
            }
            contextActions: ActionSet {
                ActionItem {
                    title: "Remove"
                }
            }
            attachedObjects: [
                ComponentDefinition {
                    id: controlSheet
                    ControlSheet {
                    }
                }
            ]
        }
    }
}
