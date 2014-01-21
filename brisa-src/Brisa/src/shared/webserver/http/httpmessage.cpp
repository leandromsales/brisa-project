#include "httpmessage.h"
#include <QBuffer>
#include <QIODevice>

using namespace brisa;

HttpMessage::HttpMessage(HttpVersion httpVersion) :
        m_httpVersion(httpVersion),
        m_entityBody(NULL) { }

HttpMessage::HttpMessage(const HttpMessage &m) :
        m_httpVersion(m.m_httpVersion),
        m_headers(m.m_headers),
        m_entityBody(m.m_entityBody),
        m_range(m.m_range)
{
    m.m_entityBody = NULL;
}

HttpMessage::~HttpMessage()
{
    // TODO: if discommented it crashes when the app is closed.
    if (m_entityBody != NULL) {
        //m_entityBody->deleteLater();
    }
}

void HttpMessage::setHttpVersion(const HttpVersion &version)
{
    m_httpVersion = version;
}

void HttpMessage::setHeader(const QByteArray &name, const QByteArray &value)
{
    if (!name.isEmpty()) {
        if (!value.isNull())
            // sets the value
            m_headers[name.toUpper()] = value;
        else
            // erases the header
            m_headers.remove(name.toUpper());
    }
}

void HttpMessage::setEntityBody(const QByteArray &body)
{
    if (m_entityBody && !dynamic_cast<QBuffer *>(m_entityBody)) {
        m_entityBody->deleteLater();
        m_entityBody = NULL;
    }

    if (m_entityBody)
        m_entityBody->close();
    else
        m_entityBody = new QBuffer;

    static_cast<QBuffer *>(m_entityBody)->setData(body);

    m_entityBody->open(QIODevice::ReadOnly);
}

bool HttpMessage::setEntityBody(QIODevice *bodyDevice)
{
    if (!bodyDevice->isReadable() || bodyDevice->isSequential())
        return false;

    if (m_entityBody)
        m_entityBody->deleteLater();

    m_entityBody = bodyDevice;
    m_entityBody->setParent(NULL);

    return true;
}

void HttpMessage::clear()
{
    m_headers.clear();

    if (m_entityBody)
        delete m_entityBody;

    m_range.first = 0;
    m_range.second = 0;

    m_entityBody = NULL;
}

void HttpMessage::setRange(const QPair<qlonglong, qlonglong> &range)
{
    m_range = range;
}
