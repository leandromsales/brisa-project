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
    /*!
     * Open a \param file and write description of \param device as XML.
     */
    void xmlGenerator(Device *device, QFile *file);

private:
    /*!
     * A private method used by \a xmlGenerator to get descripton of \param device.
     */
    void writeDevice(Device *device);

    QXmlStreamWriter *writer;
};

}
}
}

#endif /* _DEVICEXMLHANDLER_H */

