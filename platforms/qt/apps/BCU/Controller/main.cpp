#include <QQmlContext>
#include <QQuickView>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
#include <QtWebEngine>
#endif // QT_WEBVIEW_WEBENGINE_BACKEND

#include "controlpointbcu.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
    QtWebEngine::initialize();
#endif // QT_WEBVIEW_WEBENGINE_BACKEND

    QQmlApplicationEngine engine;

    ControlPointBCU *controlPoint = new ControlPointBCU(&engine);
    controlPoint->start();
    controlPoint->discover();

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}
