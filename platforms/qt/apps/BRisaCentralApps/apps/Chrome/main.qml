import QtQuick 2.2
import QtQuick.Controls 1.3
import QtWebView 1.0

Rectangle {

	anchors.fill : parent

    Rectangle {

        anchors.fill: parent

        WebView {
            id: webView
            anchors.fill: parent
            url: "http://www.google.com.br"
        }
    }
}


