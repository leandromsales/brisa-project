#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTemporaryFile>
#include <QDomDocument>
#include <QDebug>

#include "devicesales.h"

namespace brisa {
namespace upnp {
namespace controlpoint {

DeviceSales::DeviceSales(QObject *parent) : QObject(parent) {
    this->downloader = new QNetworkAccessManager(this);
    connect(downloader, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    this->serviceProcessedCounter = 0;
}

DeviceSales::DeviceSales(const QString &uuid,
               const QString &location,
               QObject *parent) : QObject(parent) {
    this->setAttribute("uuid", uuid);
    this->setAttribute("location", location);
    this->baseUrl = baseUrl;
    this->serviceProcessedCounter = 0;
    this->downloader = new QNetworkAccessManager(this);
    connect(downloader, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

DeviceSales::DeviceSales(const DeviceSales &device) : QObject(device.parent()) {
    *this = device;
    //this->downloader = new QNetworkAccessManager(this);
    //connect(downloader, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

DeviceSales &DeviceSales::operator=(const DeviceSales &device) {
    if (this != &device) {
        this->attributes = device.getAttributes();
        this->serviceList = device.getServices();
        this->iconList = device.getIcons();
    }
    return *this;
}

DeviceSales::~DeviceSales() {
    delete this->downloader;
    qDeleteAll(this->iconList.values());
    qDeleteAll(this->serviceList.values());
    qDeleteAll(this->embeddedDeviceList);
}

bool DeviceSales::setAttribute(const QString &attributeName, const QString &attributeValue) {
    this->attributes[attributeName.toLower()] = attributeValue;
    return true; // use this semantic for future validation
}

QString DeviceSales::getAttribute(const QString &attributeName) const {
    return this->attributes.value(attributeName.toLower());
}

QHash<QString, QString> DeviceSales::getAttributes() const {
    return this->attributes;
}

QHash<QString, ServiceSales *> DeviceSales::getServices() const {
    return this->serviceList;
}

ServiceSales *DeviceSales::getService(const QString &serviceId) const {
    return this->serviceList.value(serviceId);
}

void DeviceSales::parseDescription() {
    if (this->attributes["location"].isEmpty() || this->attributes["location"].isNull()) { return; }

    this->downloader->get(QNetworkRequest(QUrl(this->attributes["location"])));
}

void DeviceSales::replyFinished(QNetworkReply *reply) {
    QTemporaryFile *descriptionXml = new QTemporaryFile();
    if (!descriptionXml->open()) {
        emit errorParsingDeviceDescription(this, DeviceSales::CREATE_TMP_XML_ERROR);
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

quint8 DeviceSales::parseXMLDescription(QTemporaryFile *description) {
    QDomDocument document("Device");
    document.setContent(description);
    QDomElement element = document.documentElement();
    if (element.tagName() != "root") {
        return DeviceSales::MALFORMED_XML_DESCRIPTION_ERROR;
    }

    QDomNodeList domNodeList = element.elementsByTagName("specVersion");
    if (domNodeList.length() == 1) {
        QString major = domNodeList.item(0).toElement().elementsByTagName("major").item(0).toElement().text();
        QString minor = domNodeList.item(0).toElement().elementsByTagName("minor").item(0).toElement().text();
        if (major.isEmpty() || minor.isEmpty()) {
            return DeviceSales::WRONG_MAJOR_MINOR_VERSION_ERROR;
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
        return DeviceSales::INCOMPLETED_DEVICE_DESCRIPTION_ERROR;
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
                ServiceSales *service = new ServiceSales();
                connect(service,
                        SIGNAL(serviceDescriptionReady(ServiceSales*)),
                        this,
                        SLOT(handleServiceDescriptionReady(ServiceSales*)));
                connect(service,
                        SIGNAL(errorParsingServiceDescription(ServiceSales*, quint8)),
                        this,
                        SLOT(handleErrorParsingServiceDescription(ServiceSales*, quint8)));
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
                IconSales *icon = new IconSales();
                connect(icon,
                        SIGNAL(iconDownloaded(IconSales*)),
                        this,
                        SLOT(handleIconDownloaded(IconSales*)));
                connect(icon,
                        SIGNAL(errorDownloadingIcon(IconSales*, quint8)),
                        this,
                        SLOT(handleErrorDownloadingIcon(IconSales*, quint8)));
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

void DeviceSales::handleServiceDescriptionReady(ServiceSales* service) {
    this->serviceProcessedCounter++;
    this->serviceList[service->getAttribute("serviceId")] = service;
    if (this->serviceCount == this->serviceProcessedCounter) {
        this->serviceProcessedCounter = 0;
        qDebug() << "emit Device Description";
        emit deviceDescriptionReady(this);
    }
}

void DeviceSales::handleErrorParsingServiceDescription(ServiceSales* service, quint8 errorCode) {
    Q_UNUSED(errorCode);
    delete service;
}

void DeviceSales::handleIconDownloaded(IconSales* icon) {
    QString iconName = icon->getAttribute("name");
    this->iconList[iconName] = icon;
}

void DeviceSales::handleErrorDownloadingIcon(IconSales* icon, quint8 errorCode) {
    Q_UNUSED(errorCode);
    QString iconName = icon->getAttribute("name");
    delete icon;
}

QHash<QString, IconSales *> DeviceSales::getIcons() const {
    return this->iconList;
}

IconSales *DeviceSales::getIcon(const QString &name) {
    return (this->iconList.contains(name))
            ? this->iconList.value(name)
            : NULL;
}

QList<DeviceSales *> DeviceSales::getDevices() const {
    return this->embeddedDeviceList;
}

}
}
}
