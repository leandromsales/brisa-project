#ifndef BRISANETWORK_H
#define BRISANETWORK_H

#include <QString>
#include <QList>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QNetworkAddressEntry>
#include <QtDebug>
#include <QObject>
#include <ctime>

namespace brisa {
namespace network {

#define LOCAL_HOST "127.0.0.1"

/*!
 * Gets a valid IP address from an active interface on the host computer.
 *
 */
QString getValidIP();

/*!
 *  Gets a valid IP address from an specific interface.
 *  \param networkInterface \a The interface to acquire the IP Address from.
 */
QString getIp(QString networkInterface);

/*!
 *  Gets a closed port to be used by the WebServer.
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
 *  Verify if this address is a Promiscous ipv4 address or not.
 *  \param address \a The address to be tested
 */
bool isPromiscuousIPv4Address(QString address);

/*!
 *  Verify if this address is a Promiscous ipv6 address or not.
 *  \param address \a The address to be tested
 */
bool isPromiscuousIPv6Address(QString address);

/*!
 *  Verify if a port is currently opened on this host.
 *  \param address \a the IP address
 *  \param port    \a the port to be tested
 *  \param timeout \a the timeout to end the test
 */
bool isPortOpen(QString address, qint16 port, qint16 timeout = 2);

}
}

#endif /* _BRISANETWORK_H */

