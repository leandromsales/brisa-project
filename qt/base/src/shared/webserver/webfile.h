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

    /*!
     * Get name of file.
     */
    QString fileName() const;

    /*!
     * Set a new name to file.
     * \param fileName is the new name
     */
    void setFile(const QString &fileName);

    /*!
     * Get content-type.
     */
    QByteArray contentType() const;

    /*!
     * Set a new content type to file.
     * \param cT is the content type
     */
    void setContentType(const QByteArray &cT);

    /*!
     * Check if file use a chuncked entity or not.
     */
    bool useChunkedEntities() const;

    /*!
     * Set if file use a chuncked entity or not.
     */
    void setUseChunkedEntities(bool u);

protected:
    /*!
     * Deal with new file requests.
     */
    void onRequest(const brisa::shared::webserver::http::HttpRequest &request,
                   WebserverSession *session);

private:
    QString m_fileName;
    QByteArray m_contentType;
    bool m_useChunkedEntities;
};

}
}
}
#endif /* _WEB_FILE_H */
