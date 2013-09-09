#ifndef _BRISAEVENTSUBSCRIPTION_H
#define _BRISAEVENTSUBSCRIPTION_H

#include "../brisaabstracteventsubscription.h"
#include "upnp/shared/webserver/http/httpresponse.h"

#include <QDateTime>
#include <QList>
#include <QString>
#include <QObject>

namespace Brisa {

    class BrisaEventSubscription: public BrisaAbstractEventSubscription {
    Q_OBJECT

    public:
        BrisaEventSubscription(const QString &sid, const QStringList &callbackUrls,
                const int &timeout = -1, // <0 = infinite
                QObject *parent = 0);

        void renew(const int &newTimeout = -1); // <0 = infinite
        HttpResponse getAcceptSubscriptionResponse() const;
        HttpResponse getAcceptUnsubscriptionResponse() const;
    };

}

#endif /* _BRISAEVENTSUBSCRIPTION_H */
