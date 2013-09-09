#include "statevariable.h"

StateVariable::StateVariable(QObject *parent) : QObject(parent) { }

StateVariable::StateVariable(const QString &name,
                             const QString &dataType,
                             bool sendEvent,
                             QObject *parent) : QObject(parent) {
    this->attributes["name"] = name;
    this->attributes["datatype"] = dataType;
    this->sendEvent = sendEvent;
}

StateVariable::StateVariable(const StateVariable &stateVariable) : QObject(stateVariable.parent()) {
    *this = stateVariable;
}

StateVariable &StateVariable::operator=(const StateVariable &stateVariable) {
    if (this != &stateVariable) {
        this->sendEvent = stateVariable.getSendEvent();
        this->attributes = stateVariable.getAttributes();
    }
    return *this;
}

StateVariable::~StateVariable() { }

void StateVariable::setSendEvent(bool sendEvent) {
    this->sendEvent = sendEvent;
}

bool StateVariable::getSendEvent() const {
    return this->sendEvent;
}

bool StateVariable::setAttribute(const QString &attributeName, const QString &attributeValue) {
    this->attributes[attributeName.toLower()] = attributeValue;
    return true; // use this semantic for future validation
}

QString StateVariable::getAttribute(const QString &attributeName) {
    return this->attributes.value(attributeName.toLower());
}

QHash<QString, QString> StateVariable::getAttributes() const {
    return this->attributes;
}

void StateVariable::addAllowedValueList(const QString &allowedValue) {
    this->allowedValueList.append(allowedValue);
}

QList<QString> StateVariable::getAllowedValueList() {
    return this->allowedValueList;
}
