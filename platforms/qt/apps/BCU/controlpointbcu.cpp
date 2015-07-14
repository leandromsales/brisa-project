#include "controlpointbcu.h"

namespace brisa {

using namespace network;
using namespace shared::ssdp;
using namespace shared::webserver;

namespace upnp {
namespace controlpoint {

ControlPointBCU::ControlPointBCU(QObject *parent, QString st, int mx) :
    QObject(parent) {

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
        if (!hasUdn && deviceType == "org.compelab.AppServer:1") {
            bool has3gets = false;
            QList<Service*> serviceList = device->getServiceList();

            // foreach service, search by actions getListOfApps, getAppInfo, getApp
            foreach (Service *s, serviceList) {
                Action * list = s->getAction(QString("getListOfApps"));
                Action * info = s->getAction(QString("getAppInfo"));
                Action * app = s->getAction(QString("getApp"));

                // if 3 actions exists, break foreach
                if (list != 0 && info != 0 && app != 0) {
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

                rootXml->remove();
                delete rootXml;
                delete urlBase;
                reply->deleteLater();

                // adding founded app on grid
                QString name = device->getAttribute(device->FriendlyName);
                QString info = device->getAttribute(device->ModelDescription);
                addAppOnDataList(udn, name, info, QUrl("icon4"), QUrl("url4"));

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

}
}
}
