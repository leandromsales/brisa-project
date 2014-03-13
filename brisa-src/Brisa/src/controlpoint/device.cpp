#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTemporaryFile>
#include <QDomDocument>
#include <QDebug>

#include "device.h"

namespace brisa {
namespace upnp {
namespace controlpoint {

Device::Device(QObject *parent) : QObject(parent) {
    this->downloader = new QNetworkAccessManager(this);
    connect(downloader, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    this->serviceProcessedCounter = 0;
}

Device::Device(const QString &uuid,
               const QString &location,
               QObject *parent) : QObject(parent) {
    this->setAttribute("uuid", uuid);
    this->setAttribute("location", location);
    this->baseUrl = baseUrl;
    this->serviceProcessedCounter = 0;
    this->downloader = new QNetworkAccessManager(this);
    connect(downloader, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

Device::Device(const Device &device) : QObject(device.parent()) {
    *this = device;
    //this->downloader = new QNetworkAccessManager(this);
    //connect(downloader, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

Device &Device::operator=(const Device &device) {
    if (this != &device) {
        this->attributes = device.getAttributes();
        this->serviceList = device.getServices();
        this->iconList = device.getIcons();
    }
    return *this;
}

Device::~Device() {
    delete this->downloader;
    qDeleteAll(this->iconList.values());
    qDeleteAll(this->serviceList.values());
    qDeleteAll(this->embeddedDeviceList);
}

bool Device::setAttribute(const QString &attributeName, const QString &attributeValue) {
    this->attributes[attributeName.toLower()] = attributeValue;
    return true; // use this semantic for future validation
}

QString Device::getAttribute(const QString &attributeName) const {
    return this->attributes.value(attributeName.toLower());
}

QHash<QString, QString> Device::getAttributes() const {
    return this->attributes;
}

QHash<QString, Service *> Device::getServices() const {
    return this->serviceList;
}

Service *Device::getService(const QString &serviceId) const {
    return this->serviceList.value(serviceId);
}

void Device::parseDescription() {
    if (this->attributes["location"].isEmpty() || this->attributes["location"].isNull()) { return; }

    this->downloader->get(QNetworkRequest(QUrl(this->attributes["location"])));
}

void Device::replyFinished(QNetworkReply *reply) {
    QTemporaryFile *descriptionXml = new QTemporaryFile();
    if (!descriptionXml->open()) {
        emit errorParsingDeviceDescription(this, Device::CREATE_TMP_XML_ERROR);
    } else {
        descriptionXml->write(reply->readAll());
        descriptionXml->seek(0);
        QUrl url = reply->url();
        this->baseUrl = QString("%1://%2/").arg(url.scheme(), url.authority());

        quint8 result = this->parseXMLDescription(descriptionXml);
        if (!result) {
            emit errorParsingDeviceDescription(this, result);
        }

        descriptionXml->remove();
        delete descriptionXml;

    }
    reply->deleteLater();
}

quint8 Device::parseXMLDescription(QTemporaryFile *description) {
    QDomDocument document("Device");
    document.setContent(description);
    QDomElement element = document.documentElement();
    if (element.tagName() != "root") {
        return Device::MALFORMED_XML_DESCRIPTION_ERROR;
    }

    QDomNodeList domNodeList = element.elementsByTagName("specVersion");
    if (domNodeList.length() == 1) {
        QString major = domNodeList.item(0).toElement().elementsByTagName("major").item(0).toElement().text();
        QString minor = domNodeList.item(0).toElement().elementsByTagName("minor").item(0).toElement().text();
        if (major.isEmpty() || minor.isEmpty()) {
            return Device::WRONG_MAJOR_MINOR_VERSION_ERROR;
        }
        this->attributes["major"] = major;
        this->attributes["minor"] = minor;
    }

    domNodeList = element.elementsByTagName("URLBase");
    if (domNodeList.length() > 0) {
        QString baseUrl = domNodeList.at(0).toElement().text();
        if (!baseUrl.isEmpty()) {
            this->baseUrl = baseUrl;
        }
    }

    domNodeList = element.elementsByTagName("device");
    if (domNodeList.length() == 0) {
        return Device::INCOMPLETED_DEVICE_DESCRIPTION_ERROR;
    }

    QDomNode node;
    domNodeList = domNodeList.item(0).childNodes();
    for (int i = 0; i < domNodeList.length(); i++) {
        node = domNodeList.item(i);
        QString nodeName = node.nodeName();
        if (node.childNodes().length() == 1) {
            this->attributes[nodeName.toLower()] = node.childNodes().item(0).nodeValue();
        } else if (nodeName == "serviceList") {
            QDomNodeList serviceDomNodeList = node.childNodes();
            this->serviceCount = serviceDomNodeList.length();
            for (uint j = 0; j < this->serviceCount; j++) {
                Service *service = new Service();
                connect(service,
                        SIGNAL(serviceDescriptionReady(Service*)),
                        this,
                        SLOT(handleServiceDescriptionReady(Service*)));
                connect(service,
                        SIGNAL(errorParsingServiceDescription(Service*, quint8)),
                        this,
                        SLOT(handleErrorParsingServiceDescription(Service*, quint8)));
                QDomNodeList serviceAttribDomNodeList = serviceDomNodeList.item(j).childNodes();
                for (int k = 0; k < serviceAttribDomNodeList.length(); k++) {
                    service->setAttribute(serviceAttribDomNodeList.item(k).nodeName().toLower(),
                                          serviceAttribDomNodeList.item(k).childNodes().item(0).nodeValue());
                }
                service->setBaseUrl(this->baseUrl);
                this->serviceList.insert(service->getAttribute("serviceId"), service);
            }
        } else if (nodeName == "iconList") {
            QDomNodeList iconDomNodeList = node.childNodes();
            QString iconName = "";
            for (int j = 0; j < iconDomNodeList.length(); j++) {
                Icon *icon = new Icon();
                connect(icon,
                        SIGNAL(iconDownloaded(Icon*)),
                        this,
                        SLOT(handleIconDownloaded(Icon*)));
                connect(icon,
                        SIGNAL(errorDownloadingIcon(Icon*, quint8)),
                        this,
                        SLOT(handleErrorDownloadingIcon(Icon*, quint8)));
                QDomNodeList iconAttribDomNodeList = iconDomNodeList.item(j).childNodes();
                for (int k = 0; k < iconAttribDomNodeList.length(); k++) {
                    icon->setAttribute(iconAttribDomNodeList.item(k).nodeName().toLower(),
                                       iconAttribDomNodeList.item(k).childNodes().item(0).nodeValue());
                }
                icon->setBaseUrl(this->baseUrl);
                iconName = QString("%1_%2x%3").arg(icon->getAttribute("mimetype"),
                                                   icon->getAttribute("width"),
                                                   icon->getAttribute("height"));
                icon->setAttribute("name", iconName);

                this->iconList.insert(iconName, icon);
                //icon->downloadIt();
            }
        } else if (nodeName == "deviceList") {
            //TODO: parse embedded device
        }
    }
    return true;
}

void Device::handleServiceDescriptionReady(Service* service) {
    this->serviceProcessedCounter++;
    this->serviceList[service->getAttribute("serviceId")] = service;
    if (this->serviceCount == this->serviceProcessedCounter) {
        this->serviceProcessedCounter = 0;
        qDebug() << "emit Device Description";
        emit deviceDescriptionReady(this);
    }
}

void Device::handleErrorParsingServiceDescription(Service* service, quint8 errorCode) {
    Q_UNUSED(errorCode);
    //qDebug() << "SERVICE PARSING ERROR: " << errorCode << " parsing service description " << service->getAttribute("serviceId") << "for DEVICE " << this->getAttribute("uuid");
    delete service;
}

void Device::handleIconDownloaded(Icon* icon) {
    QString iconName = icon->getAttribute("name");
    this->iconList[iconName] = icon;
    //qDebug() << "ICON READY: " << iconName;
}

void Device::handleErrorDownloadingIcon(Icon* icon, quint8 errorCode) {
    Q_UNUSED(errorCode);
    QString iconName = icon->getAttribute("name");
    qDebug() << "ICON DOWNLOADING ERROR: " << errorCode << " from URL " << icon->getBaseUrl() << icon->getAttribute("url") << " name " << iconName;
    delete icon;
}

QHash<QString, Icon *> Device::getIcons() const {
    return this->iconList;
}

Icon *Device::getIcon(const QString &name) {
    return (this->iconList.contains(name))
            ? this->iconList.value(name)
            : NULL;
}

QList<Device *> Device::getDevices() const {
    return this->embeddedDeviceList;
}

}
}
}
