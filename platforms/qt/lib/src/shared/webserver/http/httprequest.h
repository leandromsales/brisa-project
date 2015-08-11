#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include "httpmessage.h"
#include "httpversion.h"

namespace brisa {
namespace shared {
namespace webserver {
namespace http {

class HttpRequest: public HttpMessage
{
public:
    explicit HttpRequest(const QByteArray &method = QByteArray(),
                         const QByteArray &uri = "/",
                         const HttpVersion &version = HttpVersion());

    /*!
     * Return method of HTTP Request, that indicates the method to be performed
     * on the resource identified by the URI.
     */
    QByteArray method() const;
    /*!
     * Set method with value of \param method.
     */
    void setMethod(const QByteArray &method);

    /*!
     * Return URI, Uniform Resource Identifier, of HTTP Request.
     */
    QByteArray uri() const;
    /*!
     * Set URI with value of \param uri.
     */
    void setUri(const QByteArray &uri);

private:
    QByteArray m_method;              // char *request_method;
    QByteArray m_uri;                 // char *uri
};

}  // namespace http
}  // namespace webserver
}  // namespace shared
} // namespace Brisa

inline QByteArray brisa::shared::webserver::http::HttpRequest::method() const
{
    return m_method;
}

inline QByteArray brisa::shared::webserver::http::HttpRequest::uri() const
{
    return m_uri;
}

inline QDebug &operator<<(QDebug dbg, const brisa::shared::webserver::http::HttpRequest &request)
{
    dbg.nospace() << "{\n  " << request.method().data() << ' ' <<
                     request.uri().data() << ' ' << request.httpVersion() << '\n';
    for (QHash<QByteArray, QByteArray>::const_iterator i =
         request.headersBeginIterator();i != request.headersEndIterator();++i) {
        dbg.nospace() << "  " << i.key().data() << ": " << i.value().data() << '\n';
    }
    if (request.entitySize())
        dbg.nospace() << " Entity body size: " << request.entitySize() << '\n';
    dbg.nospace() << "}\n";
    return dbg.maybeSpace();
}

#endif // HTTPREQUEST_H
