#include "statevariablesales.h"

namespace brisa {
namespace upnp {
namespace controlpoint {

StateVariableSales::StateVariableSales(QObject *parent) : QObject(parent) { }

StateVariableSales::StateVariableSales(const QString &name,
                             const QString &dataType,
                             bool sendEvent,
                             QObject *parent) : QObject(parent) {
    this->attributes["name"] = name;
    this->attributes["datatype"] = dataType;
    this->sendEvent = sendEvent;
}

StateVariableSales::StateVariableSales(const StateVariableSales &stateVariable) : QObject(stateVariable.parent()) {
    *this = stateVariable;
}

StateVariableSales &StateVariableSales::operator=(const StateVariableSales &stateVariable) {
    if (this != &stateVariable) {
        this->sendEvent = stateVariable.getSendEvent();
        this->attributes = stateVariable.getAttributes();
    }
    return *this;
}

StateVariableSales::~StateVariableSales() { }

void StateVariableSales::setSendEvent(bool sendEvent) {
    this->sendEvent = sendEvent;
}

bool StateVariableSales::getSendEvent() const {
    return this->sendEvent;
}

bool StateVariableSales::setAttribute(const QString &attributeName, const QString &attributeValue) {
    this->attributes[attributeName.toLower()] = attributeValue;
    return true; // use this semantic for future validation
}

QString StateVariableSales::getAttribute(const QString &attributeName) {
    return this->attributes.value(attributeName.toLower());
}

QHash<QString, QString> StateVariableSales::getAttributes() const {
    return this->attributes;
}

void StateVariableSales::addAllowedValueList(const QString &allowedValue) {
    this->allowedValueList.append(allowedValue);
}

QList<QString> StateVariableSales::getAllowedValueList() {
    return this->allowedValueList;
}

}
}
}
