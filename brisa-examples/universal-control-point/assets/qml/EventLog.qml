import bb.cascades 1.2

NavigationPane {
    id: navPane
    Page {
        Container {
            Label {
                text: "Event Log"
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
            }
            ListView {
                id: listView
                objectName: "event_log_listview"
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
