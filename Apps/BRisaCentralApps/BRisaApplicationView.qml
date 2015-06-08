import QtQuick 2.0
import QtQuick.Controls 1.3

Rectangle {
    id:root
    anchors.fill: parent
    anchors.margins: 20


    property string iconPath
    property string title
    property string description
    property variant servicesModel

    Image {
        id: appIcon

        anchors.top : parent.top
        anchors.left: parent.left


        width: parent.width/5
        height: parent.height/4
        source: iconPath
    }

    Text {
        id: appTitle

        anchors.top : parent.top
        anchors.left : parent.left

        anchors.leftMargin: parent.width/4

        text: qsTr(title)
        font.bold: true

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft

        font.pixelSize: parent.height/18
    }

    Text {
        id: descriptionLabel

        anchors.top : parent.top
        anchors.left: parent.left

        anchors.topMargin: parent.height/3.5

        text: qsTr("Description:")

        font.underline: true
        font.pixelSize: parent.height/20
    }

    ScrollView{
        id: descriptionScrollView

        width:parent.width
        height:parent.height/6

        frameVisible: true

        anchors.top : parent.top
        anchors.left: parent.left

        anchors.topMargin: parent.height/2.8

        contentItem:Text {
            id: descriptionText

            text: qsTr(description)
            wrapMode: Text.WordWrap
            font.pixelSize: root.height/30
        }
    }

    Text {
        id: servicesLabel

        anchors.top : parent.top
        anchors.left: parent.left

        anchors.topMargin: parent.height*(.52)

        text: qsTr("Services:")

        font.underline: true
        font.pixelSize: parent.height/20
    }

    TableView {
        id:servicesTable

        anchors.top : parent.top
        anchors.left: parent.left

        anchors.topMargin: parent.height*(.6)

        width: parent.width
        height: parent.height/4


        TableViewColumn {
            role: "title"
            title: "Title"
            width: parent.width*(0.46)
        }
        TableViewColumn {
            role: "description"
            title: "Description"
            width: parent.width*(0.46)
        }
        model: servicesModel
    }

    Button {

        anchors.bottom : parent.bottom
        anchors.left: parent.left

        anchors.leftMargin: 0
        anchors.bottomMargin: 0

        width:parent.width/5
        height: parent.height/10

        text:"Back"

        onClicked: mainRoot.state="menu"
    }
}

