#include "upnp/device/eventmessage.h"

namespace brisa {
namespace upnp {
namespace device {

EventMessage::EventMessage(EventSubscription &subscription,
        const QList<BrisaStateVariable *> *variables, QObject *parent) :
    AbstractEventMessage(parent),
    SEQ(subscription.getNextSeq()),
    VARIABLES(variables),
    subscription(subscription)
{
}

QByteArray EventMessage::getRequestMessage() const
{
    static const QString genericRequest = "NOTIFY %1 HTTP/1.1\r\n"
                                          "HOST: %2:%3\r\n"
                                          "Connection: close\r\n"
                                          "CONTENT-TYPE: text/xml\r\n"
                                          "CONTENT-LENGTH: %4\r\n"
                                          "NT: upnp:event\r\n"
                                          "NTS: upnp:propchange\r\n"
                                          "SID: uuid:%5\r\n"
                                          "SEQ: %6\r\n"
                                          "\r\n"
                                          "%7";

    QString body = "<?xml version=\"1.0\"?>\r\n"
                   "<e:propertyset xmlns:e=\"urn:schemas-upnp-org:event-1-0\">\r\n";

    for (QList<BrisaStateVariable *>::const_iterator i =
            VARIABLES->begin(); i != VARIABLES->end(); ++i) {
        QString variableName = (*i)->getAttribute(BrisaStateVariable::Name);

        body.append("  <e:property>\r\n"
                    "    <" + variableName + ">"
                    + (*i)->getAttribute(BrisaStateVariable::Value)
                    + "</" + variableName + ">\r\n"
                    "  </e:property>\r\n"
                    );
    }

    body.append("</e:propertyset>\r\n");

    QString callback;
    QStringList urlList = subscription.getCallbackUrls();
    if (urlList.size())
        callback = urlList[0];

    QUrl url(callback);

    return genericRequest.arg(url.path(),                   // URI
                              url.host(),                   // HOST HEADER
                              QString::number(url.port()),
                              QString::number(body.size()), // CONTENT-LENGTH HEADER
                              subscription.getSid(),        // SID HEADER
                              QString::number(SEQ),         // SEQ HEADER
                              body                          // REQUEST BODY
                              ).toLatin1();
}

}
}
}
