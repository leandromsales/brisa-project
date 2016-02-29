#include <QQmlContext>
#include <QQuickView>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
#include <QtWebEngine>
#endif // QT_WEBVIEW_WEBENGINE_BACKEND

#include "controlpointbcu.h"

using namespace brisa;
using namespace upnp;
using namespace controlpoint;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
    QtWebEngine::initialize();
#endif // QT_WEBVIEW_WEBENGINE_BACKEND

    ControlPointBCU *controlPoint = new ControlPointBCU();
    controlPoint->start();
    controlPoint->discover();

    return app.exec();
}
