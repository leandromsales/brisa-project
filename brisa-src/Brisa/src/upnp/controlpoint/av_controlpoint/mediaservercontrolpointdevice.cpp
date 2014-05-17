#include "mediaservercontrolpointdevice.h"

namespace brisa {
namespace upnp {
namespace controlpoint {
namespace av {

MediaServerControlPointDevice::MediaServerControlPointDevice(QObject *parent) : DeviceSales(parent) { }

MediaServerControlPointDevice::MediaServerControlPointDevice(const QString &uuid, const QString &location, QObject *parent) :
    DeviceSales(uuid, location, parent) { }

}  // namespace av
}
}
}
