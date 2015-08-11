#ifndef SOAP_H
#define SOAP_H

#include <QObject>
#include <QHash>
#include <QNetworkReply>

#include "soapenvelope.h"

class QUrl;
class QByteArray;
class QNetworkAccessManager;

namespace brisa {
namespace shared {
namespace soap {

/*!
 * This class impĺements SOAP - Simple Object Access Protocol
 */
class SOAP : public QObject
{
    Q_OBJECT
public:
    explicit SOAP(QObject *parent = 0);
    ~SOAP();

    /*!
     * Create a new SOAP envelope and a network request. Also emit a signal to handle with errors.
     * \param actionName is name of the UPnP action
     * \param parameters are UPnP parameters of the action
     */
    void executeAction(const QString &actionName, const QHash<QString, QString> &parameters);

    /*!
     * Set UPnP name space
     * \param ns is new value of name space
     */
    void setNameSpace(const QString &ns);

    /*!
     * Get a string that contains UPnP name space
     */
    QString getNameSpace();

    /*!
     * Set an URL
     * \param url is new value of URL
     */
    void setUrl(QUrl &url);

    /*!
     * Get a string that contains URL
     */
    QUrl getUrl();

    /*!
     * Get a string that contains UPnP action name
     */
    QString getActionName();

    /*!
     * Get an array that contains SOAP request envelope
     */
    QByteArray getRequestEnvelope();

    /*!
     * Get an array that contains raw content of response
     */
    QByteArray getRawContent();

    /*!
     * Get an object QNetworkReply that contains network reply
     */
    QNetworkReply *getNetworkReply();

    /*!
     * Get a hash that contains response
     */
    QHash<QString, QString> *getResponse();

private:
    QNetworkAccessManager *downloader;
    QString ns;
    QString actionName;
    QByteArray requestEnvelope;
    QByteArray rawResponse;
    QUrl url;
    QHash<QString, QString> *response;
    QNetworkReply *reply;

signals:
    void responseReady();
    void requestError(QNetworkReply::NetworkError, const QString &);

public slots:

private slots:
    void finished(QNetworkReply *);
    void handleError(QNetworkReply::NetworkError);

};

}
}  // namespace shared
}
#endif // SOAP_H
