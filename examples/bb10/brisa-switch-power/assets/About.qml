import bb.cascades 1.0
//import app.info 1.0

Sheet {
    id: self
    onClosed: {
        self.destroy()
    }
    attachedObjects: [
//        ApplicationInfo {
//            id: appInfo
//        },
        Invocation {
            id: invocationReview
            query {
                invokeTargetId: "sys.appworld"
                uri: "appworld://content/49077888" // id Pro Timing: 49077888
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
        onCreationCompleted: {
            animation.play();
        }
        titleBar: TitleBar {
            title: "About"
            acceptAction: ActionItem {
                title: "Ok"
                onTriggered: {
                    self.close()
                }
            }
        }
        Container {
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            ScrollView {
                verticalAlignment: VerticalAlignment.Fill
                horizontalAlignment: HorizontalAlignment.Fill
                Container {
                    topPadding: 20
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                    ImageView {
                        imageSource: "asset:///images/icon.png"
                        preferredWidth: 150
                        preferredHeight: 180
                        scalingMethod: ScalingMethod.AspectFit
                        horizontalAlignment: HorizontalAlignment.Center
                        onTouch: {
                            if (event.isDown()) {
                                animation.play()
                            }
                        }
                        attachedObjects: [
                            SequentialAnimation {
                                id: animation
                                animations: [
                                    FadeTransition {
                                        duration: 1000
                                        easingCurve: StockCurve.CubicOut
                                        fromOpacity: 0.0
                                        toOpacity: 1.0
                                    },
                                    RotateTransition {
                                        toAngleZ: -20
                                        duration: 125
                                    },
                                    RotateTransition {
                                        toAngleZ: 20
                                        duration: 250
                                    },
                                    RotateTransition {
                                        toAngleZ: -20
                                        duration: 250
                                    },
                                    RotateTransition {
                                        toAngleZ: 20
                                        duration: 250
                                    },
                                    RotateTransition {
                                        toAngleZ: 0
                                        duration: 125
                                    }
                                ]
                            }
                        ]
                    }
                    Container {
                        horizontalAlignment: HorizontalAlignment.Center
                        Label {
                            text: qsTr("BRisa Switch Power")
                            textStyle.fontWeight: FontWeight.Bold
                            horizontalAlignment: HorizontalAlignment.Center
                        }
                    }
                    Container {
                        horizontalAlignment: HorizontalAlignment.Center
                        Label {
//                            text: qsTr("Version: ") + appInfo.version()
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
                            preferredWidth: 350
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
                                text: "Leandro de Sales"
                            }
                        }
                        Container {
                            Label {
                                text: "Wendell Soares"
                            }
                        }
                    }
                    Divider {
                    }
                    //                    Container {
                    //                        leftPadding: 75.0
                    //                        Label {
                    //                            text: qsTr("Credits")
                    //                            textStyle.fontSize: FontSize.Large
                    //                            textStyle.fontWeight: FontWeight.Bold
                    //                        }
                    //                        Container {
                    //                            Label {
                    //                                text: "Myers Design Limited (Icons)"
                    //                            }
                    //                        }
                    //                    }
                    //                    Divider {
                    //                    }
                    Container {
                        horizontalAlignment: HorizontalAlignment.Center
                        Button {
                            text: "More Apps"
                            imageSource: "asset:///images/ic_open_black.png"
                            horizontalAlignment: HorizontalAlignment.Fill

                            onClicked: {
                                invocationAppWorld.trigger("bb.action.OPEN")
                            }
                        }
                        Button {
                            text: "Post a Review"
                            imageSource: "asset:///images/ic_review_black.png"
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
}