#ifndef MEDIASERVERDEVICE_H
#define MEDIASERVERDEVICE_H

#include <QObject>

#include "controlpoint_media_globals.h"
#include "upnp/controlpoint/devicesales.h"

namespace brisa {
namespace upnp {
namespace controlpoint {
namespace av {

class MediaServerDevice : public DeviceSales
{
    Q_OBJECT
public:
    explicit MediaServerDevice(QObject *parent = 0);
    explicit MediaServerDevice(const QString & = QString(""),
                               const QString & = QString(""),
                               QObject *parent = 0);
signals:

public slots:

};

}  // namespace av
}
}
}
#endif // MEDIASERVERDEVICE_H
