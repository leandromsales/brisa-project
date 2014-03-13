#include "mediaservercontrolpointdevice.h"

namespace brisa {
namespace upnp {
namespace controlpoint {

MediaServerControlPointDevice::MediaServerControlPointDevice(QObject *parent) : Device(parent) { }

MediaServerControlPointDevice::MediaServerControlPointDevice(const QString &uuid, const QString &location, QObject *parent) :
    Device(uuid, location, parent) { }

}
}
}
