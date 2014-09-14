/*
 * Universidade Federal de Campina Grande
 * Centro de Engenharia Elétrica e Informática
 * Laboratório de Sistemas Embarcados e Computação Pervasiva
 * BRisa Project / BRisa-Qt - http://brisa.garage.maemo.org
 * Filename: brisacontrolpoint.cpp
 * Created:
 * Description: Implementation of BrisaControlPoint class.
 * Authors: Andre Dieb Martins <andre.dieb@gmail.com>      @since 2009
 *          Jeysibel de Sousa Dantas <jeysibel@gmail.com>  @since 06/04/2010
 *
 *
 * Copyright (C) <2009> <Embbeded Systems and Pervasive Computing Laboratory>
 *
 * BRisa-Qt is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <QtCore>
#include <QtDebug>

#include "controlpoint.h"
#include "../../shared/ssdp/ssdpclient.h"
#include "msearchclientcp.h"

#include "webserversession.h"

namespace brisa {

using namespace network;
using namespace shared::ssdp;
using namespace shared::webserver;

namespace upnp {
namespace controlpoint {

ControlPoint::ControlPoint(QObject *parent, QString st, int mx) :
    QObject(parent) {
    this->discoverNetworkAddress();
    this->buildUrlBase();
    this->deliveryPath = 0;
    this->running = false;
    this->webserver = new Webserver(QHostAddress(ipAddress), port);
    this->multicastReceiver = new MulticastEventReceiver(parent);

    /* HTTP protocol implementation for requests */
    this->http = new QHttp();

    /* SSDP client */
    this->ssdpClient = new SSDPClient(this);

    /* MSearch client */
    msearch = new MSearchClientCP(this, st, mx);

    /* XML downloader */
    downloader = new QNetworkAccessManager();
    webserver->start();

    connect(http, SIGNAL(requestFinished(int, bool)), this, SLOT(httpResponse(int, bool)));
    connect(ssdpClient, SIGNAL(removedDeviceEvent(QString)), this, SLOT(deviceRemoved(QString)));
    connect(ssdpClient,
            SIGNAL(newDeviceEvent(QString, QString, QString, QString, QString, QString)),
            this,
            SLOT(deviceFound(QString, QString, QString, QString, QString, QString)));
    connect(msearch,
            SIGNAL(msearchResponseReceived(QString, QString, QString, QString, QString, QString)),
            this,
            SLOT(deviceFound(QString, QString, QString, QString, QString, QString)));
    connect(downloader, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    connect(this->multicastReceiver, SIGNAL(multicastReceived(QMap<QString,QString>)),
            this, SLOT(receiveMulticast(QMap<QString,QString>)));
    this->multicastReceiver->start();
}

ControlPoint::~ControlPoint() {
    if (!isRunning())
        this->stop();

    delete this->downloader;
    delete this->msearch;
    delete this->ssdpClient;
    delete this->webserver;
    delete this->http;
    delete this->multicastReceiver;
}

void ControlPoint::start() {
    if (isRunning()) {
        qDebug() << "Brisa Control Point: already started.";
    } else {
        this->ssdpClient->start();
        this->msearch->start();
        this->running = true;
    }
}

void ControlPoint::stop() {
    if (!isRunning()) {
        qDebug() << "Brisa Control Point: already stopped.";
    } else {
        this->ssdpClient->stop();
        this->msearch->stop();
        this->running = false;
    }
}

bool ControlPoint::isRunning() {
    return this->running;
}

void ControlPoint::discover() {
    this->msearch->discover();
}

void ControlPoint::replyFinished(QNetworkReply *reply) {
    QTemporaryFile *rootXml = new QTemporaryFile();
    if (!rootXml->open()) {
        qWarning() << "Brisa Control Point: Failed to open file for writing root XML.";
    } else {
        rootXml->write(reply->readAll());
        rootXml->seek(0);
        QUrl *urlBase = new QUrl(reply->url());

        Device *device = new Device(rootXml, urlBase);

        /* Fix embedded devices host/port attributes */
        QList<Service*> serviceList = device->getServiceList();
        foreach(Service *s, serviceList) {
                s->setAttribute(Service::Host, urlBase->host());
                s->setAttribute(Service::Port,
                        QString().setNum(urlBase->port()));
        }

        rootXml->remove();
        delete rootXml;
        delete urlBase;
        // deleteLater as per Qt documentation (see Detailed Description section of
        // QNetworkAccessManager class documentation for more details;
        reply->deleteLater();

        emit deviceFound(device);
    }
}

void ControlPoint::deviceFound(QString, QString location, QString,
                                    QString, QString, QString) {
    downloader->get(QNetworkRequest(QUrl(location)));
}

void ControlPoint::deviceRemoved(const QString udn) {
    emit deviceGone(udn);
}

void ControlPoint::buildUrlBase() {
    QString sPort;
    sPort.setNum(this->port);
    this->urlBase = "http://" + ipAddress + ":" + sPort;
}

void ControlPoint::discoverNetworkAddress() {
    this->port = getPort();
    this->ipAddress = getValidIP();
    qDebug() << "Brisa Control Point: Acquired Address " << this->ipAddress
            + ":" + QString::number(this->port);
}

EventProxy *ControlPoint::getSubscriptionProxy(Service *service) {
    deliveryPath++;
    EventProxy *subscription = new EventProxy(
                                            QStringList(this->urlBase),
                                            webserver,
                                            deliveryPath,
                                            service->getAttribute(Service::Host),
                                            service->getAttribute(Service::Port).toInt(),
                                            http,
                                            service->getAttribute(Service::EventSubUrl));

    requests[deliveryPath] = subscription;

    webserver->addService("/", subscription);

    return subscription;
}

void ControlPoint::httpResponse(int i, bool error) {
    qWarning() << "Brisa Control Point: Http response for request " << i;

    // Locate request object
    EventProxy *subscription = NULL;

    foreach(int deliveryPath, requests.keys()) {
        if (requests[deliveryPath]->requestId == i) {
            subscription = requests[deliveryPath];
            qDebug() << "Brisa Control Point: Response for request id " << i << " " << deliveryPath;
            break;
        }
    }

    if (!subscription) {
        qWarning() << "Brisa Control Point: Failed to match response with request id " << i;
        return;
    } else if (error) {
        // TODO forward error to user, notify that subscription didn't work
        qWarning() << "Brisa Control Point: Subscription error "  << http->errorString() << i;
        return;
    }

    QHttpResponseHeader header = http->lastResponse();
    QString sid = header.value("SID");

    if (sid.isEmpty()) {
        // Try "sid"
        sid = header.value("sid");
    }

    if (sid.isEmpty()) {
        // TODO report subscription error to user
        qWarning() << "Brisa Control Point: SID header not present on event subscription response.";
        foreach(QString key, header.keys()) {
            qDebug() << key << header.value(key);
        }

        qDebug() << "Brisa Control Point: Finished printing headers.. printing data" << http->readAll();

        return;
    }

    subscription->setSid(sid);
    qDebug() << "Brisa Control Point: Subscribed with SID " << subscription->getSid();
}

void ControlPoint::receiveMulticast(OutArgument attributes)
{
    emit multicastReceived(attributes.value("variableName"), attributes.value("newValue"));
    emit multicastReceivedRaw(attributes);
}

}
}
}
