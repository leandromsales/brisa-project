import bb.cascades 1.0

TabbedPane {
    id: mainScreen
    showTabsOnActionBar: true
    Menu.definition: MenuDefinition {
        helpAction: HelpActionItem {
            onTriggered: {
                helpSheet.createObject().open();
            }
        }
        actions: [
            ActionItem {
                title: "About"
                imageSource: "asset:///images/ic_info.png"
                onTriggered: {
                    aboutSheet.createObject().open();
                }
            },
            ActionItem {
                title: "Share this app"
                imageSource: "asset:///images/ic_bbm.png"
                onTriggered: {
                    _viewController.callBBMCard();
                }
            }
        ]
    }
    tabs: [
        Tab {
            title: "Devices"
            description: "Select a control point device"
            imageSource: "asset:///images/media_server_ic.png"
            NavigationPane {
                id: navPaneDevices
                onPopTransitionEnded: {
                    _viewController.onPopPage();
                }
                DevicesList {
                }
            }
        },
        Tab {
            title: "Event Log"
            description: "Show event log"
            imageSource: "asset:///images/ic_history.png"
            NavigationPane {
                id: navPaneEvent
                EventLog {
                }
            }
        }
    ]
    attachedObjects: [
        ComponentDefinition {
            id: helpSheet
            Help {
            }
        },
        ComponentDefinition {
            id: aboutSheet
            About {
            }
        }
    ]
}