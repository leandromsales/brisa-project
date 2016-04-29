#include "controlpointbcu.h"

ControlPointBCU::ControlPointBCU(QQmlApplicationEngine *e, QObject *parent, QString st, int mx) :
    ControlPoint(parent,st,mx) {

    m_engine = e;

    QDir m_dir;
    if(!m_dir.cd("files"))
        if(!m_dir.mkdir("files")) qFatal( "ERROR ON FILES DIR CREATION!");
    m_dir.cd("files");

    m_apps = new QQmlObjectListModel<DataObject>(this);
    m_engine->rootContext()->setContextProperty("filesFolder", m_dir.absolutePath().prepend("file://"));
    m_engine->rootContext()->setContextProperty(QString("manager"), this);
    m_engine->rootContext()->setContextProperty(QString("dtS"), new DataObject());
    downloader = new QNetworkAccessManager();

    connect(ssdpClient, SIGNAL(removedDeviceEvent(QString)),
            this, SLOT(deviceRemoved(QString)));
    connect(downloader, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

void ControlPointBCU::replyFinished(QNetworkReply *reply) {
    QTemporaryFile *rootXml = new QTemporaryFile();
    if (!rootXml->open()) {
        qWarning() << "BCU: Failed to open file for writing root XML.";
    } else {
        rootXml->write(reply->readAll());
        rootXml->seek(0);
        QUrl *urlBase = new QUrl(reply->url());

        Device *device = new Device(rootXml, urlBase);

        // check if this device was already recognized
        bool hasUdn = false;
        QString udn = device->getAttribute(device->udn);
        for (int i=0;i<m_apps->size();i++){
            DataObject * d = m_apps->at(i);
            if (d->get_udn() == udn) {
                hasUdn = true;
                break;
            }
        }

        QString deviceType = device->getAttribute(device->deviceType);
        if (!hasUdn && deviceType == "urn:org.compelab.AppServer:1") {
            bool has3gets = false;
            QList<Service*> serviceList = device->getServiceList();

            // foreach service, search by actions getListOfApps, getAppInfo, getApp
            foreach (Service *s, serviceList) {
                Action * list = s->getAction(QString("getListOfApps"));
                // Action * info = s->getAction(QString("getAppInfo"));
                // Action * app = s->getAction(QString("getApp"));

                // if 3 actions exists, break foreach
                // if (list != 0 && info != 0 && app != 0) {
                if (list != 0) {
                    this->auxServ = s;
                    has3gets = true;
                    break;
                } else qFatal("COULDNT FIND THE ACTION GETLIST OF APPS");
            }

            // if has3gets (it means, if 3 actions exists), go on
            // otherwise, this device is not compatible with bcu, so ignore it
            if (has3gets) {
                foreach(Service *s, serviceList) {
                    s->setAttribute(Service::Host, urlBase->host());
                    s->setAttribute(Service::Port,
                                    QString().setNum(urlBase->port()));
                }

                addAppOnDataList(udn, QString(""), QString(""), QUrl(""), QUrl(""));

                rootXml->remove();
                delete rootXml;
                delete urlBase;
                reply->deleteLater();

                connect(this->auxServ, SIGNAL(requestFinished(OutArgument, QString)),
                        this, SLOT(serviceCall(OutArgument, QString)), Qt::UniqueConnection);
                connect(this->auxServ, SIGNAL(requestError(QString, QString)),
                        this, SLOT(requestError(QString,QString)), Qt::UniqueConnection);

                InArgument parameters;
                this->auxServ->call("getListOfApps", parameters);
                this->auxDev = device;

                emit deviceFound(device);
            } else {
                qDebug() << "BCU: Incompatible device - device hasn't 3 gets";
            }

        } else {
            qDebug() << "BCU: Incompatible device - device hasn't correct deviceType";
        }
    }
}

void ControlPointBCU::deviceRemoved(const QString udn) {
    removeAppFromDataList(udn);
    emit deviceGone(udn);
}

void ControlPointBCU::requestError(QString errorMessage, QString methodName)
{
    qDebug() << errorMessage  << " when calling " << methodName;
}

void ControlPointBCU::run(QString appURL, QString name)
{
    auxAppURL = appURL;
    auxAppName = name;
    QStringList filesList = m_dir.entryList();
    bool downloadStatus = false;

    foreach (QString file, filesList) {
        if (file == name) {
            downloadStatus = true;
            qDebug() << "BCU: already downloaded" << name;
        }
    }

    qDebug() << "STATUS CODE :: " << downloadStatus;

    if (downloadStatus) {
        FileDownloader *fd = new FileDownloader(&m_dir,QUrl(appURL), name, this);
        connect(fd, SIGNAL(ready(QString)), this, SLOT (finishedGetApp(QString)),Qt::UniqueConnection);
    }
    else
        decompressedFinished();
}

bool ControlPointBCU::deleteApp(QString name)
{
    QStringList filesList = m_dir.entryList();

    bool status;
    foreach (QString file, filesList) {
        if (file == name) {
            m_dir.cd(file);
            status = m_dir.removeRecursively();
            m_dir.cdUp();

            qDebug() << "BCU: delete app - " << status << "\n" << m_dir.absolutePath();
        }
    }

    return status;
}

void ControlPointBCU::addAppOnDataList(QString udn, QString name, QString info, QUrl iconURL, QUrl appURL)
{
    bool status = true;
    for (int i = 0; i < m_apps->size(); i++) {
        DataObject *dobj =m_apps->at(i);
        if (dobj->get_udn() == udn && dobj->get_name() == "") {
            m_apps->remove(i);
            m_apps->insert(i, new DataObject(udn, name, info, iconURL, appURL));
            status = false;
        }
    }
    if (status)
        m_apps->append(new DataObject(udn, name, info, iconURL, appURL));

    qDebug() << "BCU: addded app - " << name;
}

void ControlPointBCU::removeAppFromDataList(QString udn)
{
    for (int i = 0; i < m_apps->size(); i++) {
        DataObject * d = m_apps->at(i);
        if (d->get_udn() == udn) {
            m_apps->remove(d);
        }
    }
}

void ControlPointBCU::serviceCall(OutArgument arguments, QString method)
{
    QString returnMessage = "";
    QMapIterator<QString, QString> it(arguments);
    while (it.hasNext()) {
        it.next();
        returnMessage.append(it.value());
    }

    this->jsonMsg = returnMessage;

    if (method == "getListOfApps") {
        decodeJsonList();
    } else {
        qDebug() << "BCU: I receive an unexpected call for method" << method;
    }

}

void ControlPointBCU::finishedGetApp(QString dirPath)
{
    FolderCompressor *fc = new FolderCompressor();

    QDir filesPath(dirPath);
    bool status;

    foreach (QString f, filesPath.entryList(QDir::Files)) {
        if (f.endsWith(".compe")) {
            QString fullPath = filesPath.absoluteFilePath(f);
            QString filename = fullPath;
            filename.replace(".compe", "");
            status = fc->decompressFolder(fullPath, ".");

            QFile f(fullPath);
            f.remove();

            qDebug() << "BCU: " << status << "\n" << fullPath << "\n" << filename;
            break;
        }
    }
    //Corrigir erro no download do arquivo - pasta fora de files
    if (status) {
        decompressedFinished();
    } else {
        qDebug() << "BCU: invalid file";
    }
}

void ControlPointBCU::decompressedFinished()
{
    QString path = m_dir.absolutePath();
    path.append(auxAppName + "/main.qml");
    QObject *appLoader = m_engine->rootObjects()[0]->findChild<QObject *>("appLoader");
    appLoader->setProperty("appPath","file://"+auxAppName);
    emit decompressed();
}

void ControlPointBCU::decodeJsonList()
{
    // decode JSON
    QString json = this->jsonMsg;
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(json.toLatin1(), &error);
    if (error.errorString() != "no error occurred") {
        qDebug() << error.errorString();
    }

    QList<QVariant>	listApps = doc.object().toVariantHash()["Apps"].toList();
    QString udn = auxDev->getAttribute(auxDev->udn);

    // get more info about each app
    for(int i = 0; i < listApps.length(); i++) {
        QMap<QString,QVariant> app = listApps.at(i).toMap();

        // QMap<QString, QString> param;
        // param["SelectedApp"] = app["Title"].toString();

        QString name = app["Title"].toString();
        QString info = app["Description"].toString();
        QString appUrl = app["Url"].toString();
        QString iconUrl = app["Icon"].toString();

        QUrl url(iconUrl);
        FileDownloader *fd = new FileDownloader(&m_dir, url, name.replace(" ", ""), this);

        auxDO = new DataObject(udn, name, info, url, QUrl(appUrl));
        // auxDO = new DataObject(udn, name, info, QUrl("qrc:/pics/qtlogo.png"), QUrl(appUrl));
        addAppOnDataList(udn, name, info, url, QUrl(appUrl));

        auxAppName = name;
    }
}
