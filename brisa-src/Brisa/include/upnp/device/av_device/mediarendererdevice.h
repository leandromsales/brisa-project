#ifndef MEDIARENDERERDEVICE_H
#define MEDIARENDERERDEVICE_H

#include "upnp/device/brisadevice.h"
#include <QObject>

namespace brisa {
namespace upnp {
namespace device {

class MediaRendererDevice : public QObject
{
    Q_OBJECT
public:
    explicit MediaRendererDevice(QObject *parent = 0);
    ~MediaRendererDevice();
    void start();
    void stop();

private:
    BrisaDevice *device;

signals:

public slots:

};

}
}
}

#endif // MEDIARENDERERDEVICE_H
