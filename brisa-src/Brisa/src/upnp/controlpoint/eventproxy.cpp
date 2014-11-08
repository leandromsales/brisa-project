/*
 * Universidade Federal de Campina Grande
 * Centro de Engenharia Elétrica e Informática
 * Laboratório de Sistemas Embarcados e Computação Pervasiva
 * BRisa Project / BRisa-Qt - http://brisa.garage.maemo.org
 * Filename: brisaeventproxy.cpp
 * Created:
 * Description: Implementation of BrisaEventProxy class.
 * Authors: Name <email> @since 2009
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
#include "eventproxy.h"

#include <QUrl>
#include <QUuid>
#include <QNetworkAccessManager>


#include "../../shared/webserver/webserversession.h"

namespace brisa {
using namespace shared::webserver;
using namespace shared::webserver::http;
namespace upnp {
namespace controlpoint {

EventProxy::EventProxy(const QStringList &callbackUrls,
                                 Webserver *webserver,
                                 int &deliveryPath,
                                 QString host,
                                 int port,
                                 QNetworkAccessManager *networkAccessManager,
                                 QString eventSub,
                                 QObject *parent) :
    AbstractEventSubscription(QString(), callbackUrls, -1, parent),
    deliveryPath(deliveryPath),
    host(host),
    port(port),
    m_networkAccessManager(networkAccessManager),
    eventSub(eventSub),
    webServer(webserver)
{
    requestId = "";
}

EventProxy::~EventProxy() {
}

void EventProxy::renew(const int &newTimeout) {
    QNetworkRequest renewReq = getRenewRequest(newTimeout);
    if (!renewReq.hasRawHeader("SID")) {
        qWarning() << "Renew failed: SID field not filled.";
        return;
    }
    requestId = generateId();
    renewReq.setRawHeader("ID", requestId.toUtf8());
    m_networkAccessManager->sendCustomRequest(renewReq, "SUBSCRIBE");
}

QString EventProxy::getId() {
    return this->requestId;
}

void EventProxy::subscribe(const int timeout) {
    QNetworkRequest subscribeReq = getSubscriptionRequest(timeout);
    requestId = generateId();
    subscribeReq.setRawHeader("ID", requestId.toUtf8());
    m_networkAccessManager->sendCustomRequest(subscribeReq, "SUBSCRIBE");
}

void EventProxy::unsubscribe(void) {
    QNetworkRequest unsubscribeReq = getUnsubscriptionRequest();
    if (!unsubscribeReq.hasRawHeader("SID")) {
        qWarning() << "Unsubscribe failed: SID field not filled.";
        return;
    }
    m_networkAccessManager->sendCustomRequest(unsubscribeReq, "UNSUBSCRIBE");
}

QNetworkRequest EventProxy::getSubscriptionRequest(const int timeout) {
    QUrl hostUrl;
    hostUrl.setHost(host);
    hostUrl.setPort(port);
    hostUrl.setPath(eventSub);
    QNetworkRequest request(hostUrl);

    // Remote host
    //    request.setRawHeader("HOST", host + ":" + port);

    // Our URL for receiving notifications
    const QUrl url = getUrl();

//    qDebug() << "Url: " << url.host() << " port: " << url.port();
    request.setRawHeader("CALLBACK", QString("<http://"
                                    + url.host()
                                    + ":"
                                    + QString().setNum(url.port())
                                    + "/"
                                    + QString().setNum(deliveryPath)
                                    + ">").toUtf8());

    request.setRawHeader("NT", "upnp:event");
    request.setRawHeader("TIMEOUT", (timeout > 0)
                                    ? QString("Second-" + QString().setNum(timeout)).toUtf8()
                                    : QString("INFINITE").toUtf8()); // NOTE: INFINITE is obsolete in UPnP 1.1
    return request;
}

QNetworkRequest EventProxy::getRenewRequest(const int timeout) const {

    QUrl url;
    url.setHost(host);
    url.setPort(port);
    url.setPath(eventSub);
    QNetworkRequest request(url);

//    request.setRawHeader("HOST", host + ":" + port);
    request.setRawHeader("TIMEOUT", (timeout > 0)
                                    ? QString("Second-" + QString().setNum(timeout)).toUtf8()
                                    : QString("INFINITE").toUtf8());
    if (!getSid().isEmpty())
        request.setRawHeader("SID", getSid().toUtf8());
    return request;
}

QNetworkRequest EventProxy::getUnsubscriptionRequest() const {
    QUrl url;
    url.setHost(host);
    url.setPort(port);
    url.setPath(eventSub);
    QNetworkRequest request(url);
//    request->setValue("HOST", host + ":" + port);
    if (!getSid().isEmpty())
        request.setRawHeader("SID", getSid().toUtf8());
    return request;
}

void EventProxy::onRequest(const HttpRequest &request, WebserverSession *session)
{
    QByteArray sid = request.header("SID");

    if (sid.isEmpty()) {
        // TODO report subscription error to user and should close the connection too
        qDebug() << "SID header not present on event subscription response.";

        return;
    }

    QDomDocument doc;
    QMap<QString, QString> eventingVariables;

    doc.setContent(request.entityBody(), false);
    QDomElement root = doc.documentElement();
    QDomNodeList list = root.elementsByTagName("e:property");

    for (int i = 0; i < list.count(); i++) {
        QDomNodeList internal = list.at(i).childNodes();

        for (int j = 0; j < internal.count(); j++) {
            QDomElement stateVariable = internal.at(j).toElement();
            eventingVariables[stateVariable.tagName()] = stateVariable.text();
        }
    }

    emit eventNotification(this, eventingVariables);

    HttpResponse response(request.httpVersion(), HttpResponse::OK, true);
    response.setHeader("CONNECTION", "close");
    response.setHeader("CONTENT-LENGTH", "0");
    session->respond(response);
}

void EventProxy::setSid(QString &sid) {
    this->SID = sid;
}

QString EventProxy::generateId()
{
    QString id = QUuid::createUuid().toString();
    return id.remove("{").remove("}").remove("-");
}

}
}
}

