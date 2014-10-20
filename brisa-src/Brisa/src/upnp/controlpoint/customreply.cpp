#include "customreply.h"

#include <QNetworkAccessManager>
#include <QBuffer>
#include <QTimer>

namespace brisa {
namespace upnp {
namespace controlpoint {

struct CustomNetworkReplyPrivate
{
    QByteArray content;
    qint64 offset;
};

CustomReply::CustomReply (QObject *parent): QNetworkReply(parent)
{
    d = new CustomNetworkReplyPrivate();
}

CustomReply::~CustomReply()
{
    delete d;
}

void CustomReply::setHttpStatusCode (int code, const QByteArray &statusText)
{
    setAttribute (QNetworkRequest::HttpStatusCodeAttribute, code);
    if (statusText.isNull())
        return;

    setAttribute (QNetworkRequest::HttpReasonPhraseAttribute, statusText);
}

void CustomReply::setHeader (QNetworkRequest::KnownHeaders header, const QVariant &value)
{
    QNetworkReply::setHeader (header, value);
}

void CustomReply::setContentType (const QByteArray &contentType)
{
    setHeader (QNetworkRequest::ContentTypeHeader, contentType);
}

void CustomReply::setContent (const QString &content)
{
    setContent(content.toUtf8());
}

void CustomReply::setContent (const QByteArray &content)
{
    d->content = content;
    d->offset = 0;

    open(ReadOnly | Unbuffered);
    setHeader(QNetworkRequest::ContentLengthHeader, QVariant(content.size()));

    QTimer::singleShot (0, this, SIGNAL(readyRead()));
    QTimer::singleShot (0, this, SIGNAL(finished()));
}

void CustomReply::abort()
{
    //
}

qint64 CustomReply::bytesAvailable() const
{
    return d->content.size() - d->offset;
}

bool CustomReply::isSequential() const
{
    return true;
}

qint64 CustomReply::readData(char *data, qint64 maxSize)
{
    if (d->offset >= d->content.size())
        return -1;

    qint64 number = qMin(maxSize, d->content.size() - d->offset);
    memcpy(data, d->content.constData() + d->offset, number);
    d->offset += number;

    return number;
}

}
}
}
