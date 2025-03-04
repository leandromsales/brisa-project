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
    connect(this, SIGNAL(newConnection(qintptr)), this, SLOT(onNewConnection(qintptr)));
}

void HttpSessionManager::addSession(qintptr socketDescriptor)
{
    qDebug() << "ADD SESSION";
    emit newConnection(socketDescriptor);
}

void HttpSessionManager::onNewConnection(qintptr socketDescriptor)
{
    qDebug() << "ENTROU!!!";
    bool created = false;

    mutex.lock();
    if (pool.size()) {
        pool.back()->setSession(socketDescriptor);
        pool.pop_back();
        created = true;
    }
    mutex.unlock();

    if (!created) {
        HttpSession *s = server->factory().generateSessionHandler(this);
        if (!s) qDebug() << "ERRO NA CRIAÇÃO";
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
}  // namespace Brisa
