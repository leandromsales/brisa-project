#include "brisaapplicationmanager.h"

BRisaApplicationManager::BRisaApplicationManager(QQmlApplicationEngine &engine, QByteArray dirPath, QString ip, QString port)
{
    m_numOfApps = 0;
    mainEngine = &engine;
    ctxt = engine.rootContext();
    m_dirPath = dirPath;
    m_ip = ip;
    m_port = port;

    QDir dir(dirPath);
    if(!generateJSONFile()) { qDebug() << "ERROR TO GENERATE APPSJSON"; return; }
    if(!readJSONFile())  { qDebug() << "ERROR TO READ APPSJSON"; return; }
}

bool BRisaApplicationManager::generateJSONFile()
{
    QDir dir(m_dirPath);
    QFile appsJson(dir.absoluteFilePath("apps.json"));
    if(!appsJson.exists()) {
        QStringList listApps = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);
        QJsonObject *mainJson = new QJsonObject();
        QJsonArray *jsonArrayApps = new QJsonArray();
        foreach (QString app, listApps) {
            QJsonObject *jsonApp = new QJsonObject();
            dir.cd(app);

            QFile iconFile(dir.absoluteFilePath("icon.png"));
            if(!iconFile.exists()) {
                qDebug() << "App :" << app << ", the icon.png doesn't exist";
                dir.cdUp();
                continue;
            }
            QFile descriptionFile(dir.absoluteFilePath("description.json"));
            if(!descriptionFile.exists()) {
                qDebug() << "App :" << app << ", the description.json doesn't exist";
                dir.cdUp();
                continue;
            }

            jsonApp->insert("appName",app);
            jsonApp->insert("dirPath",dir.absolutePath());
            jsonApp->insert("descriptionFile",dir.absoluteFilePath("description.json"));
            jsonApp->insert("iconPath",dir.absoluteFilePath("icon.png"));
            jsonArrayApps->append(*jsonApp);

            dir.cdUp();
        }
        mainJson->insert("apps",*jsonArrayApps);
        QJsonDocument *jsonDoc = new QJsonDocument(*mainJson);
        if(!appsJson.open(QIODevice::WriteOnly)) {
            qDebug() << "ERROR OPEN APPSJSON";
            return false;
        }
        appsJson.write(jsonDoc->toJson());
        appsJson.close();
    }
    return true;
}

bool BRisaApplicationManager::readJSONFile()
{
    QDir dir(m_dirPath);
    QFile appsJson(dir.absoluteFilePath("apps.json"));
    if(!appsJson.exists()) generateJSONFile();
    if(!appsJson.open(QIODevice::ReadOnly)) {
        qDebug() << "APPSJSON COULDN'T BE OPENED!";
        return false;
    }
    QJsonDocument jsonDoc = QJsonDocument::fromJson(appsJson.readAll());
    appsJson.close();
    QJsonArray apps = jsonDoc.object()["apps"].toArray();
    foreach (QJsonValue app, apps) {
        addApp(new BRisaApplication(app.toObject().toVariantMap()));
    }
}

void BRisaApplicationManager::refreshAppList()
{
    QDir dir(m_dirPath);
    if(!dir.exists()) { qDebug() << "M_DIRPATH DOESN'T EXISTS!"; return; }
    if(!dir.remove("apps.json")) { qDebug() << "ERROR: REMOVE APPSJSON FILE!"; return; }
    m_apps.clear(); m_numOfApps = 0;
    if(!generateJSONFile()) { qDebug() << "ERROR TO GENERATE APPSJSON"; return; }
    if(!readJSONFile())  { qDebug() << "ERROR TO READ APPSJSON"; return; }
    qDebug() << "REFRESH ENDED!" << m_numOfApps;
    emit listWasUpdated();
}

bool BRisaApplicationManager::fileExists(QString path)
{
    QFile file(path);
    return file.exists();
}

bool BRisaApplicationManager::createAnApp(QJSValue theApp)
{
    QDir dir(m_dirPath);
    if(!dir.mkdir(theApp.property("name").toString())) {
        qDebug() << "The folder already exists!";
        return false;
    }

    qDebug() << "Folder Created with Sucess!";

    dir.cd(theApp.property("name").toString());

    qDebug() << "Change the folder to " + theApp.property("name").toString();

    QFile iconFile(theApp.property("icon").toString());

    if(!iconFile.exists()) {
        qDebug() << "The icon selected doesn't exists " + iconFile.fileName();;
        if(!QFile::copy(":/img/icon.png", dir.absoluteFilePath("icon.png"))) {
            qDebug() << "Error in the copy of the Icon " + iconFile.fileName();
            return false;
        }
    } else
        if(!QFile::copy("/" + iconFile.fileName(), dir.absoluteFilePath("icon.png"))) {
            qDebug() << "Error in the copy of the Icon " + iconFile.fileName();
            return false;
        }

    qDebug() << "Icon copied with sucess!";

    if(theApp.property("appType").toString() == "QML") {
        QDir mainQMLFolder("/" + theApp.property("execPath").toString());

        QStringList qmlFiles = mainQMLFolder.entryList(QDir::Files);

        foreach (QString f, qmlFiles) {
            QFile file(dir.absoluteFilePath(f));
            if (!file.exists()) {
                if(!QFile::copy(mainQMLFolder.absoluteFilePath(f), dir.absoluteFilePath(f))) {
                    qDebug() << "Error in the copy of the files! File : " + dir.absoluteFilePath(f);
                    return false;
                } else qDebug() << "The file " + f + " was copied with sucess!";
            }
        }

        qDebug() << "Files copied with Sucess!";
    }

    QJsonObject json;
    json["Title"] = theApp.property("name").toString();
    json["Type"] = theApp.property("appType").toString() + "App";
    json["Description"] = theApp.property("description").toString();

    if(theApp.property("appType").toString() == "QML")
        json["execPath"] = "main.qml";
    else json["execPath"] = theApp.property("execPath").toString();

//    QJsonArray services;
//    QList<QVariant> appServices = theApp.property("services").toVariant().toList();
//    foreach (QVariant service, appServices) {
//        QJsonObject aux;
//        aux[service.toMap()["title"].toString()] = service.toMap()["description"].toString();
//        services.append(aux);
//    }
//    json["Services"] = services;

    QFile jsonFile (dir.absolutePath() + "/description.json");
    QJsonDocument jsonDoc(json);

    jsonFile.open(QIODevice::WriteOnly);
    jsonFile.write(jsonDoc.toJson());

    qDebug() << "JSON Created with Sucess!";
    return true;
}

bool BRisaApplicationManager::removeAnApp(QByteArray appName)
{
    QDir dir(m_dirPath);
    if(!dir.cd(appName)) {
        qDebug() << "DIR NOT FOUND!";
        return false;
    }

    QStringList files = dir.entryList(QDir::NoDotAndDotDot | QDir::Files);
    QStringList dirs = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);

    foreach (QString f, files) {
        bool reply = dir.remove(f);
        if(!reply) {
            qDebug() << "ERROR REMOVE FILE :: " << f;
            return false;
        }
    }
    foreach (QString d, dirs) {
        bool reply = dir.rmdir(d);
        if(!reply) {
            qDebug() << "ERROR REMOVE DIR :: " << d;
            return false;
        }
    }

    if(!dir.cdUp()) qDebug() << "ERROR CDUP DIR :: " << appName; return false;
    if(!dir.rmdir(appName)) qDebug() << "ERROR REMOVE DIR :: " << appName; return false;
    return true;
}

void BRisaApplicationManager::run(QString name, int type)
{
    QObject *qmlInterpreter = mainEngine->rootObjects()[0]->findChild<QObject *>("qmlInterpreter");
    qmlInterpreter->setProperty("mainQMLSource",QUrl("file:///" + getAppByName(name)->get_execPath()));
    emit mainQMLPathSetted();
}

BRisaApplication *BRisaApplicationManager::getAppByName(QString appName)
{
    foreach (QObject *obj, m_apps) {
        BRisaApplication *app = (BRisaApplication *)obj;
        if(app->get_title().toLower() == appName.toLower()) return app;
    }
    return 0;
}

QJsonObject BRisaApplicationManager::toJson()
{
    QJsonArray jsonListOfApps;

    foreach (QObject *obj, get_apps()) {
        QJsonObject jsonApp;

        BRisaApplication *app = (BRisaApplication *) obj;

        jsonApp.insert("Title",QJsonValue(app->get_title()));
        jsonApp.insert("Icon",app->get_iconRelPath());
        jsonApp.insert("Info", QJsonValue(app->get_description()));
        QString url = m_ip + ":" + m_port + "/apps/" + app->get_title() + ".compe";
        jsonApp.insert("Url", QJsonValue(url));

        jsonListOfApps.append(jsonApp);
    }

    QJsonObject mainJsonObj;
    mainJsonObj.insert("Apps",QJsonValue(jsonListOfApps));
    return mainJsonObj;
}

void BRisaApplicationManager::addApp(QObject *app) { m_apps.append(app); m_numOfApps++; }


