import bb.cascades 1.2

Page {
    Container {
        Label {
            text: "Services"
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
        }
        ListView {
            id: listView
            objectName: "services_listview"
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            dataModel: serviceController.getDataModel()

            listItemComponents: ListItemComponent {
                StandardListItem {
                    title: ListItemData.type
                    //                    description: ListItemData["deviceType"]
                }
            }
            onTriggered: {
                var openActions = actions.createObject()
                actionController.setDataModel(dataModel.data(indexPath))
                navPane.push(openActions)
            }
            contextActions: ActionSet {
                ActionItem {
                    title: "Remove"
                }
            }
            attachedObjects: [
                ComponentDefinition {
                    id: actions
                    Actions {

                    }
                }
            ]
        }
    }
}
