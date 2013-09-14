import bb.cascades 1.0

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

            listItemComponents: ListItemComponent {
                type: ""
                ListItemComponent {
                    StandardListItem {
                        title: ListItemData.name
                    }
                }

            }
            contextActions: ActionSet {
                ActionItem {
                    title: "Remove"
                }
            }
        }
    }
}
