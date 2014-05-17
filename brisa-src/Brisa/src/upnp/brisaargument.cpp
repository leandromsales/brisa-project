#include "brisaargument.h"


namespace brisa {
namespace upnp {


Argument::Argument(const Argument &brisaArgument):
    QObject(),
    name(brisaArgument.name),
    direction(brisaArgument.direction),
    relatedStateVariable(brisaArgument.relatedStateVariable) {
}

Argument::Argument(const QString &name,
                             const QString &direction,
                             const QString &relatedStateVariable,
                             QObject *parent) :
    QObject(parent),
    name(name),
    direction(direction),
    relatedStateVariable(relatedStateVariable) {
}

void Argument::setAttribute(xmlArgument key, const QString &value) {
    switch (key) {
    case ArgumentName:
        this->name = value;
        break;
    case Direction:
        this->direction = value;
        break;
    case RelatedStateVariable:
        this->relatedStateVariable = value;
        break;
    }
}

QString Argument::getAttribute(xmlArgument key) const {
    switch (key) {
    case ArgumentName:
        return this->name;
        break;
    case Direction:
        return this->direction;
        break;
    case RelatedStateVariable:
        return this->relatedStateVariable;
        break;
    default:
        return QString();
        break;
    }
}

void Argument::clear() {
    this->name.clear();
    this->direction.clear();
    this->relatedStateVariable.clear();
}

}
}

