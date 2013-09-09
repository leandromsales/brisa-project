#ifndef _BRISAEVENTMESSAGE_H
#define _BRISAEVENTMESSAGE_H

#include <QObject>
#include <QList>

#include "brisaabstracteventmessage.h"
#include "brisaeventsubscription.h"

namespace Brisa {

/*!
 * \internal
 * \class Brisa::BrisaEventMessage
 *
 * \brief Represents an UPnP unicast event message.
 */
class BrisaEventMessage: public BrisaAbstractEventMessage {
Q_OBJECT

public:
    /*!
     * Contructs a new event message to the given \a subscription and related to the
     * given \a variables, with the given \a parent object.
     */
    BrisaEventMessage(BrisaEventSubscription &subscription, const QList<
            BrisaStateVariable *> *variables, QObject *parent = 0);

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
    const QList<BrisaStateVariable *> *VARIABLES;

    /*!
     * \property subscription
     *
     * \brief the subscription for which the message will be sent
     */
    BrisaEventSubscription &subscription;
};

}

inline QString Brisa::BrisaEventMessage::getSid() const
{
    return subscription.getSid();
}

#endif /* _BRISAEVENTMESSAGE_H */
