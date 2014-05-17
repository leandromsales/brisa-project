#include "actionsales.h"

namespace brisa {
namespace upnp {
namespace controlpoint {

ActionSales::ActionSales(QObject *parent) : QObject(parent) { }

ActionSales::ActionSales(const QString &name, QObject *parent): QObject(parent) {
    this->setAttribute("name", name);
}

ActionSales::ActionSales(const ActionSales &action) : QObject(action.parent()) {
    *this = action;
}

ActionSales &ActionSales::operator=(const ActionSales &action) {
    if (this != &action) {
        this->attributes = action.getAttributes();
        this->argumentList = action.getArguments();
    }
    return *this;
}

ActionSales::~ActionSales() {
    qDeleteAll(this->argumentList.values());
}

bool ActionSales::setAttribute(const QString &attributeName, const QString &attributeValue) {
    this->attributes[attributeName.toLower()] = attributeValue;
    return true; // use this semantic for future validation
}

QString ActionSales::getAttribute(const QString &attributeName) {
    return this->attributes.value(attributeName.toLower());
}

QHash<QString, QString> ActionSales::getAttributes() const {
    return this->attributes;
}

void ActionSales::setArguments(const QHash<QString, ActionArgument*> &arguments) {
    this->argumentList = arguments;
}

QHash<QString, ActionArgument*> ActionSales::getArguments() const {
    return this->argumentList;
}

void ActionSales::addArgument(ActionArgument *argument) {
    this->argumentList.insert(argument->getAttribute("name"), argument);
}

}
}
}
