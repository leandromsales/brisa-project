#include "qcustomreply.h"

#include <QNetworkAccessManager>
#include <QBuffer>
#include <QTimer>

struct QCustomNetworkReplyPrivate
{
    QByteArray content;
    qint64 offset;
};

QCustomReply::QCustomReply (QObject *parent): QNetworkReply(parent)
{
    d = new QCustomNetworkReplyPrivate;
}

QCustomReply::~QCustomReply()
{
    delete d;
}

void QCustomReply::setHttpStatusCode (int code, const QByteArray &statusText)
{
    setAttribute (QNetworkRequest::HttpStatusCodeAttribute, code);
    if (statusText.isNull())
        return;

    setAttribute (QNetworkRequest::HttpReasonPhraseAttribute, statusText);
}

void QCustomReply::setHeader (QNetworkRequest::KnownHeaders header, const QVariant &value)
{
    QNetworkReply::setHeader (header, value);
}

void QCustomReply::setContentType (const QByteArray &contentType)
{
    setHeader (QNetworkRequest::ContentTypeHeader, contentType);
}

void QCustomReply::setContent (const QString &content)
{
    setContent(content.toUtf8());
}

void QCustomReply::setContent (const QByteArray &content)
{
    d->content = content;
    d->offset = 0;

    open(ReadOnly | Unbuffered);
    setHeader(QNetworkRequest::ContentLengthHeader, QVariant(content.size()));

    QTimer::singleShot (0, this, SIGNAL(readyRead()));
    QTimer::singleShot (0, this, SIGNAL(finished()));
}

void QCustomReply::abort()
{
    //
}

qint64 QCustomReply::bytesAvailable() const
{
    return d->content.size() - d->offset;
}

bool QCustomReply::isSequential() const
{
    return true;
}

qint64 QCustomReply::readData(char *data, qint64 maxSize)
{
    if (d->offset >= d->content.size())
        return -1;

    qint64 number = qMin(maxSize, d->content.size() - d->offset);
    memcpy(data, d->content.constData() + d->offset, number);
    d->offset += number;

    return number;
}
