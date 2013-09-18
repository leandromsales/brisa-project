import bb.cascades 1.2

TabbedPane {
    id: mainScreen
    
    showTabsOnActionBar: false
    
    tabs: [
        Tab {
            title: "Universal Control Point"
            description: "Select control point device"
            imageSource: "asset:///images/media_server_ic.png" 
            NavigationPane {
                id: navPane
                ControlPoint {
            	
            	}
            }
        },
        Tab {
         	title: "Event Log"   
//         	description: "Select others"
             imageSource: "asset:///images/media_renderer_ic.png"
            content: EventLog {
                
            }
        },
        Tab {
            title: "Action result"
            description: ""
            content: ActionResult {
                
            }
        }
    ]
}