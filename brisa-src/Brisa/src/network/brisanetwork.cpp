#include <QtDebug>
#include <QIODevice>
#include <QTcpSocket>
#include <time.h>
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

QString getValidIP() {
     /*
     #if defined(Q_OS_UNIX) || defined(Q_OS_ANDROID)
     BrisaConfigurationManager *config = BrisaConfigurationManager::getInstance();
     QString interfaceName = config->getParameter("network", "interface");
     QString ip = getIp(interfaceName);
     if (ip.isEmpty()) {
     ip = config->getParameter("network", "ip");
     }
     if (ip.isEmpty()) {
     ip = QHostAddress(QHostAddress::Any).toString();
     }
     return ip;
     #else
     */

     /* commented by leandro!
     foreach(QHostAddress addressEntry , QNetworkInterface::allAddresses() )
     {
     QString address = addressEntry.toString();
     if (!(isLoopbackIPv4Address(address)) && !(isLoopbackIPv6Address(
     address)) && !(isPromiscuousIPv4Address(address))
     && !(isPromiscuousIPv6Address(address))) {
     return address;
     }
     }
     qDebug()
     << "Couldn't acquire a non loopback IP  address,returning 127.0.0.1.";
     return "127.0.0.1";
     */

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
    return "127.0.0.1";
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
        //stay waiting for some miliseconds to re-verify the state
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
    //Generate a port number in range [49152,65535]
    //TODO modify this expression to a more legible one
    quint16 randomPort = (49152 + rand() / (RAND_MAX / (65535 - 49152 + 1) + 1));
    qDebug() << "Port value chosen:" << randomPort;
    while (isPortOpen(getValidIP(), randomPort, 5)) {
        qDebug() << "Port is already opened, trying another ";
        randomPort = (49152 + rand() / (RAND_MAX / (65535 - 49152 + 1) + 1));
    }
    return randomPort;
}

}
}

