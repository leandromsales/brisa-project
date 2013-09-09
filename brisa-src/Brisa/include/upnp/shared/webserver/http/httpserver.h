#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QTcpServer>
#include <QFile>
#include <QHostAddress>

namespace Brisa {

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
//Marden// Mudança de "int *" para int
protected:
    void incomingConnection(int  handle);

private:
    QHostAddress address;
    quint16 port;
    QList<HttpSessionManager *> threads;
    int ringIndex;

};

} // namespace Brisa

inline int Brisa::HttpServer::threadsNumber() const
{
    return threads.size();
}

#endif // HTTPSERVER_H
