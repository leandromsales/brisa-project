#ifndef WEBSERVERSESSION_H
#define WEBSERVERSESSION_H

#include "http/httpsession.h"

class QTimer;

namespace brisa {
namespace shared {
namespace webserver {

class Webserver;

class WebserverSession : public brisa::shared::webserver::http::HttpSession
{
Q_OBJECT
public:
    WebserverSession(Webserver *server, brisa::shared::webserver::http::HttpSessionManager *parent);
    ~WebserverSession();

    /*!
     * Check if version of HTTP \param request is supported.
     */
    int isRequestSupported(const brisa::shared::webserver::http::HttpRequest &request) const;

signals:
    void responsePosted(const brisa::shared::webserver::http::HttpResponse &);
    void onEventReceived(brisa::shared::webserver::http::HttpRequest request, WebserverSession* session);

public slots:
    /*!
     * Emit a signal responsePosted.
     * \param r is response that will be emitted to responsePosted
     * \param chunkedResponse set parameter useChunkedResponse
     */
    void respond(brisa::shared::webserver::http::HttpResponse r, bool chunkedResponse = false);
    /*!
     * Emit a HTTP request with code 408 of timeout connections.
     */
    void onTimeout();

protected:
    bool hasEntityBody(const brisa::shared::webserver::http::HttpRequest &request) throw(brisa::shared::webserver::http::HttpResponse);
    bool atEnd(brisa::shared::webserver::http::HttpRequest &request, QByteArray &buffer) throw(brisa::shared::webserver::http::HttpResponse);
    void onRequest(const brisa::shared::webserver::http::HttpRequest &request);

    void prepareResponse(brisa::shared::webserver::http::HttpResponse &);

    /*!
     * Write a HTTP response in a socket.
     * \param r is HHTP response
     * \param s is TCP socket
     */
    void writeEntityBody(const brisa::shared::webserver::http::HttpResponse &r, QTcpSocket *s);

    /*!
     * Start a session and set your timeout as 180000 ms.
     */
    void sessionStarted();

    /*!
     * Clean chunksBuffer and keep connection alive.
     */
    bool keepAlive();

private:
    Webserver *server;
    QTimer *timer;
    int entitySize;

    int chunkedEntity;
    QByteArray chunksBuffer;
    brisa::shared::webserver::http::HttpRequest lastRequest;

    bool useChunkedResponse;
};

}
}
}

#endif // WEBSERVERSESSION_H
