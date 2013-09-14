import bb.cascades 1.0

NavigationPane {
    id: navPane
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
                    type: ""
                    ListItemComponent {
                        StandardListItem {
                            title: ListItemData.name
                        }
                    }
                
                }
                onTriggered: {
//						dataModel.data(indexPath)
                    var openServices = services.createObject()
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
}
