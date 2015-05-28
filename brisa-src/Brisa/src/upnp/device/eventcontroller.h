#ifndef EVENTCONTROLLER_H
#define EVENTCONTROLLER_H

#include "eventmessage.h"
#include "multicasteventmessage.h"
#include "eventsubscription.h"
#include "../brisautils.h"
#include "../../shared/webserver/webservice.h"
#include "../../shared/webserver/webserver.h"

namespace brisa {
namespace upnp {
namespace device {

class EventController: public brisa::shared::webserver::WebService
{
    Q_OBJECT

public:
    /*!
     * Constructor.
     */
    EventController(brisa::shared::webserver::Webserver *sessionManager,
                    QList<StateVariable *> *stateVariableList,
                    QObject *parent = 0);
    /*!
     * Destructor.
     */
    virtual ~EventController();

public slots:
    /*!
     * When a variable change, send multicast event. Also send unicast message
     * to subscribers to tell him about changes.
     */
    void variableChanged(brisa::upnp::StateVariable *variable);

    /*!
     * Subscribe device on event. This slot is used by \a onRequest.
     */
    void subscribe(const brisa::shared::webserver::http::HttpRequest &request,
                   brisa::shared::webserver::WebserverSession *session);
    /*!
     * Unsubscribe device on event. This slot is used by \a onRequest.
     */
    void unsubscribe(const brisa::shared::webserver::http::HttpRequest &request,
                     brisa::shared::webserver::WebserverSession *session);

protected:
    /*!
     * Handle with a new request \param r in \param session. Call \a subscribe
     * or \a unsubscribe depending of request method.
     */
    void onRequest(const brisa::shared::webserver::http::HttpRequest &,
                   brisa::shared::webserver::WebserverSession *session);

private:
    /*!
     * Destructor.
     */
    EventController(const EventController &); // = delete;
    /*!
     * Connect with host specified in \param url and send \param message to him.
     */
    void sendEvent(const EventMessage &message, const QUrl &url);
    /*!
     * Sem \param message in multicast, to 172.16.6.223:7900.
     */
    void sendMulticastEvent(const MulticastEventMessage &message);
    /*!
     * Return all URLs in \param urls in QStringList.
     */
    QStringList getEventUrls(const QString &urls);
    /*!
     * Cast \param timeout to int and return it.
     */
    int getTimeOut(const QString &timeout);

private:
    QList<brisa::upnp::device::EventSubscription *> subscriptions;
    QList<brisa::upnp::StateVariable *> *variableList;
    QUdpSocket udpSocket;
};
}
}
}
#endif /* _EVENTCONTROLLER_H */
