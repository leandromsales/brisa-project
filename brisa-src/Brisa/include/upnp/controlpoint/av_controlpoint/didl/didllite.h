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
    Container *parse(const QString &);

signals:

public slots:

};

}  // namespace didl
}  // namespace av
}
}
}

#endif // DIDLLITE_H
