#ifndef BRISAUTILS_H
#define BRISAUTILS_H

#include <QString>
#include <QUuid>

namespace brisa {
namespace upnp {

/*!
 * Generates random uuids.
 */
inline QString getUuid() {
	// QUuid::toString returns a 38 characters string
	return QUuid::createUuid().toString().remove(0, 1).remove(36, 1);
}

inline QString getCompleteUuid() {
	return QString("uuid:") + getUuid();
}

}
}

#endif
