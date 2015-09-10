import QtQuick 2.2
import QtQuick.Controls 1.3
import QtWebView 1.0

Rectangle {

	anchors.fill : parent
    
    Rectangle {
        id:gamePanel

        color:"transparent"

        anchors.fill: parent

        WebView {
            id: webView
            anchors.fill: parent
            url: "https://play.spotify.com/"
        }
    }
}

