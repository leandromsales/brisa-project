#include <QByteArray>
#include <QHostAddress>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStringList>

#include "soap.h"
#include "soapenvelope.h"

//TODO: make it as a singleton?

SOAP::SOAP(QObject *parent) : QObject(parent) {
    this->downloader = new QNetworkAccessManager(this);
    connect(downloader, SIGNAL(finished(QNetworkReply *)), this, SLOT(finished(QNetworkReply *)));
    this->response = NULL;
    this->reply = NULL;
}

SOAP::~SOAP() {
    if (this->response != NULL) {
        delete this->response;
    }
    delete this->downloader;
}

void SOAP::setNameSpace(const QString &ns) {
    this->ns = ns;
}

QString SOAP::getNameSpace() {
    return this->ns;
}

void SOAP::setUrl(QUrl &url) {
    this->url = url;
}

QUrl SOAP::getUrl() {
    return this->url;
}

QString SOAP::getActionName() {
    QStringList ns_an = this->actionName.split("#");
    return ns_an[ns_an.count()-1];
}

QByteArray SOAP::getRequestEnvelope() {
    return this->requestEnvelope;
}

QByteArray SOAP::getRawContent() {
    return this->rawResponse;
}

QNetworkReply *SOAP::getNetworkReply() {
    return this->reply;
}

QHash<QString, QString> *SOAP::getResponse() {
    return this->response;
}

void SOAP::executeAction(const QString &actionName, const QHash<QString, QString> &parameters) {
    if (this->url.isValid()) {
        SoapEnvelope soapEnvelope;
        this->actionName = actionName;
        this->requestEnvelope = soapEnvelope.get(actionName, this->ns, parameters).toUtf8();
        //qDebug() << this->requestEnvelope;
        //this->requestEnvelope = "<?xml version=\"1.0\"?><s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body><u:Browse xmlns:u=\"urn:schemas-upnp-org:service:ContentDirectory:1\"><BrowseFlag>BrowseDirectChildren</BrowseFlag><ObjectID>0</ObjectID></u:Browse></s:Body></s:Envelope>";

        QNetworkRequest request(this->url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml; charset=\"utf-8\"");
        request.setRawHeader("SOAPAction", QString("\"" + this->ns + "#" + actionName + "\"").toUtf8());
        reply = this->downloader->post(request, requestEnvelope);
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handleError(QNetworkReply::NetworkError)));
    }
}

void SOAP::finished(QNetworkReply *reply) {
    SoapEnvelope soapEnvelope;
    this->rawResponse = reply->readAll();
    if (this->response) {
        delete this->response;
    }
    this->response = soapEnvelope.parse(this->rawResponse);

    reply->deleteLater();
    emit responseReady();
}

void SOAP::handleError(QNetworkReply::NetworkError code) {
    emit requestError(code, this->reply->errorString());
}
