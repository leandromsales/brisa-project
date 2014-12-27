#include "httpsessionmanager.h"
#include "httpsession.h"

namespace brisa {
namespace shared {
namespace webserver {
namespace http {

HttpSessionManager::HttpSessionManager(HttpServer *parent) :
    QThread(parent),
    server(parent)
{
    qDebug() << "INICIOU HTTP SESSION MANAGER";
    connect(this, SIGNAL(newConnection(int)), this, SLOT(onNewConnection(int)));
    // larissa
    //this->run ();
}

//void HttpSessionManager::run()
//{
//    qDebug() << "RODOU HTTP SESSION MANAGER";
//    qDebug() << "SAIU";
//    exec();
//}

void HttpSessionManager::addSession(int socketDescriptor)
{
    qDebug() << "ADD SESSION";
    emit newConnection(socketDescriptor);
}

void HttpSessionManager::onNewConnection(int socketDescriptor)
{
    qDebug() << "ENTROU!!!";
    bool created = false;

    mutex.lock();
    qDebug() << "INICIO MUTEX";
    if (pool.size()) {
        pool.back()->setSession(socketDescriptor);
        pool.pop_back();
        created = true;
    }
    qDebug() << "FIM MUTEX";
    mutex.unlock();

    if (!created) {
        qDebug() << "NAO CRIADO";
        HttpSession *s = server->factory().generateSessionHandler(this);
        s->setSession(socketDescriptor);
    }
    qDebug() << "FIM";
}

void HttpSessionManager::releaseSession(HttpSession *session)
{
    mutex.lock();

    pool.append(session);

    mutex.unlock();
}

}  // namespace http
}  // namespace webserver
}  // namespace shared
} // namespace Brisa
