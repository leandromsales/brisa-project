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

        /*!
         * Return a QByteArray with content of WebStaticContent.
         */
        QByteArray content() const;
        /*!
         * Set content of WebStaticContent with \param content.
         */
        void setContent(const QByteArray &content);

        /*!
         * Return a QByteArray with content type of WebStaticContent.
         */
        QByteArray contentType() const;
        /*!
         * Set the WebStaticContent's content type with \param contentType.
         */
        void setContentType(const QByteArray &contentType);

    protected:
        /*!
         * Attach the content and content type of WebStaticContent in a HTTP
         * Request, paramenter \param request.
         * \param session is WebserverSession
         */
        void onRequest(const brisa::shared::webserver::http::HttpRequest &request,
                       WebserverSession *session);

    private:
        QByteArray m_content;
        QByteArray m_contentType;
    };

}  // namespace webserver
}  // namespace shared
}  // namespace brisa

#endif /* _WEBSTATICCONTENT_H */
