#ifndef CONTROLPOINT_MEDIA_GLOBALS_H
#define CONTROLPOINT_MEDIA_GLOBALS_H

#include <QtCore>
#include <QList>
#include <QString>

namespace brisa {
namespace upnp {
namespace controlpoint {
namespace av {

static QList<QString> MEDIA_SERVERS_URN = QList<QString>()
        << "urn:schemas-upnp-org:device:MediaServer:"
        << "urn:schemas-upnp-org:service:ContentDirectory:"
        << "urn:microsoft.com:service:X_MS_MediaReceiverRegistrar:";

static QList<QString> MEDIA_RENDERER_URN = QList<QString>()
        << "urn:schemas-upnp-org:device:MediaRenderer:"
        << "urn:schemas-upnp-org:service:RenderingControl:"
        << "urn:schemas-upnp-org:service:AVTransport:";

}  // namespace av
}
}
}

#endif // CONTROLPOINT_MEDIA_GLOBALS_H
