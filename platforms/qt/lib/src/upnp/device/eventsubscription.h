#ifndef EVENTSUBSCRIPTION_H
#define EVENTSUBSCRIPTION_H

#include "../abstracteventsubscription.h"
#include "../../shared/webserver/http/httpresponse.h"

#include <QDateTime>
#include <QList>
#include <QString>
#include <QObject>

namespace brisa {
namespace upnp {
namespace device {

class EventSubscription: public AbstractEventSubscription {
Q_OBJECT

public:
    /*!
     * Constructor. Timeout smaller than zero means infinite.
     */
	EventSubscription(const QString &sid, const QStringList &callbackUrls,
            const int &timeout = -1,
			QObject *parent = 0);
    /*!
     * Renew date as current date and timeout as \param newTimeout.
     * Timeout smaller than zero means infinite.
     */
    void renew(const int &newTimeout = -1);
    /*!
     * Return a HTTP Response with code 200 (OK). Also set header of message
     * with date, server, SID, UUID and timeout.
     */
	brisa::shared::webserver::http::HttpResponse getAcceptSubscriptionResponse() const;
    /*!
     * Return a HTTP Response with code 200 (OK).
     */
	brisa::shared::webserver::http::HttpResponse getAcceptUnsubscriptionResponse() const;
};

}
}
}

#endif /* _EVENTSUBSCRIPTION_H */
