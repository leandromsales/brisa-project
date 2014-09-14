#ifndef SSDPUDPSOCKET_H
#define SSDPUDPSOCKET_H

#include <QUdpSocket>

namespace brisa {
namespace shared {
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
}  // namespace shared
}

#endif // SSDPUDPSOCKET_H
