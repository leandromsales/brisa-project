import bb.cascades 1.2
import bb.cascades.pickers 1.0

Page {
    property string selectedUdn
    titleBar: TitleBar {
        title: "BRisa Switch Power"
    }
    function getDeviceUdn() {
        return selectedUdn
    }
    Container {
        Header {
            id: hdrDevices
            title: "Devices list"
            subtitle: "Selected: null"
        }
        bottomPadding: 20
        ListView {
            dataModel: _app.dataModel
            listItemComponents: ListItemComponent {
                type: "item"
                StandardListItem {
                    title: pickerItemData["friendlyName"]
                }
            }
            onTriggered: {
                var data = dataModel.data(indexPath)
                selectedUdn = data["udn"]
                hdrDevices.subtitle = "Selected: " + data["friendlyName"]
            }
        }
        Container {
            id: cnTest
            topMargin: 10
            leftPadding: 10
            rightPadding: 10
            bottomPadding: 10
            Picker {
                title: "Device"
                visible: false
                dataModel: _app.dataModel
                pickerItemComponents: PickerItemComponent {
                    type: "item"
                    StandardPickerItem {
                        title: pickerItemData["friendlyName"]
                    }
                }
                onSelectedValueChanged: {
                    var index = selectedIndex(0)
                    console.log("InvokeActions.qml", 32, index)
                }
            }
        }
        Container {
            Divider {
            }
        }
        Header {
            title: "GetTarget"
        }
        Container {
            topPadding: 20
            leftPadding: 20
            rightPadding: 20
            bottomPadding: 20
            horizontalAlignment: HorizontalAlignment.Fill
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                layout: DockLayout {
                }
                Label {
                    text: "Current target:"
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    verticalAlignment: VerticalAlignment.Center
                }
                ToggleButton {
                    id: tglBtnResult
                    enabled: false
                    horizontalAlignment: HorizontalAlignment.Right
                }
            }
            Button {
                property bool firstClick: true
                text: "Get"
                horizontalAlignment: HorizontalAlignment.Fill
                onClicked: {
                    if (firstClick) {
                        firstClick = false
                    	_app.resultGetTargetChanged.connect(tglBtnResult.setChecked)
                    }
                    _app.getTarget(selectedUdn)
                }
            }
        }
        Container {
            Divider {
            }
        }
        Header {
            title: "SetTarget"
        }
        Container {
            topPadding: 20
            leftPadding: 20
            rightPadding: 20
            horizontalAlignment: HorizontalAlignment.Fill
            layout: DockLayout {
            }
            Label {
                text: "New target:"
                textStyle.base: SystemDefaults.TextStyles.TitleText
                verticalAlignment: VerticalAlignment.Center
            }
            ToggleButton {
                horizontalAlignment: HorizontalAlignment.Right
                onCheckedChanged: {
                    _app.setTarget(selectedUdn, checked)
                }
            }
        }
    }
}
