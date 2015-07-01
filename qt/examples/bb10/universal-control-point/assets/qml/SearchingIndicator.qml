import bb.cascades 1.0

Container {
    leftPadding: 20
    rightPadding: 20
    bottomPadding: 20
    horizontalAlignment: HorizontalAlignment.Fill
    layout: DockLayout {
    }
    Label {
        text: "Searching"
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Left
    }
    ActivityIndicator {
        running: true
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Right        
    }
}