#ifndef SOAP_H
#define SOAP_H

#include <QObject>
#include <QHash>
#include <QNetworkReply>

#include "soapenvelope.h"

class QUrl;
class QByteArray;
class QNetworkAccessManager;

class SOAP : public QObject
{
    Q_OBJECT
public:
    explicit SOAP(QObject *parent = 0);
    ~SOAP();

    void executeAction(const QString &, const QHash<QString, QString> &);
    void setNameSpace(const QString &);
    QString getNameSpace();
    void setUrl(QUrl &);
    QUrl getUrl();
    QString getActionName();
    QByteArray getRequestEnvelope();
    QByteArray getRawContent();
    QNetworkReply *getNetworkReply();
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

#endif // SOAP_H
