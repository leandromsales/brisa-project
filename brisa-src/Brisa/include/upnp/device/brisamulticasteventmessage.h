#ifndef MULTICASTEVENTMESSAGE_H
#define MULTICASTEVENTMESSAGE_H

#include <QObject>
#include <QList>

#include "brisaabstracteventmessage.h"

namespace brisa {
namespace upnp {
namespace device {


static const QString NOTIFY_EVENT_MESSAGE =
        "NOTIFY *\r\n"
        "HOST: %1\r\n"
        "CONTENT-TYPE: text/xml\r\n"
        "CONTENT-TYPE: charset=\"utf-8\"\r\n"
        "USN: %2\r\n"
        "SVCID: %3\r\n"
        "NT: upnp:event\r\n"
        "NTS: upnp:propchange\r\n"
        "SEQ: %4\r\n"
        "LVL: %5\r\n"
        "BOOTID.UPNP.ORG: %6\r\n"
        "CONTENT-LENGTH: %7\r\n\r\n";

class MulticastEventMessage : public AbstractEventMessage
{
public:
    MulticastEventMessage(BrisaStateVariable *variable,
                               QString LVL, QObject *parent = 0);

    QByteArray getMessageHeader() const;

    QByteArray getMessageBody() const;

    QByteArray getRequestMessage() const;

private:
    BrisaStateVariable *variable;

    /*!
     * \property SEQ
     *
     * \brief its event key
     */
    int SEQ;

    /*!
     * \property LVL
     * \brief event level
     */
    QString LVL;
};

}
}
}
#endif // MULTICASTEVENTMESSAGE_H
