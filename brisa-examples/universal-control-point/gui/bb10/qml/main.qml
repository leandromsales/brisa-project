import bb.cascades 1.0

TabbedPane {
    id: mainScreen
    
    showTabsOnActionBar: false
    
    tabs: [
        Tab {
            title: "Media Servers"
            description: "Select media server"
            imageSource: "qrc:///gui/bb10/qml/images/media_server_ic.png" 
            content: MediaServerScreen {
                
            }
        },
        Tab {
         	title: "Media Renderers"   
         	description: "Select media renderer"
            imageSource: "qrc:///gui/bb10/qml/images/media_renderer_ic.png"
            content: MediaRendererScreen {
                
            }
        }      
    ]
}