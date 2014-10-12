#ifndef MULTICASTEVENTRECEIVER_H
#define MULTICASTEVENTRECEIVER_H

#include <QObject>
#include <QDomDocument>

#include "udplistener.h"
#include "../brisaglobal.h"

namespace brisa {
namespace upnp {
namespace controlpoint {

class BRISA_UPNP_EXPORT MulticastEventReceiver : public QObject
{
    Q_OBJECT
public:

    /*!
     * Constructor.
     *
     * \param parent parent
     */
    MulticastEventReceiver(QObject *parent = 0);

    /*!
     * Destructor
     */
    virtual ~MulticastEventReceiver();

    /*!
     * Starts receiving the multicast events.
     */
    void start();

private slots:
    /*!
     * Reads the received multicast message.
     */
    void read();

    /*!
     * Formats the received multicast message and adds the attributes to a map.
     */
    void formatMessage();

signals:
    /*!
     * Signal emitted when a multicast message is received and fully formatted.
     *
     * \param attributes QMap containing the attributes of the message,
     * including the variable name ("variableName") and the new value
     * ("newValue").
     */
    void multicastReceived(QMap<QString, QString> attributes);

private:

    /*!
     * Parses the body of the multicast message.
     *
     * \param body body of the message to be parsed.
     */
    void parseBody(QByteArray body);

    /*!
     * Received multicast event message.
     */
    QByteArray message;

    /*!
     * Attributes of the multicast event.
     */
    QMap<QString, QString> attributes;

    /*!
     * Socket listening to 239.255.255.246:7900.
     */
    UdpListener *udpListener;

};

}
}
}

#endif // MULTICASTEVENTRECEIVER_H
