#include "brisanetwork.h"

namespace brisa {
namespace network {

bool isLoopbackIPv4Address(QString address) {
    return !address.compare("127.0.0.1");
}

bool isLoopbackIPv6Address(QString address) {
    return !address.compare("0:0:0:0:0:0:0:1");
}

bool isPromiscuousIPv4Address(QString address) {
    return !address.compare("0.0.0.0");
}

bool isPromiscuousIPv6Address(QString address) {
    return !address.compare("0:0:0:0:0:0:0:0") || !address.compare("::");
}

// TO DO implement if defined to others plataforms
QString getValidIP() {
#if defined(Q_OS_UNIX) || defined(Q_OS_ANDROID)
    QNetworkConfigurationManager mgr;
    QNetworkConfiguration nconfig = mgr.defaultConfiguration();
    QNetworkSession session ( nconfig );
    QNetworkInterface ninter = session.interface();

    int protocol;
    /*
     * Qt specification
     * 0 is QAbstractSocket::IPv4Protocol
     * 1 is QAbstractSocket::IPv6Protocol
     * 2 is QAbstractSocket::AnyIPProtocol (either IPv4 or IPv6)
     * -1 is QAbstractSocket::UnknownNetworkLayerProtocol (other than IPv4 and IPv6)
     */
    QList<QNetworkAddressEntry> laddr = ninter.addressEntries();
    for (QList<QNetworkAddressEntry>::const_iterator it = laddr.begin(); it != laddr.end(); ++it)
    {
        protocol = it->ip ().protocol ();
        QString ip = it->ip ().toString ();
        if ((!isLoopbackIPv4Address (ip) && !isPromiscuousIPv4Address (ip))
                || (!isLoopbackIPv6Address (ip) && !isPromiscuousIPv6Address (ip))) {
            return it->ip().toString ();
        }
    }
    if (protocol == 0 || protocol == 2) {
        qDebug() << "Couldn't acquire a non loopback IPv4 address, returning 127.0.0.1.";
        return "127.0.0.1";
    } else if (protocol == 1) {
        qDebug() << "Couldn't acquire a non loopback IPv6 address, returning 0:0:0:0:0:0:0:1.";
        return "0:0:0:0:0:0:0:1";
    } else {
        // TODO implement something to deal with this case
        qDebug() << "Couldn't acquire a non loopback IPv4/IPv6 address";
        return "";
    }
#endif

#if defined (__QNXNTO__)
    /* By Rodrigo
     * This code works properly for BlackBerry 10
     */
    foreach (const QNetworkInterface &interface, QNetworkInterface::allInterfaces())
    {
        foreach (const QHostAddress &address, interface.allAddresses())
        {
            if (address.protocol() == QAbstractSocket::IPv4Protocol
                    && address != QHostAddress(QHostAddress::LocalHost))
                return address.toString();
        }
    }
    return "127.0.0.1";
#endif
    return "192.168.0.105";
}

//TODO deprecated function
QString getIp(QString networkInterface) {
    QNetworkInterface interface = QNetworkInterface::interfaceFromName(
            networkInterface);
    if (interface.isValid()) {
        return interface.addressEntries().first().ip().toString();
    }
    return "";
}

bool isPortOpen(QString address, qint16 port, qint16 timeout) {
    bool result = false;
    QTcpSocket *socket = new QTcpSocket();
    socket->connectToHost(address, port);
    socket->waitForConnected(timeout);
    switch (socket->state()) {
    case QAbstractSocket::UnconnectedState:
        delete socket;
        return false;

    case QAbstractSocket::ConnectingState:
        socket->waitForConnected(timeout);

        result = (socket->state() == QAbstractSocket::ConnectedState);
        delete socket;
        return result;

    case QAbstractSocket::ConnectedState:
        delete socket;
        return true;
    default:
        delete socket;
    }
    return false;
}

quint16 getPort() {
    srand(time(NULL));
    //TODO modify this expression to a more legible one
    // An option could be: v3 = rand() % 30 + 1985;
    // With this, v3 is a number in the range 1985-2014
    quint16 randomPort = (49152 + rand() / (RAND_MAX / (65535 - 49152 + 1) + 1));
    while (isPortOpen(getValidIP(), randomPort, 5)) {
        randomPort = (49152 + rand() / (RAND_MAX / (65535 - 49152 + 1) + 1));
    }
    return randomPort;
}

}
}

