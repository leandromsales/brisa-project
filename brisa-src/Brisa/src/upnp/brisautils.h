#ifndef BRISAUTILS_H
#define BRISAUTILS_H

#include <QByteArray>
#include <QMap>
#include <QString>
#include <QStringList>
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

/*!
 * Return uuid.
 */
inline QString getCompleteUuid() {
	return QString("uuid:") + getUuid();
}

/*!
 * Parse \param message to QMap. All keys are lowercase.
 */
inline QMap<QString, QString> mapFromMessage(const QString &message) {
    QStringList messageLines = message.split("\r\n");
    QMap<QString, QString> response;

    foreach(QString line, messageLines) {
        if((line = line.trimmed())!= "") {
            int divpos = line.indexOf(':');
            QString key = line.left(divpos).toLower();
            QString value = line.right(line.size() - divpos).remove(0, 1).trimmed();
            response[key] = value;
        } else {
            break;
        }
    }

    return response;
}
}
}

#endif
