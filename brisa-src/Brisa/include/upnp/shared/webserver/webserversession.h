#ifndef WEBSERVERSESSION_H
#define WEBSERVERSESSION_H

#include "http/httpsession.h"

class QTimer;

namespace Brisa {

class Webserver;

class WebserverSession : public HttpSession
{
Q_OBJECT
public:
    WebserverSession(Webserver *server, HttpSessionManager *parent);
    ~WebserverSession();

    int isRequestSupported(const HttpRequest &request) const;

signals:
    void responsePosted(HttpResponse);
    void onEventReceived(HttpRequest request, WebserverSession* session);

public slots:
    void respond(HttpResponse r, bool chunkedResponse = false);
    void onTimeout();

protected:
    bool hasEntityBody(const HttpRequest &request) throw(HttpResponse);
    bool atEnd(HttpRequest &request, QByteArray &buffer) throw(HttpResponse);
    void onRequest(const HttpRequest &request);

    void prepareResponse(HttpResponse &);
    void writeEntityBody(const HttpResponse &, QTcpSocket *);

    void sessionStarted();
    bool keepAlive();

private:
    Webserver *server;
    QTimer *timer;
    int entitySize;

    int chunkedEntity;
    QByteArray chunksBuffer;
    HttpRequest lastRequest;

    bool useChunkedResponse;
};

} // namespace Brisa

#endif // WEBSERVERSESSION_H
