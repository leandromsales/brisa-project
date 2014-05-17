#ifndef ABSTRACTEVENTMESSAGE_H
#define ABSTRACTEVENTMESSAGE_H

#include <QObject>
#include <QList>

#include "upnp/statevariable.h"

namespace brisa {
namespace upnp {
namespace device {

/*!
 * \internal
 * \class Brisa::BrisaEventMessage
 *
 * \brief Represents an UPnP event message.
 */
class AbstractEventMessage: public QObject {
Q_OBJECT

public:

    AbstractEventMessage(QObject *parent = 0);

    QByteArray getRequestMessage() const;
};

}
}
}

#endif // ABSTRACTEVENTMESSAGE_H
