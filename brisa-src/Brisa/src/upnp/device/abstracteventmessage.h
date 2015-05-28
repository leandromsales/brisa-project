#ifndef ABSTRACTEVENTMESSAGE_H
#define ABSTRACTEVENTMESSAGE_H

#include <QObject>
#include <QList>

#include "../statevariable.h"

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
    /*!
     * Constructor.
     */
    AbstractEventMessage(QObject *parent = 0);
    /*!
     * Return a QByteArray contains a UPnP event message.
     */
    QByteArray getRequestMessage() const;
};

}
}
}

#endif // ABSTRACTEVENTMESSAGE_H
