#include <QtXml>
#include <QDomDocument>
#include <QDomElement>

#include "soapenvelope.h"

namespace brisa {
namespace shared {
namespace soap {

SoapEnvelope::SoapEnvelope(QObject *parent) : QObject(parent) {
}

QString SoapEnvelope::get(const QString &actionName, const QString &ns, const QHash<QString, QString> &parameters) {
    QString parameters_str = "";
    foreach (QString key, parameters.keys()) {
        parameters_str += "<" + key + ">" + parameters.value(key) + "</" + key + ">";
    }
    QString response = SOAP_REQUEST_MESSAGE.arg(actionName, ns, parameters_str);
    return response;
}

QHash<QString, QString> *SoapEnvelope::parse(const QString &content) {
    QHash<QString, QString> *response = new QHash<QString, QString>();
    QDomDocument document("SOAPEnvelope");
    document.setContent(content, true);

    QString nsURI = "http://schemas.xmlsoap.org/soap/envelope/";
    QDomElement element = document.firstChildElement("Envelope");

    if (element.isNull() || element.namespaceURI() != nsURI) {
        qDebug() << "SOAP: wrong format of envelope.";
        return response;
    }

    element = element.firstChildElement("Body");
    if (element.isNull() || element.namespaceURI() != nsURI) {
        qDebug() << "SOAP: wrong format of body.";
        return response;
    }

    QDomNodeList domNodeList = element.childNodes().item(0).childNodes();
    QDomNode node;
    QString nodeName, nodeValue;
    for (int i = 0; i < domNodeList.length(); i++) {
        node = domNodeList.item(i);
        nodeName = node.toElement().tagName();
        nodeValue = node.childNodes().item(0).nodeValue();
        response->insert(nodeName, nodeValue);
    }
    return response;
}

}
}
}
