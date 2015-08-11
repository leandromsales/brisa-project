#ifndef BRISANETWORK_H
#define BRISANETWORK_H

#include <QString>
#include <QList>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QNetworkConfigurationManager>
#include <QNetworkConfiguration>
#include <QNetworkSession>
#include <QtDebug>
#include <QObject>
#include <QIODevice>
#include <QTcpSocket>
#include <time.h>
#include <ctime>

namespace brisa {
namespace network {

#define LOCAL_HOST "127.0.0.1"

/*!
 * Get a valid IP address from an active interface on the host computer.
 *
 */
QString getValidIP();

/*!
 *  Get a valid IP address from a specific interface.
 *  \param networkInterface \a The interface to acquire the IP Address from
 */
QString getIp(QString networkInterface);

/*!
 *  Generate a port number in range [49152,65535] to be used by webserver.
 */
quint16 getPort();

/*!
 *  Verify if this address is a loopback ipv4 address or not.
 *  \param address \a The address to be tested
 */
bool isLoopbackIPv4Address(QString address);

/*!
 *  Verify if this address is a loopback ipv6 address or not.
 *  \param address \a The address to be tested
 */
bool isLoopbackIPv6Address(QString address);

/*!
 *  Verify if this address is a promiscous ipv4 address or not.
 *  \param address \a The address to be tested
 */
bool isPromiscuousIPv4Address(QString address);

/*!
 *  Verify if this address is a promiscous ipv6 address or not.
 *  \param address \a The address to be tested
 */
bool isPromiscuousIPv6Address(QString address);

/*!
 *  Verify if a port is currently openned on this host.
 *  \param address \a The IP address
 *  \param port    \a The port to be tested
 *  \param timeout \a The timeout (in milliseconds) to end the test
 */
bool isPortOpen(QString address, qint16 port, qint16 timeout = 2);

}
}

#endif /* _BRISANETWORK_H */

