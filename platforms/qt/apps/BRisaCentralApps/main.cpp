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
    QQmlContext *ctxt = engine.rootContext();

    BRisaApplicationManager *manager = new BRisaApplicationManager(engine,"../BRisaCentralApps/apps");

    BCADevice *bca = new BCADevice(manager);
    bca->printAllApps();

    ctxt->setContextProperty("manager", manager);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
