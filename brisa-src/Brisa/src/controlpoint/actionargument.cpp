#include "actionargument.h"

ActionArgument::ActionArgument(QObject *parent) : QObject(parent) { }

ActionArgument::ActionArgument(const QString &name,
                               const QString &direction,
                               const QString &relatedStateVariable,
                               QObject *parent) : QObject(parent) {
    this->setAttribute("name", name);
    this->setAttribute("direction", direction);
    this->setAttribute("relatedstatevariable", relatedStateVariable);
}

ActionArgument::ActionArgument(const ActionArgument &actionArgument) : QObject(actionArgument.parent()) {
    *this = actionArgument;
}

ActionArgument &ActionArgument::operator=(const ActionArgument &actionArgument) {
    if (this != &actionArgument) {
        this->attributes = actionArgument.getAttributes();
    }
    return *this;
}

ActionArgument::~ActionArgument() { }

bool ActionArgument::setAttribute(const QString &attributeName, const QString &attributeValue) {
    this->attributes[attributeName.toLower()] = attributeValue;
    return true; // use this semantic for future validation
}

QString ActionArgument::getAttribute(const QString &attributeName) {
    return this->attributes.value(attributeName.toLower());
}

QHash<QString, QString> ActionArgument::getAttributes() const {
    return this->attributes;
}
