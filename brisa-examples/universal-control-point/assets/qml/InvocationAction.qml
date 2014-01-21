import bb.system 1.0
import bb.cascades 1.0

Page {
    property alias labelDevice: deviceLabel.text
    property alias labelService: serviceLabel.text
    property alias labelAction: actionLabel.text
    property int i
    onCreationCompleted: {
        createArgumentsEntrys();
        _actionController.clearResult();
    }
    titleBar: TitleBar {
        title: "Invocation Action"
    }
    Container {
        leftPadding: 10
        rightPadding: 10
        horizontalAlignment: HorizontalAlignment.Fill
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            Label {
                text: "Device: "
                textStyle.fontWeight: FontWeight.Bold
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
            }
            Label {
                id: deviceLabel
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 4
                }
            }
        }
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            Label {
                text: "Service: "
                textStyle.fontWeight: FontWeight.Bold
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
            }
            Label {
                id: serviceLabel
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 4
                }
            }
        }
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            Label {
                text: "Action: "
                textStyle.fontWeight: FontWeight.Bold
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
            }
            Label {
                id: actionLabel
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 4
                }
            }
        }
        Divider {
        }
        SegmentedControl {
            id: inOutSwith
            Option {
                id: inSwith
                text: "In Arguments"
            }
            Option {
                id: outSwith
                text: "Results"
            }
            onSelectedIndexChanged: {
                if (selectedIndex == 0) {
                    dynamicContainer.visible = true
                    resultContainer.visible = false;
                } else {
                    dynamicContainer.visible = false
                    resultContainer.visible = true;
                }
            }
        }
        Container {
            layout: DockLayout {
            }
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            ScrollView {
                verticalAlignment: VerticalAlignment.Fill
                horizontalAlignment: HorizontalAlignment.Fill
                Container {
                    id: dynamicContainer
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                }
            }
            Container {
                id: resultContainer
                visible: false
                leftPadding: 10
                rightPadding: 10
                bottomPadding: 10
                verticalAlignment: VerticalAlignment.Fill
                horizontalAlignment: HorizontalAlignment.Fill
                layout: DockLayout {
                }
                ScrollView {
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                    Label {
                        id: labelResults
                        text: _actionController.getResult
                        multiline: true
                        onTextChanged: {
                            waitingInd.running = false;
                        }
                        contextActions: ActionSet {
                            title: "Result of the " + actionLabel.text
                            subtitle: "Options for this result"
                            actions: [
                                ActionItem {
                                    title: "Copy"
                                    imageSource: "asset:///images/ic_copy.png"
                                    onTriggered: {
                                        _actionController.copyText(labelResults.text);
                                    }
                                },
                                InvokeActionItem {
                                    title: "Share"

                                    query {
                                        mimeType: "text/plain"
                                        invokeActionId: "bb.action.SHARE"
                                    }

                                    onTriggered: {
                                        data = labelResults.text;
                                    }
                                },
                                DeleteActionItem {
                                    id: clearResultAction
                                    title: "Clear results"
                                    imageSource: "asset:///images/ic_clear.png"
                                    onTriggered: {
                                        clearConfirmation.show();
                                    }
                                }
                            ]
                        }
                    }
                }
                ActivityIndicator {
                    property int verify: 0
                    id: waitingInd
                    running: _actionController.haveResult
                    preferredWidth: 250
                    preferredHeight: 250
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                    onRunningChanged: {
                        if (! running) {
                            if (labelResults.text == "") {
                                systemToast.body = "Did not result!";
                                systemToast.show();
                            }
                        }
                    }
                }
            }
        }
    }
    actions: [
        ActionItem {
            title: "Invoke"
            imageSource: "asset:///images/ic_done.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                readArgumentsEntrys();
                systemToast.body = "This action has been invoked!";
                systemToast.show();
                if (outSwith.enabled) {
                    waitingInd.running = true;
                    inOutSwith.selectedIndex = 1;
                }
            }
        }
    ]
    attachedObjects: [
        ComponentDefinition {
            id: argumentComponents
            Container {
                property alias labelText: label.text
                bottomPadding: 10
                horizontalAlignment: HorizontalAlignment.Fill
                layout: DockLayout {
                }
                Label {
                    id: label
                    verticalAlignment: VerticalAlignment.Center
                }
                TextField {
                    maxWidth: 400
                    hintText: "Enter argument"
                    inputMode: TextFieldInputMode.EmailAddress
                    horizontalAlignment: HorizontalAlignment.Right
                }
            }
        },
        SystemToast {
            id: systemToast
        },
        SystemDialog {
            id: clearConfirmation
            title: "Clear result"
            body: "Clear the result of this action?"
            onFinished: {
                if (result == SystemUiResult.ConfirmButtonSelection) {
                    _actionController.clearResult();
                }
            }
        }
    ]
    function createArgumentsEntrys() {
        var arguments = _actionController.getArguments();
        var inArgs = arguments["in"];
        var outArgs = arguments["out"];

        for (var i = 0; i < inArgs.length; i ++) {
            inSwith.enabled = true
            var argComponent = argumentComponents.createObject();
            argComponent.labelText = inArgs[i]["name"] + ":";
            dynamicContainer.add(argComponent);
        }
        if (inArgs.length == 0) {
            inOutSwith.selectedIndex = 1;
            inSwith.enabled = false;
        }
        if (outArgs.length == 0) {
            outSwith.enabled = false;
            clearResultAction.enabled = false;
        }
    }
    function readArgumentsEntrys() {
        var arguments = {
        };
        arguments["action"] = actionLabel.text
        arguments["service"] = serviceLabel.text;
        var argumentsList = new Array();
        argumentsList.push(actionLabel.text);
        argumentsList.push(serviceLabel.text);
        for (var i = 0; i < dynamicContainer.controls.length; i ++) {
            var child = dynamicContainer.controls[i];
            argumentsList.push(child.controls[1].text);
            arguments[child.controls[0].text] = child.controls[1].text;
        }
        _actionController.callMethod(arguments);
    }
    function serviceText() {
        var service = serviceLabel.text;
        var num = service.split(":").length
        return service.split(":")[num - 2];
    }
}