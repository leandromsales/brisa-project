#ifndef QCUSTOMREPLY_H
#define QCUSTOMREPLY_H

#include <QNetworkReply>

class QCustomReply : public QNetworkReply
{
    Q_OBJECT

public:
    QCustomReply (QObject *parent = 0);
    ~QCustomReply ();

    void setHttpStatusCode (int code, const QByteArray &statusText = QByteArray());
    void setHeader (QNetworkRequest::KnownHeaders header, const QVariant &value);
    void setContentType (const QByteArray &contentType);

    void setContent (const QString &content);
    void setContent (const QByteArray &content);

    void abort ();
    qint64 bytesAvailable () const;
    bool isSequential () const;

signals:
    void readyRead ();
    void finished ();

protected:
    qint64 readData (char *data, qint64 maxSize);

private:
    struct QCustomNetworkReplyPrivate *d;
};

#endif // QCUSTOMREPLY_H
