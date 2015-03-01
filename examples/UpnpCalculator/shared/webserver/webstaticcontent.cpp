#include "webstaticcontent.h"
#include "webserversession.h"

namespace brisa {
namespace shared {
namespace webserver {

using namespace http;

WebStaticContent::WebStaticContent(const QByteArray &content,
                                             QObject *parent) :
        WebService(parent),
        m_content(content)
{
    qDebug() << "INSTANCIOU WEB STATIC CONTENT";
}

WebStaticContent::~WebStaticContent() { }

QByteArray WebStaticContent::content() const
{
    return m_content;
}

void WebStaticContent::setContent(const QByteArray &content)
{
    m_content = content;
}

QByteArray WebStaticContent::contentType() const
{
    return m_contentType;
}

void WebStaticContent::setContentType(const QByteArray &contentType)
{
    m_contentType = contentType;
}

void WebStaticContent::onRequest(const HttpRequest &request,
                                      WebserverSession *session)
{
    qDebug() << "ON REQUEST";
    HttpResponse response(request.httpVersion());

    response.setHeader("CONTENT-TYPE", m_contentType);
    response.setHeader("CONTENT-LENGTH", QByteArray::number(m_content.size()));
    response.setEntityBody(m_content);

    session->respond(response);
}

}  // namespace webserver
}  // namespace shared
}  // namespace brisa
