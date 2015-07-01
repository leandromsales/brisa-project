#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTemporaryFile>

#include "iconsales.h"

namespace brisa {
namespace upnp {
namespace controlpoint {

IconSales::IconSales(QObject *parent) : QObject(parent) {
    this->content = NULL;
    this->downloader = new QNetworkAccessManager(this);
    connect(downloader, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

IconSales::IconSales(const QString &mimeType,
           const QString &width,
           const QString &height,
           const QString &depth,
           const QUrl &baseUrl,
           QObject *parent) : QObject(parent) {

    this->setAttribute("mimetype", mimeType);
    this->setAttribute("width", width);
    this->setAttribute("height", height);
    this->setAttribute("depth", depth);
    this->baseUrl = baseUrl;
    this->content = NULL;
    this->downloader = new QNetworkAccessManager(this);
    connect(downloader, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

IconSales::~IconSales() {
    if (this->content) {
        this->content->remove();
        delete this->content;
    }
    delete this->downloader;
}

IconSales::IconSales(const IconSales &icon) : QObject(icon.parent()) {
    *this = icon;
}

IconSales &IconSales::operator=(const IconSales &icon) {
    if (this != &icon) {
        this->attributes = icon.getAttributes();
    }
    return *this;
}

void IconSales::setBaseUrl(const QUrl &baseUrl) {
    this->baseUrl = baseUrl;
}

QUrl IconSales::getBaseUrl() {
    return this->baseUrl;
}

bool IconSales::setAttribute(const QString &attributeName, const QString &attributeValue) {
    this->attributes[attributeName.toLower()] = attributeValue;
    return true; // use this semantic for future validation
}

QString IconSales::getAttribute(const QString &attributeName) {
    return this->attributes.value(attributeName.toLower());
}

QHash<QString, QString> IconSales::getAttributes() const {
    return this->attributes;
}

QUrl IconSales::getUrl() {
    QUrl url(this->attributes.value("url"));
    url = this->baseUrl.resolved(url);
    return url;
}

bool IconSales::downloadIt() {
    QUrl url = this->getUrl();
    if (url.isValid()) {
        this->downloader->get(QNetworkRequest(url));
        return true;
    }
    return false;
}

QTemporaryFile *IconSales::getContent() {
    return this->content;
}

void IconSales::replyFinished(QNetworkReply *reply) {
    if (this->content == NULL) {
        this->content = new QTemporaryFile();
    }

    if (!this->content->open()) {
        emit errorDownloadingIcon(this, IconSales::CREATE_TMP_ICON_ERROR);
    } else {
        this->content->write(reply->readAll());
        this->content->seek(0);
        emit iconDownloaded(this);
    }
    reply->deleteLater();
}

}
}
}
