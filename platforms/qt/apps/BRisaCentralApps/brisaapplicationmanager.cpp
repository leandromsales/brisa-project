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

bool BRisaApplicationManager::fileExists(QString path)
{
    QFile file(path);
    return file.exists();
}

bool BRisaApplicationManager::createAnApp(QJSValue theApp)
{
    QDir dir("../BRisaCentralApps/apps");

    if(!dir.mkdir(theApp.property("name").toString())) {
        qDebug() << "The folder already exists!";
        return false;
    }

    qDebug() << "Folder Created with Sucess!";

    dir.cd(theApp.property("name").toString());

    qDebug() << "Change the folder to " + theApp.property("name").toString();

    QFile iconFile(theApp.property("icon").toString());

    if(iconFile.exists()) {
        qDebug() << "The icon selected doesn't exists " + iconFile.fileName();;
        return false;
    }

    if(!QFile::copy("/" + iconFile.fileName(), dir.absoluteFilePath("icon.png"))) {
        qDebug() << "Error in the copy of the Icon " + iconFile.fileName();
        return false;
    }

    qDebug() << "Icon copied with sucess!";

    QDir mainQMLFolder("/" + theApp.property("mainPath").toString());

    QStringList qmlFiles = mainQMLFolder.entryList(QDir::Files);

    foreach (QString f, qmlFiles) {
        if(!QFile::copy(mainQMLFolder.absoluteFilePath(f), dir.absoluteFilePath(f))) {
            qDebug() << "Error in the copy of the files!";
            return false;
        } else {
            qDebug() << "The file " + f + " was copied with sucess!";
        }
    }

    qDebug() << "Files copied with Sucess!";

    QJsonObject json;
    json["Description"] = theApp.property("description").toString();

    QJsonArray services;

    QList<QVariant> titles = theApp.property("serviceTitles").toVariant().toList();
    QList<QVariant> descriptions = theApp.property("serviceDescriptions").toVariant().toList();

    for(int i = 0; i < titles.length(); i++) {
        QJsonObject aux;
        aux[titles.at(i).toString()] = descriptions.at(i).toString();
        services.append(aux);
    }

    json["Services"] = services;

    QFile jsonFile (dir.absolutePath() + "/description.json");
    QJsonDocument jsonDoc(json);

    jsonFile.open(QIODevice::WriteOnly);
    jsonFile.write(jsonDoc.toJson());

    qDebug() << "JSON Created with Sucess!";

    return true;
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
    window.loadUrl(QUrl(getAppByName(name)->getMainQMLFile()));

    QObject *stack = mainEngine->rootObjects()[0]->findChild<QObject *>("stack");

    qDebug() << window.status();
    QQuickItem *object = qobject_cast<QQuickItem*>(window.create(mainEngine->rootContext()));
    qDebug() << mainEngine->rootContext();
    qDebug() << object;

    object->setParentItem(qobject_cast<QQuickItem*>(mainEngine->rootObjects()[0]->findChild<QObject *>("appExec")));
    object->setParent(mainEngine);

    QMetaObject::invokeMethod(stack,"pushObject");

    qDebug() << "OK";
}


