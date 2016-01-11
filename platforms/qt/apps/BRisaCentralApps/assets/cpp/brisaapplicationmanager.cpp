#include "brisaapplicationmanager.h"

BRisaApplicationManager::BRisaApplicationManager(QQmlApplicationEngine &engine)
{
    m_numOfApps = 0;
    ctxt = engine.rootContext();
}
QList<QObject *> BRisaApplicationManager::getListApps()
{
    return apps;
}

QString BRisaApplicationManager::getCurrentAppDir()
{
    return currentAppDir;
}

BRisaApplication *BRisaApplicationManager::getAppByName(QString appName)
{
    foreach (QObject *obj, apps) {
        BRisaApplication *app = (BRisaApplication *)obj;

        if(app->get_title().toLower() == appName.toLower()) {
            return app;
        }
    }

    return 0;
}

void BRisaApplicationManager::addApp(QObject *app)
{
    apps.append(app);
    m_numOfApps++;
}

void BRisaApplicationManager::run(QString name)
{
    QQuickView *view = new QQuickView(QUrl(getAppByName(name)->get_url()));

    QString dirPath = getAppByName(name)->get_iconPath();
    dirPath.resize(dirPath.size() - 9);

    currentAppDir = dirPath;

    view->show();
}


