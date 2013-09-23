#ifndef BRISADEVICEXMLHANDLER_H
#define BRISADEVICEXMLHANDLER_H

#include <QXmlStreamWriter>
#include <QString>
#include <QFile>
#include <QtDebug>

namespace Brisa {

class BrisaDevice;

class BrisaDeviceXMLHandler {
public:
    void xmlGenerator(BrisaDevice *device, QFile *file);

private:
    void writeDevice(BrisaDevice *device);

    QXmlStreamWriter *writer;
};

}

#endif /* _BRISADEVICEXMLHANDLER_H */

