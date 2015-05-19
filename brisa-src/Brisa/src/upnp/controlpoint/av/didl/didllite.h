#ifndef DIDLLITE_H
#define DIDLLITE_H

#include <QObject>

#include "container.h"

namespace brisa {
namespace upnp {
namespace controlpoint {
namespace av {
namespace didl {

class DIDLLite : public QObject
{
    Q_OBJECT
public:
    explicit DIDLLite(QObject *parent = 0);
    /*!
     * Parse \param content to DOM Document and write it as a container.
     * After all, return this container.
     */
    Container *parse(const QString &content);

signals:

public slots:

};

}  // namespace didl
}  // namespace av
}
}
}

#endif // DIDLLITE_H
