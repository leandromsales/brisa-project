#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTemporaryFile>
#include <QDomDocument>
#include <QDebug>

#include "servicesales.h"
#include "../../shared/soap/soap.h"

namespace brisa {
using namespace shared::soap;
namespace upnp {
namespace controlpoint {

ServiceSales::ServiceSales(QObject *parent) : QObject(parent) {
    this->downloader = new QNetworkAccessManager(this);
    connect(downloader, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    this->soapHttp = new SOAP(this);
    connect(soapHttp, SIGNAL(responseReady()), this, SLOT(handleResponseReady()));
    connect(soapHttp, SIGNAL(requestError(QNetworkReply::NetworkError, QString)), this, SLOT(handleRequestError(QNetworkReply::NetworkError, QString)));
}

ServiceSales::ServiceSales(const QString &major,
                 const QString &minor,
                 const QString &serviceType,
                 const QString &serviceId,
                 const QString &scpdUrl,
                 const QString &controlUrl,
                 const QString &eventSubUrl,
                 const QUrl &baseUrl,
                 const QHash<QString, ActionSales*> &actionList,
                 const QHash<QString, StateVariableSales*> &stateVariableList,
                 QObject *parent) : QObject(parent) {

    this->setAttribute("major", major);
    this->setAttribute("minor", minor);
    this->setAttribute("servicetype", serviceType);
    this->setAttribute("serviceid", serviceId);
    this->setAttribute("scpdurl",scpdUrl);
    this->setAttribute("controlurl", controlUrl);
    this->setAttribute("eventsuburl", eventSubUrl);
    this->baseUrl = baseUrl;
    this->actionList = actionList;
    this->stateVariableList = stateVariableList;

    this->downloader = new QNetworkAccessManager(this);
    connect(downloader, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

    this->soapHttp = new SOAP(this);
}

ServiceSales::ServiceSales(const ServiceSales &service) : QObject(service.parent()) {
    *this = service;
}

ServiceSales::~ServiceSales() {
    delete this->downloader;
    delete this->soapHttp;
    qDeleteAll(this->actionList.values());
    qDeleteAll(this->stateVariableList.values());
}

ServiceSales &ServiceSales::operator=(const ServiceSales &service) {
    if (this != &service) {
        // review to see if this is really necessary
        this->attributes = service.getAttributes();
        this->actionList = service.getActions();
        this->stateVariableList = service.getStateVariables();
    }
    return *this;
}

void ServiceSales::setBaseUrl(const QUrl &baseUrl, bool loadDescription) {
    this->baseUrl = baseUrl;
    if (loadDescription) {
        QString scdpPath = this->attributes["scpdurl"];
        if (!scdpPath.isEmpty()) {
            QUrl scdpUrl = QUrl(this->baseUrl);
            scdpUrl.setPath(scdpPath);
            this->downloader->get(QNetworkRequest(scdpUrl));
        }
    }
}

bool ServiceSales::setAttribute(const QString &attributeName, const QString &attributeValue) {
    this->attributes[attributeName.toLower()] = attributeValue;
    return true; // use this semantic for future validation
}

QString ServiceSales::getAttribute(const QString &attributeName) {
    return this->attributes.value(attributeName.toLower());
}

QHash<QString, QString> ServiceSales::getAttributes() const {
    return this->attributes;
}

void ServiceSales::setActions(QHash<QString, ActionSales *> actionList) {
    this->actionList = actionList;
}

QHash<QString, ActionSales*> ServiceSales::getActions() const {
    return this->actionList;
}

ActionSales *ServiceSales::getAction(const QString &actionName) {
    return this->actionList.value(actionName);
}

void ServiceSales::setStateVariables(QHash<QString, StateVariableSales*> stateVariableList) {
    this->stateVariableList = stateVariableList;
}

QHash<QString, StateVariableSales *> ServiceSales::getStateVariables() const {
    return this->stateVariableList;
}

StateVariableSales *ServiceSales::getStateVariable(const QString &stateVariableName) const {
    return this->stateVariableList.value(stateVariableName);
}

void ServiceSales::replyFinished(QNetworkReply *reply) {
    QTemporaryFile *descriptionXml = new QTemporaryFile();
    if (!descriptionXml->open()) {
        emit errorParsingServiceDescription(this, ServiceSales::CREATE_TMP_XML_ERROR);
    } else {
        //qDebug() << reply->readAll();
        descriptionXml->write(reply->readAll());
        descriptionXml->seek(0);

        quint8 result = this->parseXMLDescription(descriptionXml);
        if (result) {
            emit serviceDescriptionReady(this);
        } else {
            emit errorParsingServiceDescription(this, result);
        }

        descriptionXml->remove();
        delete descriptionXml;
    }
    reply->deleteLater();
}

quint8 ServiceSales::parseXMLDescription(QTemporaryFile *description) {
    QDomDocument document("Service");
    document.setContent(description);
    QDomElement element = document.documentElement();
    if (element.tagName() != "scpd") {
        return ServiceSales::MALFORMED_XML_DESCRIPTION_ERROR;
    }

    QDomNodeList domNodeList = element.elementsByTagName("specVersion");
    if (domNodeList.length() == 1) {
        QString major = domNodeList.item(0).toElement().elementsByTagName("major").item(0).toElement().text();
        QString minor = domNodeList.item(0).toElement().elementsByTagName("minor").item(0).toElement().text();
        if (major.isEmpty() || minor.isEmpty()) {
            return ServiceSales::WRONG_MAJOR_MINOR_VERSION_ERROR;
        }
        this->attributes["major"] = major;
        this->attributes["minor"] = minor;
    }


    domNodeList = element.elementsByTagName("actionList");

    QDomNode node;
    QDomNodeList domNodeList1, domNodeList2, domNodeList3;
    QString nodeName, nodeValue;

    domNodeList = domNodeList.item(0).childNodes();
    for (unsigned int i = 0; i < domNodeList.length(); i++) {
        //qDebug() << domNodeList.item(i).nodeName(); // action action action...
        ActionSales *action = new ActionSales();
        domNodeList1 = domNodeList.item(i).childNodes();
        for (int j = 0; j < domNodeList1.length(); j++) {
            //qDebug() << domNodeList1.item(j).nodeName(); // name or argumentList
            node = domNodeList1.item(j);
            nodeName = node.nodeName();
            if (nodeName == "argumentList") {
                domNodeList2 = node.childNodes();
                for (int k = 0; k < domNodeList2.length(); k++) {
                    //qDebug() << domNodeList2.item(k).nodeName(); //arguments arguments arguments...
                    domNodeList3 = domNodeList2.item(k).childNodes();
                    ActionArgument *actionArgument = new ActionArgument();
                    for (int z = 0; z < domNodeList3.length(); z++) {
                        //qDebug() << domNodeList3.item(z).nodeName(); // name direction relatedStateVariable
                        node = domNodeList3.item(z);
                        nodeName = node.nodeName();
                        nodeValue = node.childNodes().item(0).nodeValue();
                        //qDebug() << nodeName << nodeValue;
                        actionArgument->setAttribute(nodeName, nodeValue);
                    }
                    action->addArgument(actionArgument);
                }
            } else {
                nodeValue = node.childNodes().item(0).nodeValue();
                action->setAttribute(nodeName.toLower(), nodeValue);
            }
        }
        this->actionList.insert(action->getAttribute("name"), action);
    }

    domNodeList = (element.elementsByTagName("serviceStateTable")).item(0).childNodes();
    for (int i = 0; i < domNodeList.length(); i++) {
        StateVariableSales *stateVariable = new StateVariableSales();
        bool sendEvent = (domNodeList.item(i).attributes().item(0).nodeValue() == "yes");
        stateVariable->setSendEvent(sendEvent);
        domNodeList1 = domNodeList.item(i).childNodes();
        for (int j = 0; j < domNodeList1.length(); j++) {
            node = domNodeList1.item(j);
            nodeName = node.nodeName();
            if (nodeName == "allowedValueList") {
                domNodeList1 = node.childNodes();
                for (int k = 0; k < domNodeList1.length(); k++) {
                    stateVariable->addAllowedValueList(domNodeList1.item(k).childNodes().item(0).nodeValue());
                }
            } else {
                nodeValue = node.childNodes().item(0).nodeValue();
                stateVariable->setAttribute(nodeName, nodeValue);
            }
        }
        this->stateVariableList.insert(stateVariable->getAttribute("name"), stateVariable);
    }

    return true;
}

QString ServiceSales::executeAction(const QString &actionName, const QHash<QString, QString> &arguments) {
    ActionSales *action = this->actionList.value(actionName);
    if (action) {
        QHash<QString, ActionArgument*> args = action->getArguments();
        foreach (QString argument, arguments.keys()) {
            if (!args.contains(argument)) {
                return "Invalid argument " + argument + " to call action " + actionName + " of service " + this->getAttribute("serviceId");
            }
        }

        QString ns = this->getAttribute("serviceType");
        QUrl urlPath(this->getAttribute("controlUrl"));
        QUrl url = this->baseUrl.resolved(urlPath);

//        qDebug() << "NS: " << ns;
//        qDebug() << "SOAPAction: " << action->getAttribute("name");
//        qDebug() << "URL: " << url.toString();
        this->soapHttp->setUrl(url);
        this->soapHttp->setNameSpace(ns);
        this->soapHttp->executeAction(actionName, arguments);
        return "";
    }
    return "Action " + actionName + " not found in service " + this->getAttribute("serviceId");
}

void ServiceSales::handleResponseReady() {
    QHash<QString, QString> *result = this->soapHttp->getResponse();
    /*if (message.isFault()) {
        //emit requestError("Error: " + message.faultString().toString(), lastMethod);
        emit executionActionReply(message.faultString().toString(), response, this);
        return;
    }*/

    QHash<QString, ActionArgument *> response;
    QString actionName = this->soapHttp->getActionName();
    ActionSales *action = this->actionList.value(actionName);
    foreach (ActionArgument *actionArgument, action->getArguments().values()) {
        if (actionArgument->getAttribute("direction") == "out") {
            QString name = actionArgument->getAttribute("name");
            actionArgument->setAttribute("value", result->value(name));
            response.insert(actionArgument->getAttribute("name"), actionArgument);
        }
    }
    emit executionActionReply(actionName, response, this);
}

void ServiceSales::handleRequestError(QNetworkReply::NetworkError code, const QString &errorMsg) {
    qDebug() << "It occurred an error while executing request. Error(" << code << ") " << errorMsg;
}

}
}
}
