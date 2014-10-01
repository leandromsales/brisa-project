import bb.cascades 1.2

Container {
    property alias firstText: first.text
    property alias secondText: second.text
    Container {
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
        Label {
            id: second
            text: "Uninformed"
            multiline: true
            onTextChanged: {
                if (text.length == 0)
                    second.text = "Uninformed"
            }
        }
    }
}
