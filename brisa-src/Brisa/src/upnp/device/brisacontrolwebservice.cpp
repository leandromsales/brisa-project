#include "brisacontrolwebservice.h"
#include "brisaactionxmlparser.h"
#include "shared/webserver/http/httpresponse.h"

namespace brisa {
using namespace shared::webserver;
using namespace shared::webserver::http;
namespace upnp {
namespace device {

ControlWebService::ControlWebService(const QString &serviceType, QObject *parent) :
    WebService(parent),
    serviceType(serviceType)
{
}

void ControlWebService::onRequest(const HttpRequest &request, WebserverSession *session)
{
    if (request.method() != "POST") {
        // TODO: close connection?

        HttpResponse r(request.httpVersion(), HttpResponse::METHOD_NOT_ALLOWED);
        return;
    }

    ActionXmlParser actionXmlParser;

    {
        QIODevice *xml = request.entityBody();

        //xml->seek(0);
        //qDebug() << "XML: \n" << QString(xml->readAll());

        xml->seek(0);
        actionXmlParser.setXmlContent(xml->readAll());
    }

    if (actionXmlParser.parseSOAP()) {
        //If servicetype is incorrect
        if (actionXmlParser.serviceType != serviceType)
            return;

        emit requestReceived(actionXmlParser.method, actionXmlParser.args, session);
    } else {
        qDebug() << "BrisaService: Invalid SOAP xml format.";
        emit invalidRequest(session);
    }
}

}
}
}
