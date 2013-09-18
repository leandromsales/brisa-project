import bb.cascades 1.2

Page {
    Container {
        Label {
            text: "Devices"
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
        }
        ListView {
            id: listView
            objectName: "control_point_listview"
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            listItemComponents: ListItemComponent {
                StandardListItem {
                    title: ListItemData["name"]
                    description: ListItemData["deviceType"]
                }
            }
            onTriggered: {
                var openServices = services.createObject()
                serviceController.setDataModel(indexPath)
                navPane.push(openServices)
            }
            contextActions: ActionSet {
                ActionItem {
                    title: "Remove"
                }
            }
            attachedObjects: [
                ComponentDefinition {
                    id: services
                    Services {

                    }
                }
            ]
        }
    }
}
