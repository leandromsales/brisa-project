#ifndef MEDIARENDERERCONTROLPOINTDEVICE_H
#define MEDIARENDERERCONTROLPOINTDEVICE_H

#include <QObject>

#include "controlpoint_media_globals.h"
#include "upnp/controlpoint/devicesales.h"

namespace brisa {
namespace upnp {
namespace controlpoint {
namespace av {

class MediaRendererControlPointDevice : public DeviceSales
{
    Q_OBJECT
public:
    explicit MediaRendererControlPointDevice(QObject *parent = 0);
    explicit MediaRendererControlPointDevice(const QString & = QString(""),
                                 const QString & = QString(""),
                                 QObject *parent = 0);

signals:

public slots:

};

}  // namespace av
}
}
}

#endif // MEDIARENDERERCONTROLPOINTDEVICE_H
