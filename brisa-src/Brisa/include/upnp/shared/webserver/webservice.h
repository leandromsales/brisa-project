#ifndef WEBSERVICE_H
#define WEBSERVICE_H

#include <QtCore>
#include <QtNetwork>

#include "http/httprequest.h"

namespace brisa {
namespace upnp {

class WebserverSession;

class WebService: public QObject
{
Q_OBJECT
public:
    WebService(QObject *parent = 0);
    ~WebService();

    void postRequest(const HttpRequest &request, WebserverSession *session);

signals:
    void genericRequestReceived(HttpRequest, WebserverSession *session);

protected:
    virtual void onRequest(const HttpRequest &request, WebserverSession *session);
    QByteArray path()
    {
        return m_path;
    }

private:
    QByteArray m_path;

    friend class Webserver;
};

}
}

#endif /* _WEBSERVICE_H */
