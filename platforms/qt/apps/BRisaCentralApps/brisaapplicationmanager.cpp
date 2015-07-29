#include "brisaapplicationmanager.h"

BRisaApplicationManager::BRisaApplicationManager(QQmlApplicationEngine &engine)
{
    numOfApps = 0;
    ctxt = engine.rootContext();
}

BRisaApplicationManager::~BRisaApplicationManager()
{

}

QList<QObject *> BRisaApplicationManager::getListApps()
{
    return apps;
}

int BRisaApplicationManager::getNumOfApps()
{
    return numOfApps;
}

QString BRisaApplicationManager::getCurrentAppDir()
{
    return currentAppDir;
}

BRisaApplication *BRisaApplicationManager::getAppByName(QString appName)
{
    foreach (QObject *obj, apps) {
        BRisaApplication *app = (BRisaApplication *)obj;

        if(app->getTitle().toLower() == appName.toLower()) {
            return app;
        }
    }

    return 0;
}

void BRisaApplicationManager::addApp(QObject *app)
{
    apps.append(app);
    numOfApps++;
}

void BRisaApplicationManager::run(QString name)
{
    QQuickView *view = new QQuickView(QUrl(getAppByName(name)->getUrl()));

    QString dirPath = getAppByName(name)->getIconPath();
    dirPath.resize(dirPath.size() - 9);

    currentAppDir = dirPath;

    view->show();
}


