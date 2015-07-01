#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include "httpmessage.h"
#include <QHash>

namespace brisa {
namespace shared {
namespace webserver {
namespace http {

class HttpResponse: public HttpMessage
{
public:
    enum StatusCode
    {
        // 1xx Informational
        CONTINUE = 100,
        SWITCHING_PROTOCOLS = 101,
        PROCESSING = 102,
        // 2xx Successful
        OK = 200,
        CREATED = 201,
        ACCEPTED = 202,
        NON_AUTHORITATIVE_INFORMATION = 203,
        NO_CONTENT = 204,
        RESET_CONTENT = 205,
        PARTIAL_CONTENT = 206,
        MULTI_STATUS = 207,
        // 3xx Redirection
        MULTIPLE_CHOICES = 300,
        MOVED_PERMANENTLY = 301,
        FOUND = 302,
        SEE_OTHER = 303,
        NOT_MODIFIED = 304,
        USE_PROXY = 305,
        SWITCH_PROXY = 306,
        TEMPORARY_REDIRECT = 307,
        // 4xx Client Error
        BAD_REQUEST = 400,
        UNAUTHORIZED = 401,
        PAYMENT_REQUIRED = 402,
        FORBIDDEN = 403,
        NOT_FOUND = 404,
        METHOD_NOT_ALLOWED = 405,
        NOT_ACCEPTABLE = 406,
        PROXY_AUTHENTICATION_REQUIRED = 407,
        REQUEST_TIMEOUT = 408,
        CONFLICT = 409,
        GONE = 410,
        LENGTH_REQUIRED = 411,
        PRECONDITION_FAILED = 412,
        REQUEST_ENTITY_TOO_LARGE = 413,
        REQUEST_URI_TOO_LONG = 414,
        UNSUPPORTED_MEDIA_TYPE = 415,
        REQUESTED_RANGE_NOT_SATISFIABLE = 416,
        EXPECTATION_FAILED = 417,
        I_AM_A_TEAPOT = 418, // IETF joke
        UNPROCESSABLE_ENTITY = 422,
        LOCKED = 423,
        FAILED_DEPENDENCY = 424,
        UNORDERED_COLLECTION = 425,
        UPGRADE_REQUIRED = 426,
        // 5xx Internal Server Error
        INTERNAL_SERVER_ERROR = 500,
        NOT_IMPLEMENTED = 501,
        BAD_GATEWAY = 502,
        SERVICE_UNAVAILABLE = 503,
        GATEWAY_TIMEOUT = 504,
        HTTP_VERSION_NOT_SUPPORTED = 505,
        VARIANT_ALSO_NEGOTIATES = 506,
        INSUFFICIENT_STORAGE = 507,
        NOT_EXTENDED = 510
    };

public:
    // If reasonPhrase is null and the status code is know, then the default
    // reason phrase to the is set
    explicit HttpResponse(const HttpVersion &httpVersion, int statusCode = OK,
                          bool closeConnection = false);
    explicit HttpResponse(const HttpVersion &httpVersion, int statusCode,
                          const QByteArray &reasonPhrase,
                          bool closeConnection = false);

    int statusCode() const;
    bool setStatusCode(int st);
    void setStatusCode(int st, const QByteArray &reasonPhrase);

    QByteArray reasonPhrase() const;
    void setReasonPhrase(const QByteArray &reasonPhrase);

    bool closeConnection() const;
    void setCloseConnection(bool close);

private:
    int m_statusCode;
    QByteArray m_reasonPhrase;
    bool m_closeConnection;
};

}  // namespace http
}  // namespace webserver
}  // namespace shared
} // namespace brisa

inline int brisa::shared::webserver::http::HttpResponse::statusCode() const
{
    return m_statusCode;
}

inline QByteArray brisa::shared::webserver::http::HttpResponse::reasonPhrase() const
{
    return m_reasonPhrase;
}

inline bool brisa::shared::webserver::http::HttpResponse::closeConnection() const
{
    return m_closeConnection;
}

inline QDebug &operator<<(QDebug dbg, const brisa::shared::webserver::http::HttpResponse &response)
{
    dbg.nospace() << "{\n  " << response.httpVersion() << ' ' << response.statusCode() << ' ' << response.reasonPhrase().data() << '\n';
    for (QHash<QByteArray, QByteArray>::const_iterator i = response.headersBeginIterator();i != response.headersEndIterator();++i) {
        dbg.nospace() << "  " <<  i.key().data() << ": " << i.value().data() << '\n';
    }
    if (response.entitySize())
        dbg.nospace() << " Entity body size: " << response.entitySize() << '\n';
    dbg.nospace() << "}\n";
    return dbg.maybeSpace();
}

#endif // HTTPRESPONSE_H
