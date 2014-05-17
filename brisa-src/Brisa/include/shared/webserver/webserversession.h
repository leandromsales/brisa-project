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

    int isRequestSupported(const brisa::shared::webserver::http::HttpRequest &request) const;

signals:
    void responsePosted(const brisa::shared::webserver::http::HttpResponse &);
    void onEventReceived(brisa::shared::webserver::http::HttpRequest request, WebserverSession* session);

public slots:
    void respond(brisa::shared::webserver::http::HttpResponse r, bool chunkedResponse = false);
    void onTimeout();

protected:
    bool hasEntityBody(const brisa::shared::webserver::http::HttpRequest &request) throw(brisa::shared::webserver::http::HttpResponse);
    bool atEnd(brisa::shared::webserver::http::HttpRequest &request, QByteArray &buffer) throw(brisa::shared::webserver::http::HttpResponse);
    void onRequest(const brisa::shared::webserver::http::HttpRequest &request);

    void prepareResponse(brisa::shared::webserver::http::HttpResponse &);
    void writeEntityBody(const brisa::shared::webserver::http::HttpResponse &, QTcpSocket *);

    void sessionStarted();
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

}  // namespace webserver
}  // namespace shared
}  // namespace brisa

#endif // WEBSERVERSESSION_H
