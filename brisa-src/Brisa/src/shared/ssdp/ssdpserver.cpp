#include "ssdpserver.h"

#include <QStringList>
#include <QtDebug>
#include <QDebug>
#ifdef Q_OS_UNIX
#include <arpa/inet.h>
#include <netinet/in.h>
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#endif
#include <string.h>

namespace brisa {
using namespace upnp::controlpoint;
namespace shared {
namespace ssdp {
// TODO: Add this three fields commented below in the ALIVE_MESSAGE
// as per upnp spec 1.1, section 1.2.2.
//  - BOOTID.UPNP.ORG
//  - CONFIGID.UPNP.ORG
//  - SEARCHPORT.UPNP.ORG (optional)
static const QString UPNP_ALIVE_MESSAGE = "NOTIFY * HTTP/1.1\r\n"
                                          "HOST: " + SSDP_ADDR + ":" + SSDP_PORT + "\r\n"
                                          "CACHE-CONTROL: max-age=%1\r\n"
                                          "LOCATION: %2\r\n"
                                          "NT: %3\r\n"
                                          "NTS: ssdp:alive\r\n"
                                          "SERVER: %4\r\n"
                                          "USN: %5\r\n"
                                          "\r\n";

// TODO: Implement ssdp:update as per spec 1.1, section 1.2.4
// and use the below define to build the message, where
// SEARCHPORT.UPNP.ORG are optional.
static const QString UPNP_UPDATE_MESSAGE = "NOTIFY * HTTP/1.1\r\n"
                                           "HOST: " + SSDP_ADDR + ":" + SSDP_PORT + "\r\n"
                                           "LOCATION: %1\r\n"
                                           "NT: %2\r\n"
                                           "NTS: ssdp:update\r\n"
                                           "USN: %3\r\n"
                                           "CONFIGID.UPNP.ORG: %4\r\n"
                                           "NEXTBOOTID.UPNP.ORG: %5\r\n"
                                           "SEARCHPORT.UPNP.ORG: %6\r\n"
                                           "\r\n";

// TODO: Add this two fields commented below in the BYEBYE MESSAGE
// as per upnp spec 1.1, section 1.2.2 and 1.2.3.
//  - BOOTID.UPNP.ORG
//  - CONFIGID.UPNP.ORG
static const QString UPNP_BYEBYE_MESSAGE = "NOTIFY * HTTP/1.1\r\n"
                                           "HOST: " + SSDP_ADDR + ":" + SSDP_PORT + "\r\n"
                                           "NT: %1\r\n"
                                           "NTS: ssdp:byebye\r\n"
                                           "USN: %2\r\n"
                                           "\r\n";

// TODO: Add this three fields commented below in the MSEARCH_RESPONSE
// as per upnp spec 1.1, section 1.3.3.
//  - BOOTID.UPNP.ORG
//  - CONFIGID.UPNP.ORG
//  - SEARCHPORT.UPNP.ORG (optional)
static const QString UPNP_MSEARCH_RESPONSE = "HTTP/1.1 200 OK\r\n"
                                             "CACHE-CONTROL: max-age = %1\r\n"
                                             "DATE: %2\r\n"
                                             "EXT: \r\n"
                                             "LOCATION: %3\r\n"
                                             "SERVER: %4\r\n"
                                             "ST: %5\r\n"
                                             "USN: %6\r\n"
                                             "\r\n";

SSDPServer::SSDPServer(QObject *parent) :
    QObject(parent),
    running(false),
    SSDP_ADDR(SSDP_ADDR),
    SSDP_PORT(SSDP_PORT),
    S_SSDP_PORT(SSDP_PORT)
{
    this->udpListener = new UdpListener(SSDP_ADDR, SSDP_PORT, "Brisa SSDP Server", parent);
    connect(this->udpListener, SIGNAL(readyRead()), this, SLOT(datagramReceived()));
}

SSDPServer::~SSDPServer() {
    if (isRunning())
        stop();

    delete this->udpListener;
}

void SSDPServer::start() {
    if (!isRunning()) {
        this->udpListener->start();
        qDebug() << "BrisaSSDPServer Started!";
        running = true;
    }
    else
    {
        qDebug() << "BrisaSSDPServer already running!";
    }
}

void SSDPServer::stop() {
    if (isRunning()) {
        udpListener->disconnectFromHost();
        running = false;
        qDebug() << "BrisaSSDPServer Stopped!";
    } else {
        qDebug() << "BrisaSSDPServer already stopped!";
    }
}

bool SSDPServer::isRunning() {
    return running;
}

void SSDPServer::doNotify(const QString &usn,
                               const QString &location,
                               const QString &st,
                               const QString &server,
                               const QString &cacheControl)
{
    qDebug() << "SSDP SERVER DONOTIFY()";
    QString message = UPNP_ALIVE_MESSAGE.arg(cacheControl, location, st, server, usn);

    udpListener->writeDatagram(message.toUtf8(),
                               QHostAddress(SSDP_ADDR),
                               SSDP_PORT);

    udpListener->writeDatagram(message.toUtf8(),
                               QHostAddress(SSDP_ADDR),
                               SSDP_PORT);

    qDebug() << "BrisaSSDPServer writing Notify alive for: " << usn << "";
}

void SSDPServer::doByeBye(const QString &usn, const QString &st) {
    QString message = UPNP_BYEBYE_MESSAGE.arg(st, usn);

    udpListener->writeDatagram(message.toUtf8(),
                               QHostAddress(SSDP_ADDR),
                               SSDP_PORT);

    udpListener->writeDatagram(message.toUtf8(),
                               QHostAddress(SSDP_ADDR),
                               SSDP_PORT);

    qDebug() << "BrisaSSDPServer writing notify byebye for: " << usn << "";
}

void SSDPServer::datagramReceived() {
    while (this->udpListener->hasPendingDatagrams()) {
        QByteArray datagram;
        QHostAddress *senderIP = new QHostAddress();
        quint16 senderPort;

        datagram.resize(this->udpListener->pendingDatagramSize());
        this->udpListener->readDatagram(datagram.data(),
                                        datagram.size(),
                                        senderIP,
                                        &senderPort);

        this->msearchReceived(datagram, senderIP, senderPort);

        delete senderIP;
    }
}

void SSDPServer::msearchReceived(const QByteArray datagram,
                                      QHostAddress *senderIp,
                                      quint16 senderPort)
{
    QString message = QString(datagram);
    QStringList messageLines = message.split("\r\n", QString::SkipEmptyParts);
    QMap<QString, QString> response;
    for (int i = 1; i < messageLines.size(); i++) {
        if (messageLines[i].trimmed() != "") {
            QStringList line = messageLines[i].split(":", QString::SkipEmptyParts);
            if (line.size() >= 2) {
                for (int j = 2; j < line.size (); j++) {
                    line[1].append (":");
                    line[1].append (line[j].trimmed ());
                }

                response[line[0].toLower()] = line[1].trimmed();
            } else {
                response[line[0].toLower()] = "";
            }
        } else {
            break;
        }
    }

    if (!response.contains("man")) {
        return;
    }

    if (response["man"] == "\"ssdp:discover\"") {
        qDebug() << "BrisaSSDPServer Received msearch from "
                 << senderIp->toString() << ":" << senderPort
                 << " Search target: " << response["st"];

        emit msearchRequestReceived(response["st"],
                                    senderIp->toString(),
                                    senderPort);
    }
}

void SSDPServer::respondMSearch(const QString &senderIp,
                                     quint16 senderPort,
                                     const QString &cacheControl,
                                     const QString &date,
                                     const QString &location,
                                     const QString &server,
                                     const QString &st,
                                     const QString &usn)
{
    QString message = UPNP_MSEARCH_RESPONSE.arg(cacheControl, date, location, server, st, usn);

    this->udpListener->writeDatagram(message.toUtf8(),
                                     QHostAddress(senderIp),
                                     senderPort);

    qDebug() << "BrisaSSDPServer writing msearch response for " << senderIp << ":" << senderPort;
}

}
}
}
