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
    QDir dir(QCoreApplication::applicationDirPath());
    if(!dir.cdUp())
        qFatal(qPrintable("ERROR : COULDNT FIND .. directory. Current Directory : " + dir.absolutePath()));
    if(!dir.cd("BRisaCentralApps"))
        qFatal(qPrintable("ERROR : COULDNT FIND BRisaCentralApps directory. Current Directory : " + dir.absolutePath()));
    if(!dir.cd("apps"))
        qFatal(qPrintable("ERROR : COULDNT FIND apps directory. Current Directory : " + dir.absolutePath()));
    BCADevice *bcaDevice = new BCADevice(engine,dir.absolutePath().toLatin1());
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
