#ifndef WEBSERVICE_H
#define WEBSERVICE_H

#include <QtCore>
#include <QtNetwork>

#include "http/httprequest.h"

namespace brisa {
namespace shared {
namespace webserver {

class WebserverSession;

class WebService: public QObject
{
Q_OBJECT
public:
    WebService();
    WebService(QObject *parent = 0);
    ~WebService();

    /*!
     * Emit a signal with a HTTP Request, the parameter \param request, and a
     * Webserver session, the parameter \param session.
     */
    void postRequest(const brisa::shared::webserver::http::HttpRequest &request,
                     WebserverSession *session);

signals:
    /*!
     * This signal is emmited when BrisaWebService receives a HTTP Request,
     * the parameter \param request, in a Webserver session,
     * the parameter \param session.
     */
    void genericRequestReceived(brisa::shared::webserver::http::HttpRequest &request,
                                WebserverSession *session);

protected:
    /*!
     * Emit a generic request, with a HTTP Request, the parameter \param request,
     * and a Webserver session, the parameter \param session.
     */
    virtual void onRequest(const brisa::shared::webserver::http::HttpRequest &request,
                           WebserverSession *session);
    QByteArray path()
    {
        return m_path;
    }

private:
    QByteArray m_path;

    friend class Webserver;
};

}  // namespace webserver
}  // namespace shared
}  // namespace brisa

#endif /* _WEBSERVICE_H */
