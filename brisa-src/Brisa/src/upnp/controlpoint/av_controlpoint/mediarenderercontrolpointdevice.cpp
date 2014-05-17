#include "mediarenderercontrolpointdevice.h"

namespace brisa {
namespace upnp {
namespace controlpoint {
namespace av {

MediaRendererControlPointDevice::MediaRendererControlPointDevice(QObject *parent) : DeviceSales(parent) { }

MediaRendererControlPointDevice::MediaRendererControlPointDevice(const QString &uuid, const QString &location, QObject *parent) :
    DeviceSales(uuid, location, parent) { }

}  // namespace av
}
}
}
