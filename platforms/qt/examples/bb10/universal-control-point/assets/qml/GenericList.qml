import bb.cascades 1.0

Page {
    property alias title: titleBarGeneric.title
    property alias dataModel: listView.dataModel
    titleBar: TitleBar {
        id: titleBarGeneric
    }
    Container {
        ListView {
            id: listView
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            listItemComponents: ListItemComponent {
                StandardListItem {
                    title: ListItemData.friendlyName
                    description: ListItemData.typeName
                }
            }
            onTriggered: {
                var nextList = genericList.createObject();

                var data = dataModel.data(indexPath);
                
                if (data["type"] == 0) {
                    nextList.title = "Devices/Services";
                    nextList.dataModel = _viewController.getGenericDataModelByID(data["id"]);
                    navPaneDevices.push(nextList);
                } else if (data["type"] == 1) {
                    nextList.title = "Actions";
                    nextList.dataModel = _viewController.getGenericDataModelByID(data["id"]);
                    console.log(data["id"]);
                    console.log(_executableAction.whyIsFail());
                    navPaneDevices.push(nextList);
                } else if (data["type"] == 2) {
                    _viewController.getGenericDataModelByID(data["friendlyName"]);
                    var openInvocationAction = invocationAction.createObject()
                    openInvocationAction.labelDevice = _executableAction.getDeviceFriendlyName();
                    openInvocationAction.labelService = _executableAction.getServiceFriendlyName();
                    openInvocationAction.labelAction = _executableAction.getActionName();
                    navPaneDevices.push(openInvocationAction)
                }
            }
            attachedObjects: [
                ComponentDefinition {
                    id: genericList
                    source: "asset:///qml/GenericList.qml"
                },
                ComponentDefinition {
                    id: invocationAction
                    InvocationAction {}
                }
            ]
        }
    }
}
