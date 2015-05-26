#ifndef MEDIARENDERERDEVICE_H
#define MEDIARENDERERDEVICE_H

#include "../device.h"
#include <QObject>

namespace brisa {
namespace upnp {
namespace device {
namespace av {

class MediaRendererDevice : public QObject
{
    Q_OBJECT
public:
    /*!
     * Constructor of device
     */
    explicit MediaRendererDevice(QObject *parent = 0);
    /*!
     * Destructor of device
     */
    ~MediaRendererDevice();
    /*!
     * Start device
     */
    void start();
    /*!
     * Stop device
     */
    void stop();

private:
    Device *device;

signals:

public slots:

};

}  // namespace av
}
}
}

#endif // MEDIARENDERERDEVICE_H
