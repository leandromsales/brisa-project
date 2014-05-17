#include "upnp/icon.h"

namespace brisa {
namespace upnp {

Icon::Icon(QString mimetype, QString width, QString height,
                     QString depth, QString url,
                     QObject *parent) : QObject(parent)
{
    this->mimetype = mimetype;
    this->width = width;
    this->height = height;
    this-> depth = depth;
    this->url = url;
}

void Icon::setAttribute(xmlIconTags key, QString v)
{
    switch (key) {
    case Mimetype:
        this->mimetype = v;
        break;
    case Width:
        this->width = v;
        break;
    case Height:
        this->height = v;
        break;
    case Depth:
        this->depth = v;
        break;
    case Url:
        this->url = v;
        break;
    }
}

void Icon::setIcon(QIcon icon)
{
    this->icon = icon;
}

QIcon Icon::getIcon()
{
    return this->icon;
}

void Icon::downloadIcon(QString deviceUrl)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));

    QString iconUrl = deviceUrl + this->url;
    manager->get(QNetworkRequest(QUrl(iconUrl)));
}

void Icon::downloadFinished(QNetworkReply *reply)
{
    QByteArray imageData = reply->readAll();
    QPixmap pixmap;
    pixmap.loadFromData(imageData);
    this->setIcon(QIcon(pixmap));

    emit iconDownloadFinished();
}

void Icon::clear()
{
    this->mimetype.clear();
    this->width.clear();
    this->height.clear();
    this->depth.clear();
    this->url.clear();
    this->attribute.clear();
}

QString Icon::getAttribute(xmlIconTags key)
{
    switch (key) {
    case Mimetype:
        return this->mimetype;
        break;
    case Width:
        return this->width;
        break;
    case Height:
        return this->height;
        break;
    case Depth:
        return this->depth;
        break;
    case Url:
        return this->url;
        break;
    default:
        return "";
        break;
    }
}

}
}
