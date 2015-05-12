#ifndef HTTPMESSAGE_H
#define HTTPMESSAGE_H

#include <QHash>

#include "httpversion.h"

class QIODevice;

namespace brisa {
namespace shared {
namespace webserver {
namespace http {

class HttpMessage
{
public:
    HttpMessage(HttpVersion httpVersion);
    HttpMessage(const HttpMessage &);
    ~HttpMessage();

    /*!
     * Return the HTTP version of message.
     */
    HttpVersion httpVersion() const;
    /*!
     * Set the HTTP version of message with value \param version.
     */
    void setHttpVersion(const HttpVersion &version);

    /*!
     * Return name of header content.
     */
    QByteArray header(const QByteArray &name) const;
    /*!
     * Set header content with \param name. If \param name is null, the header is
     * removed.
     */
    void setHeader(const QByteArray &name, const QByteArray &value);
    /*!
     * Return header of message.
     */
    QHash<QByteArray, QByteArray> headers() const;
    /*!
     * Return an iterator pointing to first item of message's header.
     */
    QHash<QByteArray, QByteArray>::const_iterator headersBeginIterator() const;
    /*!
     * Return an iterator pointing to the imaginary item after the last item in
     * message's header.
     */
    QHash<QByteArray, QByteArray>::const_iterator headersEndIterator() const;

    /*!
     * Return size of message's body.
     */
    qint64 entitySize() const;
    /*!
     * Return message's body.
     */
    QIODevice *entityBody() const;
    /*!
     * Set content of message's body with \param body.
     */
    void setEntityBody(const QByteArray &body);

    /*!
     * Return a range of response entity.
     */
    QPair<qlonglong, qlonglong> range() const;
    /*!
     * Set range of response entity.
     */
    void setRange(const QPair<qlonglong, qlonglong> &);
    /*!
     * Allows a client to request that only part (a range of) the response
     * entity be included within the response.
     */
    bool useRange() const;

    /*!
     * Takes ownership of \param bodyDevice if successful. Note the
     * \param bodyDevice must be opened in readMode and must be a random-access
     * (not sequencial) device.
     */
    bool setEntityBody(QIODevice *bodyDevice);

    /*!
     * Clear headers and entity body.
     */
    void clear();

private:
    HttpVersion m_httpVersion;

    // the maximum number of headers should be 64
    QHash<QByteArray, QByteArray> m_headers;

    mutable QIODevice *m_entityBody;
    QPair<qlonglong, qlonglong> m_range;
};

}  // namespace http
}  // namespace webserver
}  // namespace shared
} // namespace Brisa

inline brisa::shared::webserver::http::HttpVersion brisa::shared::webserver::http::HttpMessage::httpVersion() const
{
    return m_httpVersion;
}

inline QByteArray brisa::shared::webserver::http::HttpMessage::header(const QByteArray &name) const
{
    return m_headers.value(name.toUpper());
}

inline QHash<QByteArray, QByteArray> brisa::shared::webserver::http::HttpMessage::headers() const
{
    return m_headers;
}

inline QHash<QByteArray, QByteArray>::const_iterator brisa::shared::webserver::http::HttpMessage::headersBeginIterator() const
{
    return m_headers.begin();
}

inline QHash<QByteArray, QByteArray>::const_iterator brisa::shared::webserver::http::HttpMessage::headersEndIterator() const
{
    return m_headers.end();
}

inline qint64 brisa::shared::webserver::http::HttpMessage::entitySize() const
{
    if(m_entityBody) {
        return m_entityBody->size();
    } else {
        return 0;
    }
}

inline QIODevice *brisa::shared::webserver::http::HttpMessage::entityBody() const
{
    return m_entityBody;
}

inline QPair<qlonglong, qlonglong> brisa::shared::webserver::http::HttpMessage::range() const
{
    return m_range;
}

inline bool brisa::shared::webserver::http::HttpMessage::useRange() const
{
    return (m_range.second != 0);
}

#endif // HTTPMESSAGE_H
