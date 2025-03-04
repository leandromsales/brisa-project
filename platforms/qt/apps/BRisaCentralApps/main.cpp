#include <QQmlApplicationEngine>
#include <QApplication>
#include <QQmlContext>

#include "assets/cpp/bcajson.h"
#include "assets/cpp/bcadevice.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QQmlContext *ctxt = engine.rootContext();

    BRisaApplicationManager *manager = new BRisaApplicationManager(engine);

    QDir dir("../BRisaCentralApps/apps");
    QStringList listApps = dir.entryList();

    for(int i = 2; i < listApps.size(); i++) {

        BCAJson json(qPrintable(dir.absoluteFilePath(listApps[i]) + "/" + "description.json"));
        QString icon = "file:///" + dir.absoluteFilePath(listApps[i]) + "/icon.png";
        QString url = "file:///" + dir.absoluteFilePath(listApps[i]) + "/main.qml";

        manager->addApp(new BRisaApplication(icon, listApps[i], url, json.toBRisaApp()));
    }

    BCADevice *bca = new BCADevice(manager);
    bca->printAllApps();

    ctxt->setContextProperty("manager", manager);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
