#ifndef WEBSTATICCONTENT_H
#define WEBSTATICCONTENT_H

#include <QtCore>
#include <QtNetwork>
#include "webservice.h"

namespace brisa {
namespace shared {
namespace webserver {

    class WebStaticContent: public WebService
    {
    Q_OBJECT
    public:
        WebStaticContent(const QByteArray &content = "", QObject *parent = 0);
        ~WebStaticContent();

        QByteArray content() const;
        void setContent(const QByteArray &content);

        QByteArray contentType() const;
        void setContentType(const QByteArray &contentType);

    protected:
        void onRequest(const brisa::shared::webserver::http::HttpRequest &request, WebserverSession *session);

    private:
        QByteArray m_content;
        QByteArray m_contentType;
    };

}  // namespace webserver
}  // namespace shared
}  // namespace brisa

#endif /* _WEBSTATICCONTENT_H */
