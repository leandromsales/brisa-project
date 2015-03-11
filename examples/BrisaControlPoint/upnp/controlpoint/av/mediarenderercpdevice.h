#ifndef CP_MEDIARENDERERDEVICE_H
#define CP_MEDIARENDERERDEVICE_H

#include <QObject>

#include "controlpoint_media_globals.h"
#include "../devicesales.h"

namespace brisa {
namespace upnp {
namespace controlpoint {
namespace av {

class MediaRendererDevice : public DeviceSales
{
    Q_OBJECT
public:
    explicit MediaRendererDevice(QObject *parent = 0);
    explicit MediaRendererDevice(const QString & = QString(""),
                                 const QString & = QString(""),
                                 QObject *parent = 0);

signals:

public slots:

};

}  // namespace av
}
}
}

#endif // CP_MEDIARENDERERDEVICE_H
