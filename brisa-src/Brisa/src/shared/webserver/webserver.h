#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <QtCore>
#include <QtNetwork>

#include "http/httpserver.h"

namespace brisa {
namespace shared {
namespace webserver {

class WebService;
class WebserverSession;

class Webserver: public brisa::shared::webserver::http::HttpServer
{
    Q_OBJECT
public:
    Webserver(const QHostAddress &host, quint16 port);
    ~Webserver();

    /*!
     * Add a new webservice in \param path
     * \param service is an instance of webservice
     */
    void addService(QByteArray path, WebService *service);

    /*!
     * Remove a webservice in \param path
     */
    void removeService(QByteArray path);

    /*!
     * Get a webservice in \param path
     */
    WebService *service(QByteArray path) const;

protected:
    brisa::shared::webserver::http::HttpServerFactory &factory();

private:
    class Factory: public brisa::shared::webserver::http::HttpServerFactory
    {
    public:
        Factory(Webserver *server) : server(server) {}
        brisa::shared::webserver::http::HttpSession *generateSessionHandler(brisa::shared::webserver::http::HttpSessionManager *parent);

    private:
        Webserver *server;
    } m_factory;

    // QHash and QList are reentrant, not thread-safe
    mutable QMutex mutex;
    QHash<QByteArray, WebService *> services;
};

}
}
}

#endif /* _WEBSERVER_H */
