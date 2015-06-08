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

void BRisaApplicationManager::addApp(QObject *app)
{
    apps.append(app);
    numOfApps++;
}

