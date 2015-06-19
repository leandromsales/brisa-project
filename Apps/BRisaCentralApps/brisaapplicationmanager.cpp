#include "brisaapplicationmanager.h"

BRisaApplicationManager::BRisaApplicationManager()
{
    numOfApps = 0;

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


