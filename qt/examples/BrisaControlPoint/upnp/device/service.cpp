#include <QtDebug>
#include <QUrl>
#include <QMetaMethod>
#include "controlwebservice.h"
#include "../../shared/webserver/webfile.h"
#include "../../shared/webserver/webserversession.h"

// TODO: move the include directive to the begin of the file
#include "service.h"
#include "../servicexmlhandler.h"

#define PRE_ACTION_SIG "preAction(InArgument*const,BrisaAction*const,QString&)"
#define POST_ACTION_SIG "postAction(InArgument*const,OutArgument*const,BrisaAction*const,QString&)"
#define FAILURE_ACTION_SIG "handleActionFailure(InArgument*const,BrisaAction*const,QString&)"
#define ACTION_IN "(InArgument*const,BrisaAction*const)"
#define ACTION_OUT "OutArgument*"
#define PRE_ACTION_OUT "int"
#define POST_ACTION_OUT "int"
#define FAILURE_ACTION_OUT "int"

namespace brisa {
using namespace shared::webserver;
using namespace shared::webserver::http;
namespace upnp {
namespace device {

static const QString SOAP_ERROR_TEMPLATE =
        "<?xml version=\"1.0\"  encoding=\"utf-8\"?>\r\n"
                "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\r\n"
                "<s:Body>\r\n"
                "<s:Fault>\r\n"
                "<faultcode>s:Client</faultcode>\r\n"
                "<faultstring>UPnPError</faultstring>\r\n"
                "<detail>\r\n"
                "<UPnPError xmlns=\"urn:schemas-upnp-org:control-1-0\">\r\n"
                "<errorCode>%1</errorCode>\r\n"
                "<errorDescription>%2</errorDescription>\r\n"
                "</UPnPError>\r\n"
                "</detail>\r\n"
                "</s:Fault>\r\n"
                "</s:Body>\r\n"
                "</s:Envelope>\r\n";
Service::Service(QObject *parent) :
        AbstractService(parent) {
}

Service::Service(const QString &serviceType, const QString &serviceId,
        const QString &scpdUrl, const QString &controlUrl,
        const QString &eventSubUrl, const QString &host, QObject *parent) :
        AbstractService(serviceType, serviceId, scpdUrl, controlUrl,
                eventSubUrl, host, parent) {
}

Service::Service(Service &serv) :
        AbstractService(serv) {
}

Service::~Service() {
    qDeleteAll(this->childWebServices);
    childWebServices.clear();
}

// TODO: Verify commit
void Service::call(const QString &method, InArgument param,
        WebserverSession *session) {
    qDebug() << "Service::call: Calling function " << method << " with args "
            << param;

    Action * action = this->actionRelatedToMethod(method);
    OutArgument *outArguments = new OutArgument();

    if (action) {
        if (action->call(&param, outArguments)) {
            // send response.
            qDebug() << "Service::call: sending response to the requester.";
            this->respondAction(session, outArguments, action->getName());
            //delete outArguments;
        } else {
            qDebug() << "An error has occurred during the " << action->getName()
                                        << " callback.";
            this->respondError(session, UPNP_ACTION_FAILED);
            return;
        }
    } else {
        qDebug() << "Service: Unknown callback: " << method;
        this->respondError(session, UPNP_INVALID_ACTION);
    }

    /*for (QList<Action *>::iterator i = this->actionList.begin();
            i != actionList.end(); ++i) {
        Action *action = *i;
        if (action->getName() == method) {
            int prePostActionReturn = 0;
            QString errorDescription = "";
            // executing preMethod if available
            if (this->preActionMethod.methodIndex() >= 0) {
                if (!this->preActionMethod.invoke(this, Qt::DirectConnection,
                        Q_RETURN_ARG(int, prePostActionReturn),
                        Q_ARG(InArgument *, &param),
                        Q_ARG(Action *, action),
                        Q_ARG(QString, errorDescription))) {
                    qDebug()
                            << "Error invoking preAction method. Continuing...";
                }
            } else {
                qDebug()
                        << "Warning: preAction method not found, continuing...";
            }

            if (prePostActionReturn == 0) {
                // call the action
                OutArgument *outArguments;

                if (!action->call(&param, outArguments)) {
                    qDebug() << "An error has occurred during the "
                            << action->getName() << " callback.";
                    if (!outArguments) {
                        delete outArguments;
                    }

                    if (this->handleActionFailureMethod.methodIndex() >= 0) {
                        int handleFailureActionMethodReturn = 0;
                        if (!this->handleActionFailureMethod.invoke(this,
                                Qt::DirectConnection,
                                Q_RETURN_ARG(int, handleFailureActionMethodReturn),
                                Q_ARG(InArgument *, &param),
                                Q_ARG(Action *, action),
                                Q_ARG(QString, errorDescription))) {
                            qDebug()
                                    << "Error invoking handleActionFailure method. Continuing...";
                            respondError(session, UPNP_ACTION_FAILED);
                            return;
                        }
                        respondError(session, handleFailureActionMethodReturn,
                                "Error specified by UPnP vendor: "
                                        + errorDescription);
                        return;
                    }

                    qDebug()
                            << "handleActionFailure method not implemented in service, returning default error.";
                    this->respondError(session, UPNP_ACTION_FAILED);
                    return;
                }

                // avoiding segmentation fault...
                if (!outArguments) {
                    outArguments = new OutArgument();
                }

                // executing postMethod if available
                if (this->postActionMethod.methodIndex() >= 0) {
                    if (!this->postActionMethod.invoke(this,
                            Qt::DirectConnection,
                            Q_RETURN_ARG(int, prePostActionReturn),
                            Q_ARG(InArgument *, &param),
                            Q_ARG(OutArgument *, outArguments),
                            Q_ARG(Action *, action),
                            Q_ARG(QString, errorDescription))) {
                        qDebug() << "Error invoking postAction method.";
                    }

                    if (prePostActionReturn != 0) {
                        qDebug()
                                << "Warning: postAction service method returned non-zero, sending UPnP error code "
                                << prePostActionReturn;
                        delete outArguments;
                        respondError(session, prePostActionReturn,
                                errorDescription);
                        return;
                    }
                } else {
                    qDebug()
                            << "Warning: postAction method not found, continuing...";
                }

                // send response.
                this->respondAction(session, outArguments, action->getName());
                delete outArguments;
                return;
            } else { // preAction returned non-zero
                qDebug()
                        << "Warning: preAction method returned non-zero value, it returned "
                        << prePostActionReturn << " with description "
                        << errorDescription;
                respondError(session, prePostActionReturn, errorDescription);
                return;
            }
        }
    }

    qDebug() << "BrisaService: Unknown callback: " << method;
    respondError(session, UPNP_INVALID_ACTION);*/
}

void Service::buildWebServiceTree(Webserver *sessionManager) {
    WebService *control = new ControlWebService(serviceType);

    connect(control,
            SIGNAL(requestReceived(QString,InArgument,brisa::shared::webserver::WebserverSession*)),
            this, SLOT(call(QString,InArgument,brisa::shared::webserver::WebserverSession*)));
    connect(control, SIGNAL(invalidRequest(brisa::shared::webserver::WebserverSession*)), this,
            SLOT(onInvalidRequest(brisa::shared::webserver::WebserverSession*)));

    EventController *event = new EventController(sessionManager,
            &stateVariableList, this);

    sessionManager->addService(
            (serviceId + '/' + controlUrl.section('/', -1)).toUtf8(), control);

    sessionManager->addService(
            (serviceId + '/' + eventSubUrl.section('/', -1)).toUtf8(), event);

    sessionManager->addService(
            (serviceId + '/' + scpdUrl.section('/', -1)).toUtf8(),
            new WebFile(scpdFilePath, this));

    childWebServices.insert(controlUrl.section('/', -1), control);
    childWebServices.insert(eventSubUrl.section('/', -1), event);

    parseDescriptionFile();
}

void Service::onInvalidRequest(WebserverSession *session) {
    respondError(session, UPNP_INVALID_ACTION);
}

StateVariable *Service::getVariable(const QString &variableName) {
    for (QList<StateVariable *>::iterator i =
            this->stateVariableList.begin(); i != this->stateVariableList.end();
            ++i) {
        if ((*i)->getAttribute(StateVariable::Name) == variableName)
            return *i;
    }

    return 0;
}

void Service::onRequest(const HttpRequest &request,
        WebserverSession *session) {

    // qDebug() << "REQUEST: " << HttpRequest. << endl;

    if (request.method() != "POST") {
        session->respond(
                HttpResponse(request.httpVersion(), HttpResponse::BAD_REQUEST,
                        true));
    }

    ActionXmlParser actionXmlParser;

    {
        QIODevice *xml = request.entityBody();
        xml->seek(0);
        actionXmlParser.setXmlContent(xml->readAll());
    }

    if (actionXmlParser.parseSOAP()) {
        //If servicetype is incorrect
        if (actionXmlParser.serviceType != serviceType)
            return;

        call(actionXmlParser.method, actionXmlParser.args, session);
    } else {
        qDebug() << "Service: Invalid SOAP xml format.";
        respondError(session, UPNP_INVALID_ACTION);
    }
}

inline void Service::respondAction(WebserverSession *session,
        const OutArgument *outArgs, const QString &actionName) {
    QByteArray message(
            "<?xml version=\"1.0\"  encoding=\"utf-8\"?>\r\n"
                    "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                    "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\r\n"
                    "<s:Body>\r\n"
                    "<u:" + actionName.toUtf8() + "Response xmlns:u=\""
                    + serviceType.toUtf8() + "\">\r\n");

    for (QMap<QString, QString>::const_iterator i = outArgs->begin();
            i != outArgs->end(); ++i) {
        message.append(
                "<" + i.key() + ">" + i.value() + "</" + i.key() + ">\r\n");
    }

    message.append("</u:" + actionName + "Response>\r\n"
            "</s:Body>\r\n"
            "</s:Envelope>\r\n");

    // TODO: if the connection should br closed, set the flag in the HttpResponse constructor
    HttpResponse r(HttpVersion(1, 1), HttpResponse::OK);
    r.setHeader("CONTENT-LENGTH", QByteArray::number(message.size()));
    r.setEntityBody(message);
    session->respond(r);
}

inline void Service::respondError(WebserverSession *session, int errorCode,
        QString errorDescription) {
    if (errorDescription == "") {
        errorDescription = this->errorCodeToString(errorCode);
    }
    QString message = SOAP_ERROR_TEMPLATE.arg(QString::number(errorCode),
            this->errorCodeToString(errorCode));

    // TODO: if the connection should br closed, set the flag in the HttpResponse constructor
    HttpResponse r(HttpVersion(1, 1), HttpResponse::OK);
    r.setHeader("CONTENT-LENGTH", QByteArray::number(message.size()));
    r.setEntityBody(message.toUtf8());
    session->respond(r);
}

void Service::setDescriptionFile(const QString &scpdFilePath) {
    this->scpdFilePath = scpdFilePath;
}

QString Service::getDescriptionFile() {
    return this->scpdFilePath;
}

void Service::parseDescriptionFile() {
    if (this->scpdFilePath.isEmpty()) {
        qDebug()
                << "WARNING::Service::parseDescriptionFile: scpd filePath is empty";
        return;
    }

    // TODO: Change this to only pass the file (this->scdpFilePath) and make the
    // BrisaServiceXMLHandler class open the file when parseService method
    // is called.
    qDebug() << "INFO::Service::parseDescriptionFile: scpd filePath: "
            << this->scpdFilePath;
    QFile file(this->scpdFilePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ServiceXMLHandler *serviceXMLHandler =
                new ServiceXMLHandler();
        serviceXMLHandler->parseService(this, &file);
        delete serviceXMLHandler;

        file.close();

        this->bindActionsToServiceMethods();
        this->connectVariablesEventSignals();
        this->setDefaultValues();
    } else {
        //It should throw and exception
        qDebug()
                << "ERROR:Service::parseDescriptionFile: Could not open the description file.";
    }
}

void Service::connectVariablesEventSignals() {
    EventController *event =
            dynamic_cast<EventController *>(childWebServices.value(
                    eventSubUrl.section('/', -1)));

    if (!event) {
        return;
    }

    foreach (StateVariable *stateVar, this->stateVariableList)
    {
        if (stateVar->sendEventsChange()) {
            QObject::connect(stateVar, SIGNAL(changed(brisa::upnp::StateVariable *)),
                    event, SLOT(variableChanged(brisa::upnp::StateVariable *)));
        }
    }
}

void Service::setDefaultValues() {
    foreach (StateVariable *stateVar, this->stateVariableList)
    {
        stateVar->setAttribute(StateVariable::Value,
                stateVar->getAttribute(StateVariable::DefaultValue));
    }
}

Action * Service::actionRelatedToMethod(QString methodSignature) {
    Action *action = 0;
     qDebug() << "BrisaService::actionByName: Searching for action that fits in: "<< methodSignature;
    for (QList<Action *>::iterator i = this->actionList.begin();
            i != actionList.end(); ++i) {
        action = *i;
        qDebug() << "Current action:" << action->getName();
        if (QString(methodSignature).contains(action->getName(), Qt::CaseInsensitive)) {
            qDebug() << "ACHEI:" << action->getName();
            break;
        }
        action = 0;
    }
    return action;
}

void Service::bindActionsToServiceMethods() {
    const QMetaObject *meta = this->metaObject();
    QMetaMethod method;

    // binding preAction method
    int methodIndex = meta->indexOfMethod(PRE_ACTION_SIG);
    if (methodIndex >= 0) {
        method = meta->method(methodIndex);
        if (strcmp(method.typeName(), PRE_ACTION_OUT) == 0) {
            this->preActionMethod = method;
            qDebug() << "Binding method " << PRE_ACTION_SIG << " of service ID "
                    << this->serviceId;
        }
    }

    // binding postAction method
    methodIndex = meta->indexOfMethod(POST_ACTION_SIG);
    if (methodIndex >= 0) {
        method = meta->method(methodIndex);
        if (strcmp(method.typeName(), POST_ACTION_OUT) == 0) {
            this->postActionMethod = method;
            qDebug() << "Binding method " << POST_ACTION_SIG
                    << " of service ID " << this->serviceId;
        }
    }

    // binding handleActionFailure method
    methodIndex = meta->indexOfMethod(FAILURE_ACTION_SIG);
    if (methodIndex >= 0) {
        method = meta->method(methodIndex);
        if (strcmp(method.typeName(), FAILURE_ACTION_OUT) == 0) {
            this->handleActionFailureMethod = method;
            qDebug() << "Binding method " << FAILURE_ACTION_SIG
                    << " of service ID " << this->serviceId;
        }
    }
    for(int i = meta->methodOffset(); i < meta->methodCount(); ++i) {
        method = meta->method(i);
        qDebug() << "METODO É " << method.name ();
        Action * action = actionRelatedToMethod(method.methodSignature());
        if(action) {
            action->setMethod(method, this);
            qDebug() << "Binding method " << method.methodSignature () << " of service ID " << this->serviceId << " to service action " << action->getName();
        }
    }

    // binding service actions methods
    /*for (QList<BrisaAction *>::iterator i = this->actionList.begin();
            i != actionList.end(); ++i) {
        BrisaAction *action = *i;
        QString actionName = action->getName();
        QString methodName = actionName.toLower();

        //qDebug() << "INFO:BrisaService::bindActionsToServiceMethods: Current binding method " << methodName + ACTION_IN;
        int methodIndex = meta->indexOfMethod(
                qPrintable(methodName + ACTION_IN));

        if (methodIndex < 0) {
            qDebug() << "Error: no method named " << methodName
                    << " was found in the "
                    << "specified service class definition that matches with expected "
                    << "signature " << ACTION_OUT << " " << methodName
                    << ACTION_IN << "." << "It was not possible to bind action "
                    << actionName << " to " << methodName;
            continue;
        }

        QMetaMethod method = meta->method(methodIndex);
        if (strcmp(method.typeName(), ACTION_OUT) == 0) {
            qDebug() << "Binding method " << methodName << " of service ID "
                    << this->serviceId << " to service action " << actionName;
            action->setMethod(method, this);
        } else {
            qDebug() << "Error: method named " << methodName
                    << " has wrong prototype. "
                    << "It is expected a method prototype that matches with the prototype "
                    << "'" << ACTION_OUT << " " << methodName << ACTION_IN
                    << "'. It was " << "found a method with prototype '"
                    << method.typeName() << " " << methodName << ACTION_IN;
        }
    }*/
}

}
}
}
