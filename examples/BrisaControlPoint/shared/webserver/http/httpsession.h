#ifndef HTTPSESSION_H
#define HTTPSESSION_H

#include <QObject>
#include <QDateTime>
#include "httprequest.h"
#include "httpresponse.h"

class QTcpSocket;

namespace brisa {
namespace shared {
namespace webserver {
namespace http {

class HttpSessionManager;

class HttpSession: public QObject
{
Q_OBJECT
public:
    explicit HttpSession(HttpSessionManager *sessionManager);
    virtual ~HttpSession();

    void setSession(qintptr socketDescriptor);

    // @ret should return the HTTP response status (404 not found, method not implemented,
    // ...) or 0. If a 0 is returned, the server continues to read the request, otherwise,
    // respond the request with the specified response code and closes the connection.
    // Default implementation returns 0 if version is lower or equals to the
    // last supported version or HTTP_VERSION_NOTSUPPORTED otherwise.
    // used to identify if this http request (version, method, uri, ...) is supported
    // always close the connection after respond the message
    virtual int isRequestSupported(const HttpRequest &request) const;

protected slots:
    void writeResponse(brisa::shared::webserver::http::HttpResponse);

protected:
    // used to respond BAD_REQUESTs
    // must be set in the constructor (HttpVersion isn't thread-safe yet)
    HttpVersion lastSupportedHttpVersion;

    virtual bool hasEntityBody(const HttpRequest &request) throw(HttpResponse) = 0;
    // @ret returns true when the entity body was fully received
    // in future, the entity body should be put on qiodevice buffer, not in memory
    virtual bool atEnd(HttpRequest &request, QByteArray &buffer) throw(HttpResponse) = 0;
    virtual void onRequest(const HttpRequest &request) = 0;

    virtual void prepareResponse(HttpResponse &);
    virtual void writeEntityBody(const HttpResponse &, QTcpSocket *);

    virtual void sessionStarted()
    {
    }

    // this function is called every time a connection is closed
    // should return true if the object must keep alive or false
    // if the object should be deleted
    virtual bool keepAlive()
    {
        return false;
    }

private slots:
    void onReadyRead();
    void onConnectionDie();

private:
    HttpSessionManager *sessionManager;
    QTcpSocket *socket;
    HttpRequest requestInfo;
    int state;
    QByteArray buffer;
};

}  // namespace http
}  // namespace webserver
}  // namespace shared
} // namespace Brisa

#endif // HTTPSESSION_H
