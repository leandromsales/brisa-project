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
    connect(this, SIGNAL(newConnection(qintptr)), this, SLOT(onNewConnection(qintptr)));
}

void HttpSessionManager::addSession(qintptr socketDescriptor)
{
    emit newConnection(socketDescriptor);
}

void HttpSessionManager::onNewConnection(qintptr socketDescriptor)
{
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
        s->setSession(socketDescriptor);
    }
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
