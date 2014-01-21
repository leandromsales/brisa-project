import bb.cascades 1.0

Sheet {
    id: aboutSheet
    Page {

        titleBar: TitleBar {
            dismissAction: ActionItem {
                title: qsTr("Close")
                onTriggered: {
                    aboutSheet.close()
                }
            }
        }
        Container {
            background: Color.White
            ScrollView {
                verticalAlignment: VerticalAlignment.Fill
                horizontalAlignment: HorizontalAlignment.Fill
                Container {
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                    Container {
                        horizontalAlignment: HorizontalAlignment.Center
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        ImageView {
                            horizontalAlignment: HorizontalAlignment.Center
                            imageSource: "asset:///images/compe.png"
                            scalingMethod: ScalingMethod.AspectFit
                            preferredHeight: 200
                            preferredWidth: 275
                            onTouch: {
                            }
                        }
                        Label {
                        }
                        ImageView {
                            horizontalAlignment: HorizontalAlignment.Center
                            imageSource: "asset:///images/techcenter.png"
                            scalingMethod: ScalingMethod.AspectFit
                            preferredHeight: 200
                            preferredWidth: 325
                            onTouch: {
                            }
                        }
                    }
                    Divider {
                    }
                    ImageView {
                        imageSource: "asset:///images/onlight.png"
                        preferredWidth: 150
                        preferredHeight: 180
                        scalingMethod: ScalingMethod.AspectFit
                        horizontalAlignment: HorizontalAlignment.Center
                    }
                    Label {
                        text: "BRisa Binary Light - V 1.3.5"
                        textStyle.fontWeight: FontWeight.Bold
                        horizontalAlignment: HorizontalAlignment.Center
                    }
                    Divider {
                    }
                    Container {
                        leftPadding: 75.0
                        // horizontalAlignment: HorizontalAlignment.Center
                        Label {
                            text: qsTr("Developers")
                            textStyle.fontSize: FontSize.XLarge
                            textStyle.fontWeight: FontWeight.Bold
                        }
                        Container {
                            Label {
                                text: "Daniel San"
                                textStyle.fontSize: FontSize.Large
                            }
                        }
                        Container {
                            Label {
                                text: "Wendell Soares"
                                textStyle.fontSize: FontSize.Large
                            }
                        }
                        Label {
                        }
                    }
//                    Divider {
//                    }
//                    Container {
//                        leftPadding: 75.0
//                        horizontalAlignment: HorizontalAlignment.Center
//                        Label {
//                            text: qsTr("Designer")
//                            textStyle.fontSize: FontSize.Large
//                            textStyle.color: Color.Black
//                        }
//                        Container {
//                            Label {
//                                text: "Dieges Lima"
//                                textStyle.color: Color.Black
//                            }
//                        }
//                    }
                }
            }
        }
    }

}
