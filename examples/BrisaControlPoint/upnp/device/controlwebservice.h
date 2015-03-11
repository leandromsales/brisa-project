#ifndef CONTROLWEBSERVICE_H
#define CONTROLWEBSERVICE_H

#include "../../shared/webserver/webservice.h"
#include "../../shared/webserver/http/httprequest.h"
#include "../action.h"

// TODO: use "Class WebService; Class XXX and include their .h in the .cpp

namespace brisa {
namespace upnp {
namespace device {

class ControlWebService : public brisa::shared::webserver::WebService {
    Q_OBJECT
public:
    explicit ControlWebService(const QString &serviceType = QString(), QObject *parent = 0);

signals:
    void requestReceived(QString xml, InArgument args, brisa::shared::webserver::WebserverSession *server);
    void invalidRequest(brisa::shared::webserver::WebserverSession *server);

protected:
    void onRequest(const brisa::shared::webserver::http::HttpRequest &request, brisa::shared::webserver::WebserverSession *session);

private:
    QString serviceType;
};

}
}
}

#endif // CONTROLWEBSERVICE_H
