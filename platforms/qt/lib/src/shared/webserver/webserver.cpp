#include "webserver.h"
#include "webstaticcontent.h"
#include "webserversession.h"
#include "webfile.h"
#include "webserversession.h"

#define DEFAULT_PAGE "<html><body><h1>BRisa WebServer!</h1></body></html>\n"

namespace brisa {
namespace shared {
namespace webserver {

using namespace http;

Webserver::Webserver(const QHostAddress &host, quint16 port) :
        HttpServer(host, port),
        m_factory(this)
{
    addService("/", new WebStaticContent(DEFAULT_PAGE, this));
}

Webserver::~Webserver()
{
}

void Webserver::addService(QByteArray path, WebService *service)
{
    if (!service || path.isEmpty()) {
        return;
    }

    mutex.lock();

    if (!path.startsWith('/'))
        path.prepend('/');

    services[path] = service;
    service->m_path = path;

    qDebug() << "Adding Service: " << path;

    mutex.unlock();
}

void Webserver::removeService(QByteArray path)
{
    if (!path.startsWith('/'))
        path.prepend('/');

    mutex.lock();

    if (services.contains(path)) {
        services[path]->m_path.clear();
        services.remove(path);
    } else {
        qDebug() << "There is no service with this path";
    }

    mutex.unlock();
}

WebService *Webserver::service(QByteArray path) const
{
    if (path.isEmpty()) {
        qDebug() << "There is no path";
        return NULL;
    }

    if (!path.startsWith('/'))
        path.prepend('/');

    mutex.lock();
    WebService *service = services.value(path);
    if (!service) {
        qDebug() << "There is no service with this path";
        return NULL;
    }
    mutex.unlock();
    return service;
}

HttpServerFactory &Webserver::factory()
{
    return m_factory;
}

HttpSession *Webserver::Factory::generateSessionHandler(HttpSessionManager *parent)
{
    return new WebserverSession(server, parent);
}

}
}
}
