import bb.cascades 1.0
import app.info 1.0

Sheet {
    id: self

    attachedObjects: [
        ApplicationInfo {
            id: appInfo
        },
        Invocation {
            id: invocationReview
            query {
                invokeTargetId: "sys.appworld"
                uri: "appworld://content/37260887" // id FeetFit: 37260887
            }
        },
        Invocation {
            id: invocationAppWorld
            query {
                invokeTargetId: "sys.appworld"
                uri: "appworld://vendor/55406" // id vendor COMPELab: 55406
            }
        }
    ]

    Page {
        titleBar: TitleBar {
            title: "About"
            dismissAction: ActionItem {
                title: qsTr("Close")
                onTriggered: {
                    self.close()
                }
            }
        }
        ScrollView {
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill

            Container {
                topPadding: 20
                verticalAlignment: VerticalAlignment.Fill
                horizontalAlignment: HorizontalAlignment.Fill

                ImageView {
                    imageSource: "asset:///images/icon114x114.png"
                    preferredWidth: 150
                    preferredHeight: 180
                    scalingMethod: ScalingMethod.AspectFit
                    horizontalAlignment: HorizontalAlignment.Center
                }
                Container {
                    horizontalAlignment: HorizontalAlignment.Center
                    
                    Label {
                        text: "BRisa Universal Control Point"
                        textStyle.fontWeight: FontWeight.Bold
                    }
                }
                Container {
                    horizontalAlignment: HorizontalAlignment.Center

                    Label {
                        text: "Version: " + appInfo.version()
                        textStyle.fontWeight: FontWeight.Bold
                    }
                }
                Container {
                    horizontalAlignment: HorizontalAlignment.Center
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }

                    ImageView {
                        horizontalAlignment: HorizontalAlignment.Center
                        imageSource: "asset:///images/compe.png"
                        scalingMethod: ScalingMethod.AspectFit
                        preferredHeight: 120
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
                        preferredHeight: 120
                        preferredWidth: 325
                        onTouch: {
                        }
                    }
                }
                LabelLink {
                    text: "http://www.compelab.org/"
                    uri: "http://www.compelab.org/"
                    horizontalAlignment: HorizontalAlignment.Center
                }
                Divider {
                }
                Container {
                    leftPadding: 75.0
                    Label {
                        text: qsTr("Team")
                        textStyle.fontSize: FontSize.Large
                        textStyle.fontWeight: FontWeight.Bold
                    }
                    Container {
                        Label {
                            text: "Daniel San"
                        }
                    }
                    Container {
                        Label {
                            text: "Dieges Lima"
                        }
                    }
                    Container {
                        Label {
                            text: "Leandro de Sales"
                        }
                    }
                    Container {
                        Label {
                            text: "Rodrigo Peixoto"
                        }
                    }
                    Container {
                        Label {
                            text: "Thiago Ribeiro"
                        }
                    }
                }
                Divider {
                }
                Container {
                    horizontalAlignment: HorizontalAlignment.Center
                    Button {
                        text: "More Apps"
                        imageSource: "asset:///images/ic_open.png"
                        horizontalAlignment: HorizontalAlignment.Fill

                        onClicked: {
                            invocationAppWorld.trigger("bb.action.OPEN")
                        }
                    }
                    Button {
                        text: "Post a Review"
                        imageSource: "asset:///images/ic_review.png"
                        horizontalAlignment: HorizontalAlignment.Fill

                        onClicked: {
                            invocationReview.trigger("bb.action.OPEN")
                        }
                    }
                }
                Label {
                    text: "Copyright (c) 2014 COMPELab"
                    textStyle.fontSize: FontSize.Small
                    verticalAlignment: VerticalAlignment.Bottom
                    horizontalAlignment: HorizontalAlignment.Center
                }
            }
        }
    }
}
