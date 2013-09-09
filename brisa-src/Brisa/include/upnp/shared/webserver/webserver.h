#ifndef _WEBSERVER_H
#define _WEBSERVER_H

#include <QtCore>
#include <QtNetwork>

#include "http/httpserver.h"

namespace Brisa {

/*!
 *  \brief The Webserver class is a web server implementation.
 *
 *  WebServer implements a Web Server.
 */
    class WebService;
    class WebserverSession;

    class Webserver: public HttpServer
    {
    Q_OBJECT
    public:
        Webserver(const QHostAddress &host, quint16 port);
        ~Webserver();

        void addService(QByteArray path, WebService *service);
        void removeService(QByteArray path);
        WebService *service(QByteArray path) const;

    protected:
        HttpServerFactory &factory();

    private:
        class Factory: public HttpServerFactory
        {
        public:
            Factory(Webserver *server) : server(server) {}
            HttpSession *generateSessionHandler(HttpSessionManager *parent);

        private:
            Webserver *server;
        } m_factory;

        // QHash and QList are reentrant, not thread-safe
        mutable QMutex mutex;
        QHash<QByteArray, WebService *> services;
    };

}

#endif /* _WEBSERVER_H */
