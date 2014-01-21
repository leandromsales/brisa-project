#include "brisaservicexmlhandler.h"
#include "brisaabstractservice.h"

namespace brisa {
namespace upnp {

BrisaServiceXMLHandler::BrisaServiceXMLHandler(QObject *parent) : QObject(parent) { }

BrisaServiceXMLHandler::~BrisaServiceXMLHandler() { }

void BrisaServiceXMLHandler::parseService(BrisaAbstractService *service,
        QIODevice *scpd) {

    QDomDocument doc("service");
    doc.setContent(scpd);
    QDomElement element = doc.documentElement();
    if (element.tagName() != "scpd") {
        qDebug() << "WARNING:BrisaServiceXMLHandler::parseService: tag name different from scpd. Its name is " <<  element.tagName();
        return;
    }

    QDomNode n = element.firstChild();
    while (!n.isNull()) {
        element = n.toElement();
        if (element.tagName() == "specVersion") {
            QString major = element.elementsByTagName("major").at(0).toElement().text();
            QString minor = element.elementsByTagName("minor").at(0).toElement().text();
            service->setAttribute(BrisaAbstractService::Major, major);
            service->setAttribute(BrisaAbstractService::Minor, minor);
        } else if (element.tagName() == "actionList") {
            QDomNodeList actionList = element.elementsByTagName("action");
            for (int i = 0; i < actionList.size(); i++) {
                QString name = actionList.at(i).toElement().elementsByTagName("name").at(0).toElement().text();
                BrisaAction *action;
                if (service->getAction(name)) {
                    action = service->getAction(name);
                } else {
                    action = new BrisaAction();
                    action->setName(name);
                }
                QDomNode argumentList = actionList.at(i).toElement().elementsByTagName("argumentList").at(0);
                QDomNodeList arguments = argumentList.toElement().elementsByTagName("argument");
                for (int i = 0; i < arguments.size(); i++) {
                    QString argumentName = arguments.at(i).toElement().elementsByTagName("name").at(0).toElement().text();
                    QString direction = arguments.at(i).toElement().elementsByTagName("direction").at(0).toElement().text();
                    QString relatedStateVariable = arguments.at(i).toElement().elementsByTagName("relatedStateVariable").at(0).toElement().text();
                    action->addArgument(argumentName, direction, relatedStateVariable);
                }
                service->addAction(action);
            }
        } else if (element.tagName() == "serviceStateTable") {
            QDomNodeList stateVariables = element.elementsByTagName("stateVariable");
            for (int i = 0; i < stateVariables.size(); i++) {
                BrisaStateVariable *stateVariable = new BrisaStateVariable();
                QString sendEvents = stateVariables.at(i).toElement().attribute("sendEvents");
                QString multicast = stateVariables.at(i).toElement().attribute("multicast");
                QString name = stateVariables.at(i).toElement().elementsByTagName("name").at(0).toElement().text();
                QString dataType = stateVariables.at(i).toElement().elementsByTagName("dataType").at(0).toElement().text();
                QString defaultValue = stateVariables.at(i).toElement().elementsByTagName("defaultValue").at(0).toElement().text();
                stateVariable->setAttribute(BrisaStateVariable::SendEvents, sendEvents);
                stateVariable->setAttribute(BrisaStateVariable::Multicast, multicast);
                stateVariable->setAttribute(BrisaStateVariable::Name, name);
                stateVariable->setAttribute(BrisaStateVariable::DataType, dataType);
                stateVariable->setAttribute(BrisaStateVariable::DefaultValue, defaultValue);
                QDomNode allowedValueList = stateVariables.at(i).toElement().elementsByTagName("allowedValueList").at(0);
                QDomNodeList allowedValues = allowedValueList.toElement().elementsByTagName("allowedValue");
                for (int j = 0; j < allowedValues.size(); j++)
                    stateVariable->addAllowedValue(allowedValues.at(j).toElement().text());

                QDomNodeList allowedValueRange = stateVariables.at(i).toElement().elementsByTagName("allowedValueRange");
                if (allowedValueRange.size() > 0) {
                    QString minimum = allowedValueRange.at(0).toElement().elementsByTagName("minimum").at(0).toElement().text();
                    QString maximum = allowedValueRange.at(0).toElement().elementsByTagName("maximum").at(0).toElement().text();
                    if (minimum.isEmpty() || maximum.isEmpty()) {
                        continue;
                    }
                    QString step = allowedValueRange.at(0).toElement().elementsByTagName("step").at(0).toElement().text();
                    stateVariable->setAttribute(BrisaStateVariable::Minimum, minimum);
                    stateVariable->setAttribute(BrisaStateVariable::Maximum, maximum);
                    if (!step.isEmpty())
                        stateVariable->setAttribute(BrisaStateVariable::Step, step);
                }
                service->addStateVariable(stateVariable);
            }
        } else
            return;
        n = n.nextSibling();
    }
}

}
}
