#include "ssdpudpsocket.h"

namespace brisa {
namespace upnp {

SSDPUdpSocket::SSDPUdpSocket(bool multicast, QUdpSocket *parent) :
    QUdpSocket(parent), multicast(multicast) { }

bool SSDPUdpSocket::isMulticast() {
    return this->multicast;
}

void SSDPUdpSocket::setMulticast(bool multicast) {
    this->multicast = multicast;
}

}
}
