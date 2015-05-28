#ifndef CONTROLWEBSERVICE_H
#define CONTROLWEBSERVICE_H

#include "../../shared/webserver/webservice.h"
#include "../../shared/webserver/http/httprequest.h"
#include "../action.h"

namespace brisa {
namespace upnp {
namespace device {

class ControlWebService : public brisa::shared::webserver::WebService {
    Q_OBJECT
public:
    /*!
     * Constructor.
     */
    explicit ControlWebService(const QString &serviceType = QString(), QObject *parent = 0);

signals:
    /*!
     * This signal is emmitted when a new request is received and parsed to SOAP.
     * \param xml is method of action
     * \param args are arguments of action
     * \param server is web server session
     */
    void requestReceived(QString xml, InArgument args, brisa::shared::webserver::WebserverSession *server);
    /*!
     * This signal is emmited when a new request is received but parse to SOAP fail.
     * \param server is web server session
     */
    void invalidRequest(brisa::shared::webserver::WebserverSession *server);

protected:
    /*!
     * Handle with \param request of this \param session. Also write a response, if request is valid.
     */
    void onRequest(const brisa::shared::webserver::http::HttpRequest &request, brisa::shared::webserver::WebserverSession *session);

private:
    QString serviceType;
};

}
}
}

#endif // CONTROLWEBSERVICE_H
