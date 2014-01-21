import bb.system 1.0
import bb.cascades 1.0

Page {
    property string noDeviesMessage: "No UPnP device found on this wireless\nnetwork! Connect any device to the\nnetwork or change your wireless network.\nOr also try using the refresh button."
    titleBar: TitleBar {
        title: "Devices"
        acceptAction: ActionItem {
            title: "Refresh"
            onTriggered: {
                searchingDlgt.delegateActive = true;
                _viewController.startSearchingTimer();
                _viewController.refreshList();
            }
        }
    }
    Container {
        property bool emptyList: _viewController.isEmptyList
        property bool connection: _viewController.isConnected
        property bool searchingVisible: _viewController.searching
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill
        layout: DockLayout {
        }
//        onCreationCompleted: {
//            _viewController.emptyList.connect(onEmptyList);
//        }
        onEmptyListChanged: {
            onEmptyList(emptyList);
        }
        onConnectionChanged: {
            if (connection) {
                systemToast.body = "You are connected!";
                noDeviesMessage = "No UPnP device found on this wireless\nnetwork! Connect any device to the\nnetwork or change your wireless network.\nOr also try using the refresh button."
                searchingDlgt.delegateActive = true;
            } else {
                systemToast.body = "You are disconnected!";
                noDeviesMessage = "No UPnP device found!\nNo connection to the internet!\nPlease connect to any wireless network."
                searchingDlgt.delegateActive = false;
            }
            systemToast.show();
        }
        onSearchingVisibleChanged: {
            if (emptyList && connection) {
                searchingDlgt.delegateActive = true;
            } else {
                searchingDlgt.delegateActive = false;
            }
        }
        function onEmptyList(isEmpty) {
            if (isEmpty) {
                noItemListContainer.visible = true;
                listView.visible = false;
                _viewController.startSearchingTimer();
            } else {
                noItemListContainer.visible = false;
                listView.visible = true;
            }
        }
        ListView {
            id: listView
            objectName: "control_point_listview"
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            dataModel: _viewController.devicesModel
            listItemComponents: ListItemComponent {
                StandardListItem {
                    title: ListItemData.friendlyName
                    description: ListItemData.typeName
                    contextActions: ActionSet {
                        title: "Share this device"
                        subtitle: "Share that you can control this device!"
                        InvokeActionItem {
                            ActionBar.placement: ActionBarPlacement.OnBar
                            title: "Share"
                            query {
                                mimeType: "text/plain"
                                invokeActionId: "bb.action.SHARE"
                            }
                            onTriggered: {
                                data = "I can control " + ListItemData["name"] + " device using Universal Control Point App for BlackBerry 10.";
                            }
                        }
                    }
                }
            }
            onTriggered: {
                var nextList = genericList.createObject();
                nextList.title = "Devices/Services";
                nextList.dataModel = _viewController.getGenericDataModelByID(dataModel.data(indexPath)["id"]);
                navPaneDevices.push(nextList);
            }
            attachedObjects: [
                ComponentDefinition {
                    id: genericList
                    GenericList {
                    }
                }
            ]
        }
        Container {
            id: noItemListContainer
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            Label {
                multiline: true
                text: noDeviesMessage
                textStyle.textAlign: TextAlign.Center
                horizontalAlignment: HorizontalAlignment.Center
            }
            Button {
                text: "Network Connections"
                horizontalAlignment: HorizontalAlignment.Fill
                onClicked: {
                    _viewController.callSettingsCard();
                }
            }
        }
        ControlDelegate {
            id: searchingDlgt
            delegateActive: false
            sourceComponent: searchingInd
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Fill
        }
        attachedObjects: [
            ComponentDefinition {
                id: searchingInd
                SearchingIndicator {
                }
            },
            SystemToast {
                id: systemToast
            }
        ]
    }
}
