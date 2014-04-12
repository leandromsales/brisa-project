#include "mediarenderercontrolpointdevice.h"

namespace brisa {
namespace upnp {
namespace controlpoint {
namespace av {

MediaRendererControlPointDevice::MediaRendererControlPointDevice(QObject *parent) : Device(parent) { }

MediaRendererControlPointDevice::MediaRendererControlPointDevice(const QString &uuid, const QString &location, QObject *parent) :
    Device(uuid, location, parent) { }

}  // namespace av
}
}
}
