#include "action.h"

namespace brisa {
namespace upnp {
namespace controlpoint {

Action::Action(QObject *parent) : QObject(parent) { }

Action::Action(const QString &name, QObject *parent): QObject(parent) {
    this->setAttribute("name", name);
}

Action::Action(const Action &action) : QObject(action.parent()) {
    *this = action;
}

Action &Action::operator=(const Action &action) {
    if (this != &action) {
        this->attributes = action.getAttributes();
        this->argumentList = action.getArguments();
    }
    return *this;
}

Action::~Action() {
    qDeleteAll(this->argumentList.values());
}

bool Action::setAttribute(const QString &attributeName, const QString &attributeValue) {
    this->attributes[attributeName.toLower()] = attributeValue;
    return true; // use this semantic for future validation
}

QString Action::getAttribute(const QString &attributeName) {
    return this->attributes.value(attributeName.toLower());
}

QHash<QString, QString> Action::getAttributes() const {
    return this->attributes;
}

void Action::setArguments(const QHash<QString, ActionArgument*> &arguments) {
    this->argumentList = arguments;
}

QHash<QString, ActionArgument*> Action::getArguments() const {
    return this->argumentList;
}

void Action::addArgument(ActionArgument *argument) {
    this->argumentList.insert(argument->getAttribute("name"), argument);
}

}
}
}
