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

    /*!
     * Set status code of QNetworkReply with value of \param code.
     */
    void setHttpStatusCode (int code, const QByteArray &statusText = QByteArray());
    /*!
     * Set headers of message.
     */
    void setHeader (QNetworkRequest::KnownHeaders header, const QVariant &value);
    /*!
     * Set content-type of message.
     */
    void setContentType (const QByteArray &contentType);

    /*!
     * Set content of message using a QString.
     */
    void setContent (const QString &content);
    /*!
     * Set content of message using a QByteArray.
     */
    void setContent (const QByteArray &content);

    /*!
     * Aborts the operation immediately and close down any network connections
     * still open.
     */
    void abort ();
    /*!
     * Returns the number of bytes that are available for reading.
     */
    qint64 bytesAvailable () const;
    /*!
     * Returns true if this device is sequential; otherwise returns false.
     */
    bool isSequential () const;

signals:
    /*!
     * This signal is emitted once every time new data is available for reading
     * from the device.
     */
    void readyRead ();
    /*!
     * This signal is emitted when the reply has finished processing.
     */
    void finished ();

protected:
    qint64 readData (char *data, qint64 maxSize);

private:
    struct CustomNetworkReplyPrivate *d;
};
}
}
}

#endif // CUSTOMREPLY_H
