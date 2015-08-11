import bb.cascades 1.2

Container {
    property alias firstText: first.text
    property alias secondText: second.text
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }
    Container {
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1
        }
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        Container {
            Label {
                id: first
                text: "Title"
                textStyle.fontWeight: FontWeight.Bold
            }
        }
        Container {
            Label {
                text: ":"
                textStyle.fontWeight: FontWeight.Bold
            }
        }
    }
    Container {
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1
        }
        Label {
            id: second
            text: "Uninformed"
            multiline: true
            onTextChanged: {
                if (text.length == 0)
                    text = "Uninformed"
            }
        }
    }
}
