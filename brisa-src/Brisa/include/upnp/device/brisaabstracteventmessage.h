#ifndef BRISAABSTRACTEVENTMESSAGE_H
#define BRISAABSTRACTEVENTMESSAGE_H

#include <QObject>
#include <QList>

#include "upnp/brisastatevariable.h"

namespace brisa {

/*!
 * \internal
 * \class Brisa::BrisaEventMessage
 *
 * \brief Represents an UPnP event message.
 */
class BrisaAbstractEventMessage: public QObject {
Q_OBJECT

public:

    BrisaAbstractEventMessage(QObject *parent = 0);

    QByteArray getRequestMessage() const;
};

}

#endif // BRISAABSTRACTEVENTMESSAGE_H
