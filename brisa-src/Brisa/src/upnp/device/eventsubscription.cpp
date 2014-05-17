#include "upnp/device/eventsubscription.h"

namespace brisa {
using namespace shared::webserver::http;
namespace upnp {
namespace device {

EventSubscription::EventSubscription(const QString &sid,
        const QStringList &callbackUrls, const int &timeout, QObject *parent) :
            AbstractEventSubscription(sid, callbackUrls, timeout, parent) {
}

void EventSubscription::renew(const int &newTimeout) {
    this->date = QDateTime::currentDateTime();
    this->timeout = newTimeout;
}

HttpResponse EventSubscription::getAcceptSubscriptionResponse() const {
    HttpResponse response(HttpVersion(1, 1), HttpResponse::OK);

    response.setHeader("DATE", QDateTime::currentDateTime().toUTC().toString(
            "ddd, dd MMM yyyy HH:mm:ss").toLatin1() + " GMT");

    //FIXME: use system information
    response.setHeader("SERVER", QByteArray("OS/version") + " UPnP/1.0"
                       + QByteArray("product/version"));

    response.setHeader("SID", QByteArray("uuid:") + SID.toUtf8());

    response.setHeader("TIMEOUT", (timeout >= 0) ? QByteArray("Second-")
                       + QByteArray::number(timeout) : "infinite");

    return response;
}

HttpResponse EventSubscription::getAcceptUnsubscriptionResponse() const {
    return HttpResponse(HttpVersion(1, 1), HttpResponse::OK);
}

}
}
}
