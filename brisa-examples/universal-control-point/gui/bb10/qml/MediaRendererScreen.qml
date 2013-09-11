import bb.cascades 1.0

NavigationPane {
    id: navPane
    Page {
        Container {
            Label {
                text: "Media renderer"
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
            }
            ListView {
                id: listView
                objectName: "media_renderer_listview"
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
}
