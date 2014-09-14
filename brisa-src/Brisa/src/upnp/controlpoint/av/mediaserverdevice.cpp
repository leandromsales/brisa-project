#include "mediaserverdevice.h"

namespace brisa {
namespace upnp {
namespace controlpoint {
namespace av {

brisa::upnp::controlpoint::av::MediaServerDevice::MediaServerDevice(QObject *parent) : DeviceSales(parent) { }

brisa::upnp::controlpoint::av::MediaServerDevice::MediaServerDevice(const QString &uuid, const QString &location, QObject *parent) :
    DeviceSales(uuid, location, parent) { }

}  // namespace av
}
}
}
