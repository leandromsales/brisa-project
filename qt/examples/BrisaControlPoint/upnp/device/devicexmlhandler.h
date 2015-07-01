#ifndef DEVICEXMLHANDLER_H
#define DEVICEXMLHANDLER_H

#include <QXmlStreamWriter>
#include <QString>
#include <QFile>
#include <QtDebug>

namespace brisa {
namespace upnp {
namespace device {

class Device;

class DeviceXMLHandler {
public:
    void xmlGenerator(Device *device, QFile *file);

private:
    void writeDevice(Device *device);

    QXmlStreamWriter *writer;
};

}
}
}

#endif /* _DEVICEXMLHANDLER_H */

