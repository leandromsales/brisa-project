import bb.cascades 1.2

Container {
    id: self
    property string text
    property string uri
    
    Label {
        id: label
        property bool touchIsDown: false
        text: "<html><span style='text-decoration:underline'>" + self.text + "</span></html>"
        textStyle {
            base: SystemDefaults.TextStyles.BigText
            fontSize: FontSize.Large
            color: Color.Blue
        }
        
        attachedObjects: [ // attach to the page or something
            Invocation {
                id: linkInvocation
                property bool auto_trigger: false
                
                onArmed: {
                    // don't auto-trigger on initial setup
                    if (auto_trigger)
                        trigger("bb.action.OPEN");
                    auto_trigger = true; // allow re-arming to auto-trigger
                }
                
                query {
                    uri: self.uri
                    invokeTargetId: "sys.browser"
                }
            }
        ]
        
        onTouch: {
            if (event.touchType == TouchType.Up && touchIsDown) {
                linkInvocation.query.updateQuery()
                touchIsDown = false
            } else if (event.touchType == TouchType.Down) {
                touchIsDown = true
            }
        }
    }
    
}
