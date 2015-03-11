#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <QtCore>
#include <QtNetwork>

#include "http/httpserver.h"

namespace brisa {
namespace shared {
namespace webserver {

/*!
 *  \brief The Webserver class is a web server implementation.
 *
 *  WebServer implements a Web Server.
 */
    class WebService;
    class WebserverSession;

    class Webserver: public brisa::shared::webserver::http::HttpServer
    {
    Q_OBJECT
    public:
        Webserver(const QHostAddress &host, quint16 port);
        ~Webserver();

        void addService(QByteArray path, WebService *service);
        void removeService(QByteArray path);
        WebService *service(QByteArray path) const;

    protected:
        brisa::shared::webserver::http::HttpServerFactory &factory();

    private:
        class Factory: public brisa::shared::webserver::http::HttpServerFactory
        {
        public:
            Factory(Webserver *server) : server(server) {
                qDebug() << "INSTANCIOU FACTORY";
            }
            brisa::shared::webserver::http::HttpSession *generateSessionHandler(brisa::shared::webserver::http::HttpSessionManager *parent);

        private:
            Webserver *server;
        } m_factory;

        // QHash and QList are reentrant, not thread-safe
        mutable QMutex mutex;
        QHash<QByteArray, WebService *> services;
    };

}  // namespace webserver
}  // namespace shared
}  // namespace brisa

#endif /* _WEBSERVER_H */
