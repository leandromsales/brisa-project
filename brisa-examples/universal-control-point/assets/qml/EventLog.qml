import bb.cascades 1.0

Page {
    titleBar: TitleBar {
        title: "Event Log"
    }
    Container {
        ListView {
            id: eventList
            objectName: "event_log_listview"
            dataModel: _viewController.eventLogModel
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            listItemComponents: [
                ListItemComponent {
                    type: "listItem"
                    StandardListItem {
                        title: ListItemData.service + " - " + ListItemData.variableName
                        description: "By " + ListItemData.deviceName + " device"
                        status: ListItemData.newValue
                        contextActions: ActionSet {
                            title: ListItemData.type + " event"
                            actions: [
                                InvokeActionItem {
                                    title: "Share"

                                    query {
                                        mimeType: "text/plain"
                                        invokeActionId: "bb.action.SHARE"
                                    }

                                    onTriggered: {
                                        data = "I received a " + ListItemData.type + " event of the " + ListItemData.deviceName + " device, using the Universal Control Point.\nService: " + ListItemData.service + ";\nVariable name: " + ListItemData.variableName + ";\nNew value: " + ListItemData.newValue + "."
                                    }
                                },
                                DeleteActionItem {
                                    title: "Delete item"
                                    imageSource: "asset:///images/ic_delete.png"
                                    onTriggered: {
                                        console.log(ListItemData)
                                        console.log(ListItem)
                                        _viewController.removeEvent(ListItemData);
                                    }
                                }
                            ]
                        }
                    }
                }
            ]
            onTriggered: {
                var data = dataModel.data(indexPath);
                var eventDetail = eventLogDetail.createObject();
                eventDetail.event = data;
                navPaneEvent.push(eventDetail);
            }
            
            function itemType(data, indexPath) {
                if (indexPath.length == 1) {
                    // If the index path contains a single integer, the item
                    // is a "header" type item
                    return "header";
                } else {
                    // If the index path contains more than one integer, the
                    // item is a "listItem" type item
                    return "listItem";
                }
            }
        }
    }
    actions: [
        InvokeActionItem {
            title: "Share"

            query {
                mimeType: "text/plain"
                invokeActionId: "bb.action.SHARE"
            }

            onTriggered: {
                data = "I can receive multicast and unicast events of the UPnP devices of my local network using Universal Control Point.";
            }
        },
        DeleteActionItem {
            title: "Clear list"
            imageSource: "asset:///images/ic_clear_list.png"
            onTriggered: {
                _viewController.clearEventLog();
            }
        }
    ]
    attachedObjects: ComponentDefinition {
        id: eventLogDetail
        EventLogDetail {
        }
    }
}
