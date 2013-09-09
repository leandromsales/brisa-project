#ifndef BRISACONTROLWEBSERVICE_H
#define BRISACONTROLWEBSERVICE_H

#include "upnp/shared/webserver/webservice.h"
#include "upnp/shared/webserver/http/httprequest.h"
#include "upnp/brisaaction.h"

// TODO: use "Class WebService; Class XXX and include their .h in the .cpp

namespace Brisa {

class BrisaControlWebService : public WebService {
    Q_OBJECT
public:
    explicit BrisaControlWebService(const QString &serviceType = QString(), QObject *parent = 0);

signals:
    void requestReceived(QString xml, BrisaInArgument args, WebserverSession *server);
    void invalidRequest(WebserverSession *server);

protected:
    void onRequest(const HttpRequest &request, WebserverSession *session);

private:
    QString serviceType;
};

}

#endif // BRISACONTROLWEBSERVICE_H
