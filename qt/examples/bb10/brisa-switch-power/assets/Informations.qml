import bb.cascades 1.2

Page {
    titleBar: TitleBar {
        title: "BRisa Switch Power"
    }
    function setDeviceUdn(udn) {
        var infos = _app.getDeviceInformations(udn)
        lblFriendly.secondText = infos["friendlyName"]
        lblManu.secondText = infos["manufacturer"]
        lblManuUrl.secondText = infos["manufacturerUrl"]
        lblModelDesc.secondText = infos["modelDescription"]
        lblSerialNumber.secondText = infos["serialNumber"]
        lblUrlBase.secondText = infos["urlBase"]
        lblUdn.secondText = infos["udn"]
        lblModelUrl.secondText = infos["modelUrl"]
        lblType.secondText = infos["deviceType"]
    }
    Container {
        Header {
            title: "Informations"
        }
        ScrollView {
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            Container {
                leftPadding: 20
                rightPadding: 20
                verticalAlignment: VerticalAlignment.Fill
                horizontalAlignment: HorizontalAlignment.Fill
                VDoubleLabel {
                    id: lblFriendly
                    firstText: "Friendly name"
                }
                VDoubleLabel {
                    id: lblManu
                    firstText: "Manufacturer"
                }
                VDoubleLabel {
                    id: lblManuUrl
                    firstText: "Manufacturer URL"
                }
                VDoubleLabel {
                    id: lblModelDesc
                    firstText: "Model description"
                }
                VDoubleLabel {
                    id: lblSerialNumber
                    firstText: "Serial number"
                }
                VDoubleLabel {
                    id: lblUrlBase
                    firstText: "Url base"
                }
                VDoubleLabel {
                    id: lblUdn
                    firstText: "UDN"
                }
                VDoubleLabel {
                    id: lblType
                    firstText: "Device type"
                }
                VDoubleLabel {
                    id: lblModelUrl
                    firstText: "Model URL"
                }
            }
        }
    }
}
