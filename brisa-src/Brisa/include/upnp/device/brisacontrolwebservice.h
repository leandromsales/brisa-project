#ifndef BRISACONTROLWEBSERVICE_H
#define BRISACONTROLWEBSERVICE_H

#include "shared/webserver/webservice.h"
#include "shared/webserver/http/httprequest.h"
#include "upnp/brisaaction.h"

// TODO: use "Class WebService; Class XXX and include their .h in the .cpp

namespace brisa {
namespace upnp {
namespace device {

class BrisaControlWebService : public brisa::shared::webserver::WebService {
    Q_OBJECT
public:
    explicit BrisaControlWebService(const QString &serviceType = QString(), QObject *parent = 0);

signals:
    void requestReceived(QString xml, BrisaInArgument args, brisa::shared::webserver::WebserverSession *server);
    void invalidRequest(brisa::shared::webserver::WebserverSession *server);

protected:
    void onRequest(const brisa::shared::webserver::http::HttpRequest &request, brisa::shared::webserver::WebserverSession *session);

private:
    QString serviceType;
};

}
}
}

#endif // BRISACONTROLWEBSERVICE_H
