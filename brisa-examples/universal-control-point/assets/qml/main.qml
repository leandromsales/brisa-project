import bb.cascades 1.0

TabbedPane {
    id: mainScreen
    
    showTabsOnActionBar: false
    
    tabs: [
        Tab {
            title: "Universal Control Point"
            description: "Select media server"
            imageSource: "qrc:///assets/images/media_server_ic.png" 
            content: ControlPoint {
            	
            }
        },
        Tab {
         	title: "Others"   
         	description: "Select media renderer"
            imageSource: "qrc:///assets/images/media_renderer_ic.png"
            content: MediaServerScreen {
                
            }
        }      
    ]
}