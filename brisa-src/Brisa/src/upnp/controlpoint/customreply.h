#ifndef CUSTOMREPLY_H
#define CUSTOMREPLY_H

#include <QNetworkReply>

namespace brisa {
namespace upnp {
namespace controlpoint {
class CustomReply : public QNetworkReply
{
    Q_OBJECT

public:
    CustomReply (QObject *parent = 0);
    ~CustomReply ();

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
}
}
}

#endif // CUSTOMREPLY_H
