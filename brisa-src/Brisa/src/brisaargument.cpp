#include "brisaargument.h"

using namespace Brisa;

BrisaArgument::BrisaArgument(const BrisaArgument &brisaArgument):
    QObject(),
    name(brisaArgument.name),
    direction(brisaArgument.direction),
    relatedStateVariable(brisaArgument.relatedStateVariable) {
}

BrisaArgument::BrisaArgument(const QString &name,
                             const QString &direction,
                             const QString &relatedStateVariable,
                             QObject *parent) :
    QObject(parent),
    name(name),
    direction(direction),
    relatedStateVariable(relatedStateVariable) {
}

void BrisaArgument::setAttribute(xmlArgument key, const QString &value) {
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

QString BrisaArgument::getAttribute(xmlArgument key) const {
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

void BrisaArgument::clear() {
    this->name.clear();
    this->direction.clear();
    this->relatedStateVariable.clear();
}

