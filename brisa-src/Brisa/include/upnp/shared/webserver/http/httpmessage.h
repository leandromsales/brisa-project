#ifndef HTTPMESSAGE_H
#define HTTPMESSAGE_H

#include "httpversion.h"
#include <QHash>

class QIODevice;

namespace brisa {

class HttpMessage
{
public:
    HttpMessage(HttpVersion httpVersion);
    HttpMessage(const HttpMessage &);
    ~HttpMessage();

    HttpVersion httpVersion() const;
    void setHttpVersion(const HttpVersion &version);

    QByteArray header(const QByteArray &name) const;
    /*
      if value is null, the header is removed (if exists)
      */
    void setHeader(const QByteArray &name, const QByteArray &value);
    QHash<QByteArray, QByteArray> headers() const;
    QHash<QByteArray, QByteArray>::const_iterator headersBeginIterator() const;
    QHash<QByteArray, QByteArray>::const_iterator headersEndIterator() const;

    qint64 entitySize() const;
    QIODevice *entityBody() const;

    // creates a QBuffer
    void setEntityBody(const QByteArray &body);

    QPair<qlonglong, qlonglong> range() const;
    void setRange(const QPair<qlonglong, qlonglong> &);

    bool useRange() const;

    /*
      the \p bodyDevice must be opened in readMode and must be
      random-access (not sequencial) device

      takes ownership of bodyDevice if successful
      */
    bool setEntityBody(QIODevice *bodyDevice);

    // clear the headers and entity body
    void clear();

private:
    HttpVersion m_httpVersion;

    // the maximum number of headers should be 64
    QHash<QByteArray, QByteArray> m_headers;

    mutable QIODevice *m_entityBody;
    QPair<qlonglong, qlonglong> m_range;
};

} // namespace Brisa

inline brisa::HttpVersion brisa::HttpMessage::httpVersion() const
{
    return m_httpVersion;
}

inline QByteArray brisa::HttpMessage::header(const QByteArray &name) const
{
    return m_headers.value(name.toUpper());
}

inline QHash<QByteArray, QByteArray> brisa::HttpMessage::headers() const
{
    return m_headers;
}

inline QHash<QByteArray, QByteArray>::const_iterator brisa::HttpMessage::headersBeginIterator() const
{
    return m_headers.begin();
}

inline QHash<QByteArray, QByteArray>::const_iterator brisa::HttpMessage::headersEndIterator() const
{
    return m_headers.end();
}

inline qint64 brisa::HttpMessage::entitySize() const
{
    if(m_entityBody) {
        return m_entityBody->size();
    } else {
        return 0;
    }
}

inline QIODevice *brisa::HttpMessage::entityBody() const
{
    return m_entityBody;
}

inline QPair<qlonglong, qlonglong> brisa::HttpMessage::range() const
{
    return m_range;
}

inline bool brisa::HttpMessage::useRange() const
{
    return (m_range.second != 0);
}

#endif // HTTPMESSAGE_H
