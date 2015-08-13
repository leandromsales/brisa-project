#include "brisaapplicationmanager.h"

BRisaApplicationManager::BRisaApplicationManager(QQmlApplicationEngine &engine)
{
    numOfApps = 0;
    mainEngine = &engine;
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
    QQmlComponent window(mainEngine);
    window.loadUrl(QUrl(getAppByName(name)->getUrl()));

    QObject *stack = mainEngine->rootObjects()[0]->findChild<QObject *>("stack");

    QQuickItem *object = qobject_cast<QQuickItem*>(window.create(mainEngine->rootContext()));
    qDebug() << mainEngine->rootObjects()[0]->findChild<QObject *>("appExec");

    object->setParentItem(qobject_cast<QQuickItem*>(mainEngine->rootObjects()[0]->findChild<QObject *>("appExec")));
    object->setParent(mainEngine);

    QMetaObject::invokeMethod(stack,"pushObject");

    qDebug() << "OK";
}


