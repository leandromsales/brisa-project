#include <QQmlApplicationEngine>
#include <QApplication>
#include <QQmlContext>
#include "assets/cpp/bcadevice.h"

#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
#include <QtWebEngine>
#endif // QT_WEBVIEW_WEBENGINE_BACKEND

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
    QtWebEngine::initialize();
#endif // QT_WEBVIEW_WEBENGINE_BACKEND

    QQmlApplicationEngine engine;
    BCADevice *bcaDevice = new BCADevice(engine,"../BRisaCentralApps/apps");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
