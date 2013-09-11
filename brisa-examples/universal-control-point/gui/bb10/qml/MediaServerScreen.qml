import bb.cascades 1.0

NavigationPane {
    id: navPane
    Page {
        Container {
            Label {
                text: "Media servers"
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
            }
            ListView {
                id: listView
                objectName: "media_server_listview"
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
