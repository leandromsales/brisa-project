#include "httprequest.h"

using namespace Brisa;

HttpRequest::HttpRequest(const QByteArray &method, const QByteArray &uri,
                         const HttpVersion &httpVersion) :
    HttpMessage(httpVersion),
    m_method(method),
    m_uri(uri)
{
}

void HttpRequest::setMethod(const QByteArray &method)
{
    m_method = method;
}

void HttpRequest::setUri(const QByteArray &uri)
{
    m_uri = uri;
}
