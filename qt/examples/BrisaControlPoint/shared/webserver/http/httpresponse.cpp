#include "httpresponse.h"

static const QByteArray reasonPhrases[] =
{
    // 1xx Informational
    "Continue",
    "Switching Protocols",
    "Processing",
    // 2xx Successful
    "OK",
    "Created",
    "Accepted",
    "Non-Authoritative Information",
    "No Content",
    "Reset Content",
    "Partial Content",
    "Multi-Status",
    // 3xx Redirection
    "Multiple Choices",
    "Moved Permanently",
    "Found",
    "See Other",
    "Not Modified",
    "Use Proxy",
    "Switch Proxy",
    "Temporary Redirect",
    // 4xx Client Error
    "Bad Request",
    "Unauthorized",
    "Payment Required",
    "Forbidden",
    "Not Found",
    "Method Not Allowed",
    "Not Acceptable",
    "Proxy Authentication Required",
    "Request Timeout",
    "Conflict",
    "Gone",
    "Length Required",
    "Precondition Failed",
    "Request Entity Too Large",
    "Request-URI Too Long",
    "Unsupported Media Type",
    "Requested Range Not Satisfiable",
    "Expectation Failed",
    "I'm a teapot", // IETF joke
    "Unprocessable Entity",
    "Locked",
    "Failed Dependency",
    "Unordered Collection",
    "Upgrade Required",
    // 5xx Internal Server Error
    "Internal Server Error",
    "Not Implemented",
    "Bad Gateway",
    "Service Unavailable",
    "Gateway Timeout",
    "HTTP Version Not Supported",
    "Variant Also Negotiates",
    "Insufficient Storage",
    "Not Extended"
};

inline int statusCodeToReasonPhrasesIndex(int st)
{
    int index = 0;
    // invalid
    if (st < 100) {
        return -1;
    }
    // 1xx code
    if (st < 200) {
        if (st < 103)
            return index + (st - 100);
        else
            return -1;
    } else {
        index += 3;
    }
    // 2xx code
    if (st < 300) {
        if (st < 208)
            return index + (st - 200);
        else
            return -1;
    } else {
        index += 8;
    }
    // 3xx code
    if (st < 400) {
        if (st < 308)
            return index + (st - 300);
        else
            return -1;
    } else {
        index += 8;
    }
    // 4xx code
    if (st < 500) {
        if (st < 419)
            return index + (st - 400);
        else if (st > 421 && st < 427)
            return index + (st - 403);
        else
            return -1;
    } else {
        index += 24;
    }
    // 5xx code
    if (st < 508)
        return index + (st - 500);
    else if (st == 510)
        return index + 8;
    else
        return -1;
}

namespace brisa {
namespace shared {
namespace webserver {
namespace http {

HttpResponse::HttpResponse(const HttpVersion &httpVersion, int statusCode,
                           bool closeConnection) :
    HttpMessage(httpVersion),
    m_statusCode(statusCode),
    m_closeConnection(closeConnection)
{
    int i = statusCodeToReasonPhrasesIndex(statusCode);
    if (i != -1)
        m_reasonPhrase = reasonPhrases[i];
}

HttpResponse::HttpResponse(const HttpVersion &httpVersion, int statusCode,
                           const QByteArray &reasonPhrase, bool closeConnection) :
    HttpMessage(httpVersion),
    m_statusCode(statusCode),
    m_reasonPhrase(reasonPhrase),
    m_closeConnection(closeConnection)
{
}

bool HttpResponse::setStatusCode(int st)
{
    int i = statusCodeToReasonPhrasesIndex(st);
    if (i != -1) {
        m_statusCode = st;
        m_reasonPhrase = reasonPhrases[i];
        return true;
    } else {
        return false;
    }
}

void HttpResponse::setStatusCode(int st, const QByteArray &reasonPhrase)
{
    m_statusCode = st;
    m_reasonPhrase = reasonPhrase;
}

void HttpResponse::setReasonPhrase(const QByteArray &reasonPhrase)
{
    m_reasonPhrase = reasonPhrase;
}

void HttpResponse::setCloseConnection(bool close)
{
    m_closeConnection = close;
}

}  // namespace http
}  // namespace webserver
}  // namespace shared
} // namespace Brisa
