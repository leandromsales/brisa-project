#ifndef EVENTMESSAGE_H
#define EVENTMESSAGE_H

#include <QObject>
#include <QList>

#include "upnp/device/abstracteventmessage.h"
#include "upnp/device/eventsubscription.h"

namespace brisa {
namespace upnp {
namespace device {


/*!
 * \internal
 * \class Brisa::BrisaEventMessage
 *
 * \brief Represents an UPnP unicast event message.
 */
class EventMessage: public AbstractEventMessage {
Q_OBJECT

public:
    /*!
     * Contructs a new event message to the given \a subscription and related to the
     * given \a variables, with the given \a parent object.
     */
    EventMessage(EventSubscription &subscription, const QList<
            StateVariable *> *variables, QObject *parent = 0);

    QByteArray getRequestMessage() const;

    QString getSid() const;

private:

    /*!
     * \property SEQ
     *
     * \brief its event key
     */
    const int SEQ;

    /*!
     * \property VARIABLES
     *
     * \brief the list of state variables related to the event
     */
    const QList<StateVariable *> *VARIABLES;

    /*!
     * \property subscription
     *
     * \brief the subscription for which the message will be sent
     */
    EventSubscription &subscription;
};

inline QString EventMessage::getSid() const
{
    return subscription.getSid();
}

}
}
}
#endif /* _EVENTMESSAGE_H */
