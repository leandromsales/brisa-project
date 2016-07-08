#include <QQmlContext>
#include <QQuickView>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

#ifdef QT_HAS_WEBVIEW_BACKEND
#include <QtWebView>
#endif // QT_HAS_WEBVIEW_BACKEND

#include "controlpointbcu.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#ifdef QT_HAS_WEBVIEW_BACKEND
    QtWebView::initialize();
#endif // QT_HAS_WEBVIEW_BACKEND

    QQmlApplicationEngine engine;

    ControlPointBCU *controlPoint = new ControlPointBCU(&engine);
    controlPoint->start();
    controlPoint->discover();

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}
