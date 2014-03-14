#ifndef SSDPUDPSOCKET_H
#define SSDPUDPSOCKET_H

#include <QUdpSocket>

namespace brisa {
namespace ssdp {

class SSDPUdpSocket : public QUdpSocket
{
    Q_OBJECT
public:
    explicit SSDPUdpSocket(bool multicast = false, QUdpSocket *parent = 0);

    bool isMulticast();
    void setMulticast(bool multicast);

private:
    bool multicast;

signals:

public slots:

};

}
}

#endif // SSDPUDPSOCKET_H
