#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTemporaryFile>

#include "icon.h"

Icon::Icon(QObject *parent) : QObject(parent) {
    this->content = NULL;
    this->downloader = new QNetworkAccessManager(this);
    connect(downloader, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

Icon::Icon(const QString &mimeType,
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

Icon::~Icon() {
    if (this->content) {
        this->content->remove();
        delete this->content;
    }
    delete this->downloader;
}

Icon::Icon(const Icon &icon) : QObject(icon.parent()) {
    *this = icon;
}

Icon &Icon::operator=(const Icon &icon) {
    if (this != &icon) {
        this->attributes = icon.getAttributes();
    }
    return *this;
}

void Icon::setBaseUrl(const QUrl &baseUrl) {
    this->baseUrl = baseUrl;
}

QUrl Icon::getBaseUrl() {
    return this->baseUrl;
}

bool Icon::setAttribute(const QString &attributeName, const QString &attributeValue) {
    this->attributes[attributeName.toLower()] = attributeValue;
    return true; // use this semantic for future validation
}

QString Icon::getAttribute(const QString &attributeName) {
    return this->attributes.value(attributeName.toLower());
}

QHash<QString, QString> Icon::getAttributes() const {
    return this->attributes;
}

QUrl Icon::getUrl() {
    QUrl url(this->attributes.value("url"));
    url = this->baseUrl.resolved(url);
    return url;
}

bool Icon::downloadIt() {
    QUrl url = this->getUrl();
    if (url.isValid()) {
        this->downloader->get(QNetworkRequest(url));
        return true;
    }
    return false;
}

QTemporaryFile *Icon::getContent() {
    return this->content;
}

void Icon::replyFinished(QNetworkReply *reply) {
    if (this->content == NULL) {
        this->content = new QTemporaryFile();
    }

    if (!this->content->open()) {
        emit errorDownloadingIcon(this, Icon::CREATE_TMP_ICON_ERROR);
    } else {
        this->content->write(reply->readAll());
        this->content->seek(0);
        emit iconDownloaded(this);
    }
    reply->deleteLater();
}
