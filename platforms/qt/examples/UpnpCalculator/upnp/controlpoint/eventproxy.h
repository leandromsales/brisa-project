/*
 * Universidade Federal de Campina Grande
 * Centro de Engenharia Elétrica e Informática
 * Laboratório de Sistemas Embarcados e Computação Pervasiva
 * BRisa Project / BRisa-Qt - http://brisa.garage.maemo.org
 * Filename: brisaeventproxy.h
 * Created:
 * Description: Definition of BrisaEventProxy class.
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
#ifndef EVENTPROXY_H
#define EVENTPROXY_H

#include <QDateTime>
#include <QList>
#include <QNetworkRequest>
#include <QString>
#include <QObject>
#include <QtXml>

#include "../abstracteventsubscription.h"
#include "../brisaglobal.h"
#include "../../shared/webserver/webserver.h"

QT_BEGIN_NAMESPACE

class QNetworkAccessManager;
// class QNetworkReply;

QT_END_NAMESPACE


namespace brisa {
namespace upnp {
namespace controlpoint {

/*!
 *  \class Brisa::BrisaEventProxy brisaeventproxy.h BrisaUpnp/BrisaEventProxy
 *  Class that implements the event part in control point side in Brisa Qt, this class makes the
 *  operations of subscribe, renew subscription and unsubscribe.
 */
class BRISA_UPNP_EXPORT EventProxy: public AbstractEventSubscription
{
Q_OBJECT

public:
    friend class ControlPoint;

    /*!
     *  Destructor
     */
    virtual ~EventProxy();

    /*!
     *  Gets the request id.
     */
    QString getId();

    /*!
     *  Renew the subscribe in a event for the \a newTimeout passed
     *  \param newTimeout new timeout. Less than 0 to infinite
     */
    void renew(const int &newTimeout = -1);

    /*!
     *  Subscribe for the events from a service subscriptions will last the \a timeout passed.
     *  \param timeout timeout
     */
    void subscribe(const int timeout = -1);

    /*!
     *  Unsubscribe the events from a service, using this the user won't receive more event responses.
     */
    void unsubscribe();

    /*!
     *  Returns the path in service to subscribe events.
     */
    QString getEventSub() {
        return eventSub;
    }

signals:
    /*!
     *  \fn BrisaEventProxy::void eventNotification(BrisaEventProxy *subscription,
     *                           QMap<QString, QString> eventingVariables);
     *
     *  \brief Signal that is emitted when an event is received.
     */
    void eventNotification(brisa::upnp::controlpoint::EventProxy *subscription,
            QMap<QString, QString> eventingVariables);


protected:
    void onRequest(const brisa::shared::webserver::http::HttpRequest &request, brisa::shared::webserver::WebserverSession *session);

private:
    /*!
     *  BrisaEventProxy constructor, prepare all the attribute and starts the webServer and the
     *  webService.
     */
    EventProxy(const QStringList &callbackUrls,
    		brisa::shared::webserver::Webserver *webserver,
                    int &deliveryPath,
                    QString host,
                    int port,
                    QNetworkAccessManager *networkAccessManager,
                    QString eventSub,
                    QObject *parent = 0);

    /*!
     *  \property requestId
     *  \brief id that identifies the request.
     */
    // TODO: Verificar porque esse atributo pode ser visto na classe ControlPoint
    QString requestId;

    /*!
     *  \property deliveryPath
     *  \brief path to receive in webserver the event notifications.
     */
    int deliveryPath;

    /*!
     *  \property host
     *  \brief host to subscribe events.
     */
    QString host;

    /*!
     *  \property port
     *  \brief  port of the host to subscribe events.
     */
    int port;

    /*!
     *  \property http
     *  \brief http object to send the event notification(subscribe i.e.)
     */
    QNetworkAccessManager *m_networkAccessManager;

    /*!
     *  \property eventSub
     *  \brief path in service to subscribe events.
     */
    QString eventSub;

    /*!
     *  \property webServer
     *  \brief Webserver to receive event responses.
     */
    brisa::shared::webserver::Webserver *webServer;

    /*!
     *  Creates and returns a Http subscription header with the passed \a timeout
     *  \param timeout timeout
     *  \return Http subscription header with passed timeout
     */
    QNetworkRequest getSubscriptionRequest(const int timeout);

    /*!
     *    Creates and returns an unsubscription Http header.
     *    \param unsubscription Http header
     */
    QNetworkRequest getUnsubscriptionRequest() const;

    /*!
     *  Creates and returns a Http renew subscription header with the passed \a timeout
     *  \param timeout timeout
     *  \return Http renew subscription header with passed timeout
     */
    QNetworkRequest getRenewRequest(const int timeout) const;

    /*!
     *  Sets the subscription sid.
     *  \param sid sid
     */
    void setSid(QString &sid);

    QString generateId();
};

}
}
}
#endif /* _EVENTPROXY_H */
