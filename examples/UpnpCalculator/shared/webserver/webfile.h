#ifndef WEB_FILE_H
#define WEB_FILE_H

#include <QtCore>

#include "webservice.h"

namespace brisa {
namespace shared {
namespace webserver {

    class WebFile: public WebService
    {
    Q_OBJECT
    public:
        WebFile(const QString &filePath = QString(), QObject *parent = 0);
        ~WebFile();

        QString fileName() const;
        void setFile(const QString &fileName);

        QByteArray contentType() const;
        void setContentType(const QByteArray &);

        bool useChunkedEntities() const;
        void setUseChunkedEntities(bool);

    protected:
        void onRequest(const brisa::shared::webserver::http::HttpRequest &request, WebserverSession *session);

    private:
        QString m_fileName;
        QByteArray m_contentType;
        bool m_useChunkedEntities;
    };

    inline QString WebFile::fileName() const
    {
        return m_fileName;
    }

    inline QByteArray WebFile::contentType() const
    {
        return m_contentType;
    }

    inline bool WebFile::useChunkedEntities() const
    {
        return m_useChunkedEntities;
    }

}  // namespace webserver
}  // namespace shared
}  // namespace brisa

#endif /* _WEB_FILE_H */
