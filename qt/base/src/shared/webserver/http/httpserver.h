#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QTcpServer>
#include <QFile>
#include <QHostAddress>

namespace brisa {
namespace shared {
namespace webserver {
namespace http {

class HttpSession;
class HttpSessionManager;

class HttpServerFactory
{
public:
    virtual HttpSession *generateSessionHandler(HttpSessionManager *parent) = 0;
};

class HttpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit HttpServer(const QHostAddress &address = QHostAddress::Any,
                        quint16 port = 0, QObject *parent = 0);
    ~HttpServer();

    /*!
     * Start a HTTP Server.
     */
    void start();

    /*!
     * Return number of threads in this HTTP Server.
     */
    int threadsNumber() const;
    /*!
     * Set number of threads in this HTTP Server. This value must be greater
     * than one.
     */
    void setThreadsNumber(int);

    virtual HttpServerFactory &factory() = 0;

protected:
    /*!
     * This virtual function is called when a new connection is available.
     * The \param handle argument is a socket descriptor for the accepted connection.
     *
     */
    void incomingConnection(qintptr handle);

private:
    QHostAddress address;
    quint16 port;
    QList<HttpSessionManager *> threads;
    int ringIndex;

};

}  // namespace http
}  // namespace webserver
}  // namespace shared
} // namespace Brisa

inline int brisa::shared::webserver::http::HttpServer::threadsNumber() const
{
    return threads.size();
}

#endif // HTTPSERVER_H
