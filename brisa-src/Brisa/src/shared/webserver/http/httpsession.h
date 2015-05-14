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

    /*!
     * Set value of \param socketDescriptor, this argument is a socket descriptor
     * for the session.
     */
    void setSession(qintptr socketDescriptor);

    /*!
     * Return zero if \param request is a valid HTTP Request. Otherwise, return
     * 505, status code to HTTP_VERSION_NOT_SUPPORTED.
     */
    virtual int isRequestSupported(const HttpRequest &request) const;

protected slots:
    /*!
     * Build header and body of HTTP Response.
     */
    void writeResponse(brisa::shared::webserver::http::HttpResponse);

protected:
    /*!
     * Through content-length and content-size of \param request, this method define
     * is \param request is a chunked entity or not.
     */
    virtual bool hasEntityBody(const HttpRequest &request) throw(HttpResponse) = 0;
    /*!
     * returns true when the entity body was fully received.
     */
    // TO DO: the entity body should be put on qiodevice buffer, not in memory
    virtual bool atEnd(HttpRequest &request, QByteArray &buffer) throw(HttpResponse) = 0;
    /*!
     * Write an appropriate response to \param request.
     */
    virtual void onRequest(const HttpRequest &request) = 0;

    /*!
     * Set content length on header of \param r.
     */
    virtual void prepareResponse(HttpResponse &r);
    /*!
     * Write a HTTP response in a socket.
     * \param r is HHTP response.
     * \param s is TCP socket.
     */
    virtual void writeEntityBody(const HttpResponse &r, QTcpSocket *s);

    virtual void sessionStarted();

    /*!
     * This function is called every time a connection is closed. Should return
     * true, if the object must keep alive, or false, if the object should be deleted.
     */
    virtual bool keepAlive()
    {
        return false;
    }

private slots:
    /*!
     * Read socket and try write an appropriate response.
     */
    void onReadyRead();
    /*!
     * Clear buffers and set states when a connection is closed.
     */
    void onConnectionDie();

protected:
    HttpVersion lastSupportedHttpVersion;

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
