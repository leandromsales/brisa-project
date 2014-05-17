#include "upnp/controlpoint/av/mediarenderercpdevice.h"

namespace brisa {
namespace upnp {
namespace controlpoint {
namespace av {

MediaRendererDevice::MediaRendererDevice(QObject *parent) : DeviceSales(parent) { }

MediaRendererDevice::MediaRendererDevice(const QString &uuid, const QString &location, QObject *parent) :
    DeviceSales(uuid, location, parent) { }

}  // namespace av
}
}
}
