#ifndef BRISAEVENTSUBSCRIPTION_H
#define BRISAEVENTSUBSCRIPTION_H

#include "upnp/brisaabstracteventsubscription.h"
#include "shared/webserver/http/httpresponse.h"

#include <QDateTime>
#include <QList>
#include <QString>
#include <QObject>

namespace brisa {
namespace upnp {
namespace device {

    class BrisaEventSubscription: public BrisaAbstractEventSubscription {
    Q_OBJECT

    public:
        BrisaEventSubscription(const QString &sid, const QStringList &callbackUrls,
                const int &timeout = -1, // <0 = infinite
                QObject *parent = 0);

        void renew(const int &newTimeout = -1); // <0 = infinite
        brisa::shared::webserver::http::HttpResponse getAcceptSubscriptionResponse() const;
        brisa::shared::webserver::http::HttpResponse getAcceptUnsubscriptionResponse() const;
    };

}
}
}

#endif /* _BRISAEVENTSUBSCRIPTION_H */
