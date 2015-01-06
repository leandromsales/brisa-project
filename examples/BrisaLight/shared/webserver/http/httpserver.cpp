#include "httpserver.h"
#include "httpsessionmanager.h"

#define DBG_PREFIX "HttpContext: "

namespace brisa {
namespace shared {
namespace webserver {
namespace http {

HttpServer::HttpServer(const QHostAddress &address, quint16 port, QObject *parent) :
    QTcpServer(parent),
    address(address),
    port(port)
{
    qDebug() << "INSTANCIOU HTTPSERVER";
    threads.append(new HttpSessionManager(this));
}


void HttpServer::setThreadsNumber(int number)
{
    if (number < 1 || isListening())
        return;

    int oldNumber = threadsNumber();

    if (number > oldNumber) {
        int i = number - oldNumber;
        while (i--) {
            threads.append(new HttpSessionManager(this));
        }
    } else if (number < oldNumber) {
        int i = oldNumber - number;
        while (i--) {
            delete threads.takeLast();
        }
    }
}

HttpServer::~HttpServer()
{
    // TODO: review code
    foreach(HttpSessionManager *thread, threads) {
        thread->terminate();
    }
    foreach(HttpSessionManager *thread, threads) {
        thread->wait();
        delete thread;
    }
}

void HttpServer::incomingConnection(int socketDescriptor)
{
    qDebug() << "CHEGOU NOVA CONEXAO";
    threads[(ringIndex++) % threads.size()]->addSession(socketDescriptor);
}

void HttpServer::start()
{
    listen(address, port);
    foreach(HttpSessionManager *thread, threads) {
        thread->start();
    }
    qDebug() << "SERVIDOR INICIOU";
    // this->incomingConnection (2312);
}

}  // namespace http
}  // namespace webserver
}  // namespace shared
} // namespace Brisa
