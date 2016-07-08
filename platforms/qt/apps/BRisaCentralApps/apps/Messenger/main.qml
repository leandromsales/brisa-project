import QtQuick 2.6
import QtQuick.Controls 2.0
import QtWebView 1.1

Item {
	WebView {
		anchors.fill:parent
		url:Qt.resolvedUrl("https://www.messenger.com/")
	}
}