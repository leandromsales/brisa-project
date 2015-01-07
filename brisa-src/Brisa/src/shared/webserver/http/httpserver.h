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

    void start();

    int threadsNumber() const;
    // must be more than 0
    void setThreadsNumber(int);

    virtual HttpServerFactory &factory() = 0;

// Marden: Mudança de "int *" para int
// Larissa: mudança de int para qintptr
protected:
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
