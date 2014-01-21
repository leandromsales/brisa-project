#include "webservice.h"

#define DEFAULT_PAGE "<html><body><h1>BRisa WebServer!</h1></body></html>\n"

namespace brisa {
namespace upnp {

WebService::WebService(QObject *parent) :
    QObject(parent) { }

WebService::~WebService() { }

void WebService::postRequest(const HttpRequest &request, WebserverSession *session)
{
    onRequest(request, session);
}

void WebService::onRequest(const HttpRequest &request, WebserverSession *session)
{
    emit genericRequestReceived(request, session);
}

}
}
