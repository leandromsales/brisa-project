#ifndef BRISAEVENTCONTROLLER_H
#define BRISAEVENTCONTROLLER_H

#include "brisaeventmessage.h"
#include "brisamulticasteventmessage.h"
#include "brisaeventsubscription.h"
#include "upnp/brisautils.h"
#include "upnp/shared/webserver/webservice.h"
#include "upnp/shared/webserver/webserver.h"

namespace Brisa {
    class BrisaEventController: public WebService
    {
    Q_OBJECT

    public:
        BrisaEventController(Webserver *sessionManager,
       			QList<BrisaStateVariable *> *stateVariableList, QObject *parent = 0);
        virtual ~BrisaEventController();

    public slots:
        void variableChanged(BrisaStateVariable *variable);

        void subscribe(const HttpRequest &request, WebserverSession *session);

        void unsubscribe(const HttpRequest &request, WebserverSession *session);

    protected:
        void onRequest(const HttpRequest &, WebserverSession *session);

    private:
        BrisaEventController(const BrisaEventController &); // = delete;

        void sendEvent(const BrisaEventMessage &message, const QUrl &url);

        void sendMulticastEvent(const BrisaMulticastEventMessage &message);

        QStringList getEventUrls(const QString &urls);

        int getTimeOut(const QString &timeout);

        QList<BrisaEventSubscription *> subscriptions;

        QList<BrisaStateVariable *> *variableList;

        QUdpSocket udpSocket;
    };
}

#endif /* _BRISAEVENTCONTROLLER_H */
