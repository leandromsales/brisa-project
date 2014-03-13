#include "mediarenderercontrolpointdevice.h"

namespace brisa {
namespace upnp {
namespace controlpoint {

MediaRendererControlPointDevice::MediaRendererControlPointDevice(QObject *parent) : Device(parent) { }

MediaRendererControlPointDevice::MediaRendererControlPointDevice(const QString &uuid, const QString &location, QObject *parent) :
    Device(uuid, location, parent) { }

}
}
}
