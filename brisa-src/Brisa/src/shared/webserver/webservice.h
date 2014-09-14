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
    WebService(QObject *parent = 0);
    ~WebService();

    void postRequest(const brisa::shared::webserver::http::HttpRequest &request, WebserverSession *session);

signals:
    void genericRequestReceived(brisa::shared::webserver::http::HttpRequest, WebserverSession *session);

protected:
    virtual void onRequest(const brisa::shared::webserver::http::HttpRequest &request, WebserverSession *session);
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
