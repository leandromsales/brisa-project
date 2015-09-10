import QtQuick 2.0
import QtQuick.Controls 1.3

Rectangle {

    property string iconPath
    property string title
    property string description
    property variant servicesModel

    id:root

    Rectangle {

        color:"transparent"

        anchors {
            fill:parent
            margins:20
        }

        Image {
            id: appIcon

            anchors {
                top : parent.top
                left : parent.left
            }

            width: parent.width/6
            height: width
            source: iconPath
        }

        Text {
            id: appTitle

            anchors {
                top : appIcon.top
                left : appIcon.right
                leftMargin: 20
            }

            text: qsTr(title)
            font.bold: true

            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft

            font.pixelSize: parent.height/18
        }

        Button {

            anchors.bottom : appIcon.bottom
            anchors.left: appTitle.left

            width:appTitle.contentWidth
            height: parent.height/10

            onClicked: manager.run(title)

            Text {
                anchors.fill: parent

                font.pixelSize: height/3

                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter

                color:"#444"
                text:"Run"
            }
        }

        Text {
            id: descriptionLabel

            anchors.top : appIcon.bottom
            anchors.left: parent.left

            text: qsTr("Description:")

            font.underline: true
            font.pixelSize: parent.height/20
        }

        ScrollView{
            id: descriptionScrollView

            width:parent.width
            height:parent.height/6

            frameVisible: true

            anchors.top : descriptionLabel.bottom
            anchors.left: parent.left

            anchors.topMargin: 10

            contentItem:Text {
                id: descriptionText

                text: qsTr(description)
                wrapMode: Text.WordWrap
                font.pixelSize: root.height/30
            }
        }

        Text {
            id: servicesLabel

            anchors.top : descriptionScrollView.bottom
            anchors.left: parent.left

            anchors.topMargin:5

            text: qsTr("Services:")

            font.underline: true
            font.pixelSize: parent.height/20
        }

        TableView {
            id:servicesTable

            anchors.top : servicesLabel.bottom
            anchors.left: parent.left

            anchors.topMargin:10

            width: parent.width
            height: parent.height/4


            TableViewColumn {
                role: "title"
                title: "Title"
                width: parent.width*(0.10)
            }
            TableViewColumn {
                role: "description"
                title: "Description"
                width: parent.width*(0.83)
            }
            model: servicesModel
        }

        Button {

            anchors{
                top : servicesTable.bottom
                left: parent.left
                topMargin:5
            }

            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                id:backButtonText
                text:"Back"

                font.pixelSize: height/3

                color:"#444"
            }

            width:parent.width/6
            height: parent.height/12

            onClicked: stackPages.pop();
        }
    }
}

