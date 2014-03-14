#include "networkmanager.h"

namespace brisa {

using namespace ssdp;

namespace network {

NetworkManager::NetworkManager(QObject *parent) : QObject(parent) {

}

NetworkManager::~NetworkManager() { }

QList<SSDPUdpSocket*> NetworkManager::enableMulticastAny(const QString &ipMulticast, quint16 port, const QNetworkInterface &interface) {

	QList<SSDPUdpSocket*> sockets;
    SSDPUdpSocket *socket_UNICAST = new SSDPUdpSocket(false);
    SSDPUdpSocket *socket_MULTICAST = new SSDPUdpSocket(true);
    quint16 start = 49152, end = 65535;
    quint16 bind_port_UNICAST = (rand() % (end-start)) + start;

    //Marden// mudei AnyIPv4 para Any
    if (!socket_UNICAST->bind(QHostAddress::Any, bind_port_UNICAST)) {
        qDebug() << "cannot bind unicast socket in ANY";
        // try bind each ip individualy
    }

    socket_MULTICAST->setMulticastInterface(interface);
    if (socket_MULTICAST->joinMulticastGroup(QHostAddress(ipMulticast), interface)) {
        qDebug() << "Interface " + interface.name() + " multicast enabled";
        sockets.append(socket_UNICAST);
        sockets.append(socket_MULTICAST);
    } else {
        delete socket_UNICAST;
        delete socket_MULTICAST;
    }

    return sockets;
}

QList<SSDPUdpSocket*> NetworkManager::enableMulticastAny(const QString &ipMulticast, quint16 port, QList<QNetworkInterface> interfaces) {
    QList<SSDPUdpSocket*> sockets;
    foreach (QNetworkInterface interface, interfaces) {
        sockets += this->enableMulticastAny(ipMulticast, port, interface);
    }
    return sockets;
}

QList<SSDPUdpSocket*> NetworkManager::enableMulticastAny(const QString &ipMulticast, quint16 port) {
    QList<SSDPUdpSocket*> sockets;
    foreach (QNetworkInterface interface, this->getUpIPv4Interfaces()) {
        sockets += this->enableMulticastAny(ipMulticast, port, interface);
    }
    return sockets;
}

QList<SSDPUdpSocket*> NetworkManager::enableMulticast(const QString &ipMulticast, quint16 port, const QNetworkInterface &interface) {
    QList<SSDPUdpSocket*> sockets;
    foreach (QNetworkAddressEntry entry, interface.addressEntries()) {
        QHostAddress ifIp = entry.ip();
        if (ifIp.protocol() == QAbstractSocket::IPv4Protocol) {
            SSDPUdpSocket *socket = new SSDPUdpSocket();
                if (!socket->bind(ifIp, port,
                        SSDPUdpSocket::ShareAddress |
                    SSDPUdpSocket::ReuseAddressHint)) {
                    for (quint16 i = 49152; i < 65535; ++i) {
                        if (socket->bind(QHostAddress(ifIp), i)) {
                            break;
                        }
                    }
                }
            socket->setMulticastInterface(interface);
            if (socket->joinMulticastGroup(QHostAddress(ipMulticast), interface)) {
                sockets.append(socket);
            }
        }
    }
    return sockets;
}

QList<SSDPUdpSocket*> NetworkManager::enableMulticast(const QString &ipMulticast, quint16 port, QList<QNetworkInterface> userInterfaces) {
    QList<SSDPUdpSocket*> sockets;
    foreach (QNetworkInterface interface, userInterfaces) {
        sockets += this->enableMulticast(ipMulticast, port, interface);
    }
    return sockets;
}

QList<SSDPUdpSocket*> NetworkManager::enableMulticast(const QString &ipMulticast, quint16 port) {
    QList<SSDPUdpSocket*> sockets;
        foreach (QNetworkInterface interface, this->getUpIPv4Interfaces()) {
	sockets += this->enableMulticast(ipMulticast, port, interface);
    }
    return sockets;
}

QList<QNetworkInterface> NetworkManager::getUpIPv4Interfaces() {
    QList<QNetworkInterface> multicastIfs;
    foreach (QNetworkInterface interface, QNetworkInterface::allInterfaces()) {
        if (interface.flags().testFlag(QNetworkInterface::IsUp)) {
            multicastIfs << interface;
        }
    }
    return multicastIfs;
}

QList<QHostAddress> NetworkManager::getAddresses(QNetworkInterface *interface) {
    QList<QHostAddress> addresses;
    foreach (QNetworkAddressEntry entry, interface->addressEntries()) {
        if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
            addresses << entry.ip();
        }
    }
    return addresses;
}

}
}

