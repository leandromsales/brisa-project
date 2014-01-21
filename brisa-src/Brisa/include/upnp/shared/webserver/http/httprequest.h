#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include "httpmessage.h"
#include "httpversion.h"

namespace brisa {

class HttpRequest: public HttpMessage
{
public:
    explicit HttpRequest(const QByteArray &method = QByteArray(),
                         const QByteArray &uri = "/",
                         const HttpVersion &version = HttpVersion());

    QByteArray method() const;
    void setMethod(const QByteArray &method);

    QByteArray uri() const;
    void setUri(const QByteArray &uri);

private:
    QByteArray m_method;              // char *request_method;
    QByteArray m_uri;                 // char *uri
};

} // namespace Brisa

inline QByteArray brisa::HttpRequest::method() const
{
    return m_method;
}

inline QByteArray brisa::HttpRequest::uri() const
{
    return m_uri;
}

inline QDebug &operator<<(QDebug dbg, const brisa::HttpRequest &request)
{
    dbg.nospace() << "{\n  " << request.method().data() << ' ' << request.uri().data() << ' ' << request.httpVersion() << '\n';
    for (QHash<QByteArray, QByteArray>::const_iterator i = request.headersBeginIterator();i != request.headersEndIterator();++i) {
        dbg.nospace() << "  " << i.key().data() << ": " << i.value().data() << '\n';
    }
    if (request.entitySize())
        dbg.nospace() << " Entity body size: " << request.entitySize() << '\n';
    dbg.nospace() << "}\n";
    return dbg.maybeSpace();
}

#endif // HTTPREQUEST_H
