#include "controlpointbcu.h"

namespace brisa {

using namespace network;
using namespace shared::ssdp;
using namespace shared::webserver;

namespace upnp {
namespace controlpoint {

ControlPointBCU::ControlPointBCU(QObject *parent, QString st, int mx) :
    QObject(parent) {

    engine.rootContext()->setContextProperty(QString("manager"), this);
    engine.rootContext()->setContextProperty(QString("myModel"),
                                             QVariant::fromValue(dataList));
    engine.rootContext()->setContextProperty(QString("dtS"), new DataObject());
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    this->discoverNetworkAddress();
    this->buildUrlBase();
    this->deliveryPath = 0;
    this->running = false;
    this->webserver = new Webserver(QHostAddress(ipAddress), port);
    this->multicastReceiver = new MulticastEventReceiver(parent);
    m_networkAccessManager = new QNetworkAccessManager(this);
    this->ssdpClient = new SSDPClient(this);
    msearch = new MSearchClientCP(this, st, mx);
    downloader = new QNetworkAccessManager();
    webserver->start();

    connect(m_networkAccessManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(httpResponse(QNetworkReply*)));
    connect(ssdpClient, SIGNAL(removedDeviceEvent(QString)),
            this, SLOT(deviceRemoved(QString)));
    connect(ssdpClient, SIGNAL(newDeviceEvent(QString, QString, QString, QString, QString, QString)),
            this, SLOT(deviceFound(QString, QString, QString, QString, QString, QString)));
    connect(msearch, SIGNAL(msearchResponseReceived(QString, QString, QString, QString, QString, QString)),
            this, SLOT(deviceFound(QString, QString, QString, QString, QString, QString)));
    connect(downloader, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
    connect(this->multicastReceiver, SIGNAL(multicastReceived(QMap<QString,QString>)),
            this, SLOT(receiveMulticast(QMap<QString,QString>)));
    this->multicastReceiver->start();
}

ControlPointBCU::~ControlPointBCU() {
    if (!isRunning())
        this->stop();

    delete this->downloader;
    delete this->msearch;
    delete this->ssdpClient;
    delete this->webserver;
    delete m_networkAccessManager;
    delete this->multicastReceiver;
}

void ControlPointBCU::start() {
    if (isRunning()) {
        qDebug() << "BCU: already started.";
    } else {
        this->ssdpClient->start();
        this->msearch->start();
        this->running = true;
    }
}

void ControlPointBCU::stop() {
    if (!isRunning()) {
        qDebug() << "BCU: already stopped.";
    } else {
        this->ssdpClient->stop();
        this->msearch->stop();
        this->running = false;
    }
}

bool ControlPointBCU::isRunning() {
    return this->running;
}

void ControlPointBCU::discover() {
    this->msearch->discover();
}

void ControlPointBCU::replyFinished(QNetworkReply *reply) {
    QTemporaryFile *rootXml = new QTemporaryFile();
    // qDebug() << reply->readAll();
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
        foreach (QObject* q, dataList){
            DataObject * d = (DataObject *) q;
            if (d->getUdn() == udn) {
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
                Action * info = s->getAction(QString("getAppInfo"));
                Action * app = s->getAction(QString("getApp"));

                // melhorar isso
                // if 3 actions exists, break foreach
                if (list != 0 && info != 0 && app != 0) {
                    this->auxServ = s;
                    has3gets = true;
                    break;
                }
            }

            // if has3gets (it means, if 3 actions exists), go on
            // otherwise, this device is not compatible with bcu, so ignore it
            if (has3gets) {
                foreach(Service *s, serviceList) {
                    s->setAttribute(Service::Host, urlBase->host());
                    s->setAttribute(Service::Port,
                                    QString().setNum(urlBase->port()));
                }

                addAppOnDataList(udn, "", "", QUrl(""), QUrl(""));

                rootXml->remove();
                delete rootXml;
                delete urlBase;
                reply->deleteLater();

                connect(this->auxServ, SIGNAL(requestFinished(OutArgument, QString)),
                        this, SLOT(serviceCall(OutArgument, QString)));
                connect(this->auxServ, SIGNAL(requestError(QString, QString)),
                        this, SLOT(requestError(QString,QString)));

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

void ControlPointBCU::deviceFound(QString, QString location, QString,
                                  QString, QString, QString) {
    downloader->get(QNetworkRequest(QUrl(location)));
}

void ControlPointBCU::deviceRemoved(const QString udn) {
    removeAppFromDataList(udn);
    emit deviceGone(udn);
}

void ControlPointBCU::buildUrlBase() {
    QString sPort;
    sPort.setNum(this->port);
    this->urlBase = "http://" + ipAddress + ":" + sPort;
}

void ControlPointBCU::discoverNetworkAddress() {
    this->port = getPort();
    this->ipAddress = getValidIP();
    qDebug() << "BCU: Acquired Address " << this->ipAddress
                + ":" + QString::number(this->port);
}

EventProxy *ControlPointBCU::getSubscriptionProxy(Service *service) {
    //    deliveryPath++;
    //    EventProxy *subscription = new EventProxy(QStringList(this->urlBase),
    //                                              webserver, deliveryPath,
    //                                              service->getAttribute(Service::Host),
    //                                              service->getAttribute(Service::Port).toInt(),
    //                                              m_networkAccessManager,
    //                                              service->getAttribute(Service::EventSubUrl));

    //    requests[deliveryPath] = subscription;

    //    webserver->addService("/", subscription);

    //    return subscription;
    return NULL;
}

void ControlPointBCU::run(QString appURL)
{
    auxAppURL = appURL;
    // improve here to use name of app, not compe-file
    FileDownloader *fd = new FileDownloader(QUrl(appURL), "compe-file", this);
    connect(fd, SIGNAL (ready()), this, SLOT (finishedGetApp()));

}

void ControlPointBCU::execApp(QString appURL)
{
    qDebug() << "-----------------------";
    qDebug() << appURL;
    appURL.replace(".compe", "/main.qml");
    qDebug() << appURL;
    qDebug() << "-----------------------";
    QQmlComponent window(&engine);
    window.loadUrl(QUrl(appURL));

    QObject *stack = engine.rootObjects()[0]->findChild<QObject *>("stack");
    QQuickItem *object = qobject_cast<QQuickItem*>(window.create(engine.rootContext()));
    object->setParentItem(qobject_cast<QQuickItem*>(engine.rootObjects()[0]->findChild<QObject *>("appExec")));
    object->setParent(&engine);

    QMetaObject::invokeMethod(stack,"pushObject");
}

void ControlPointBCU::addAppOnDataList(QString udn, QString name, QString info, QUrl iconURL, QUrl appURL)
{
    dataList.insert(dataList.size() - 1, new DataObject(udn, name, info, iconURL, appURL));

    engine.rootContext()->setContextProperty(QString("myModel"),
                                             QVariant::fromValue(dataList));
}

void ControlPointBCU::removeAppFromDataList(QString udn)
{
    foreach (QObject* q, dataList){
        DataObject * d = (DataObject *) q;
        if (d->getUdn() == udn) {
            dataList.removeOne(q);
        }
    }

    engine.rootContext()->setContextProperty(QString("myModel"),
                                             QVariant::fromValue(dataList));
}

void ControlPointBCU::httpResponse(QNetworkReply *networkReply) {
    EventProxy *subscription = NULL;
    foreach(int deliveryPath, requests.keys()) {
        if (requests[deliveryPath]->getId().toUtf8() == networkReply->request().rawHeader("ID")) {
            subscription = requests[deliveryPath];
            break;
        }
    }

    if (!subscription) {
        qWarning() << "BCU: Failed to match response";
        return;
    } else if (networkReply->error()) {
        // TODO forward error to user, notify that subscription didn't work
        qWarning() << "BCU: Subscription error "  << networkReply->errorString();
        return;
    }

    QString sid = networkReply->rawHeader("SID");
    if (sid.isEmpty())
        // Try "sid"
        sid = networkReply->hasRawHeader("sid");
    if (sid.isEmpty()) {
        // TODO report subscription error to user
        qWarning() << "BCU: SID header not present on event subscription response.";
        QList<QByteArray> rawHeaderList = networkReply->rawHeaderList();
        foreach(QByteArray rawHeader, rawHeaderList) {
            qDebug() << rawHeader;
        }
        qDebug() << "BCU: Finished printing headers.. printing data" << networkReply->readAll();
        return;
    }

    // subscription->setSid(sid);
    qDebug() << "BCU: Subscribed with SID " << subscription->getSid();
}

void ControlPointBCU::receiveMulticast(OutArgument attributes)
{
    emit multicastReceived(attributes.value("variableName"), attributes.value("newValue"));
    emit multicastReceivedRaw(attributes);
}

void ControlPointBCU::serviceCall(OutArgument arguments, QString method)
{
    QString returnMessage = "";
    QMapIterator<QString, QString> it(arguments);
    while (it.hasNext()) {
        it.next();
        returnMessage.append(it.value());
    }

    // qDebug() << "Calling method: " << method << "Returned: \n" << returnMessage;

    this->jsonMsg = returnMessage;

    if (method == "getListOfApps") {
        decodeJsonList();
    } else if (method == "getAppInfo") {
        decodeJsonInfo();
    }

}

void ControlPointBCU::requestError(QString errorMessage, QString methodName)
{
    qDebug() << errorMessage  << " when calling " << methodName;
}

void ControlPointBCU::add()
{
    addAppOnDataList(auxDO->getUdn(), auxDO->getName(), auxDO->getInfo(),
                     auxDO->getIconURL(), auxDO->getAppURL());
    delete auxDO;
}

void ControlPointBCU::finishedGetApp()
{
    connect(this, SIGNAL (decompressed()), this, SLOT (decompressedFinished()));

    FolderCompressor *fc = new FolderCompressor();
    // improved here to not use full path, but a relative one
    QString path = "/home/larissa/UFAL/Compelab/git/brisa-project/platforms/qt/apps/BCU/files/";
    QDir dir(path);
    QStringList listCompeFiles = dir.entryList();

    bool status;
    foreach (QString file, listCompeFiles) {
        if (file.endsWith(".compe")) {
            QString fullPath = file.prepend(path);
            QString filename = fullPath;
            filename.replace(".compe", "");
            status = fc->decompressFolder(fullPath, filename);
            QFile (fullPath).remove();

            qDebug() << "BCU: " << status << "\n" << fullPath << "\n" << filename;
        }
    }

    if (status) {
        emit decompressed();
    } else {
        qDebug() << "BCU: invalid file";
    }
}

void ControlPointBCU::decompressedFinished()
{
    qDebug() << "descompressed";
    QQmlComponent window(&engine);
    qDebug() << "descompressed1";
    // improve this path, absolute path is bad!
    QString path = "/home/larissa/UFAL/Compelab/git/brisa-project/platforms/qt/apps/BCU/files/compe-file/main.qml";
    qDebug() << "descompressed2";
    window.loadUrl(QUrl(path));
    qDebug() << "a";
    QObject *stack = engine.rootObjects()[0]->findChild<QObject *>("stack");
    qDebug() << "a1";
    QQuickItem *object = qobject_cast<QQuickItem*>(window.create(engine.rootContext()));
    qDebug() << "a2";
    object->setParentItem(qobject_cast<QQuickItem*>(engine.rootObjects()[0]->findChild<QObject *>("appExec")));
    qDebug() << "a3";
    object->setParent(&engine);
    qDebug() << "a4";
    QMetaObject::invokeMethod(stack,"pushObject");
    qDebug() << "a5";
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

    // get more info about each app
    for(int i = 0; i < listApps.length(); i++) {
        QMap<QString,QVariant> app = listApps.at(i).toMap();

        QMap<QString, QString> param;
        param["SelectedApp"] = app["Title"].toString();

        this->auxServ->call("getAppInfo", param);
    }
}

void ControlPointBCU::decodeJsonInfo()
{
    // decode JSON
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(this->jsonMsg.toLatin1(), &error);
    if (error.errorString() != "no error occurred") {
        qDebug() << error.errorString();
    }

    // adding founded apps on grid
    QVariantHash app = doc.object().toVariantHash();

    QString udn = auxDev->getAttribute(auxDev->udn);
    QString name = app["Title"].toString();
    QString info = app["Description"].toString();
    QString appUrl = app["Url"].toString();

    QString iconUrl;

    if (app["Icon"].toString().startsWith("file://")) {
        iconUrl = app["Icon"].toString();
        addAppOnDataList(udn, name, info, QUrl(iconUrl), QUrl(appUrl));
    } else {
        iconUrl = "pics/" + name.replace(" ", "") + ".png";
        QUrl url(iconUrl);
        auxDO = new DataObject(udn, name, info, QUrl(iconUrl), QUrl(appUrl));
        FileDownloader *fd = new FileDownloader(url, name.replace(" ", ""), this);
        connect(fd, SIGNAL (ready()), this, SLOT (add()));
    }
}
}

}
}
