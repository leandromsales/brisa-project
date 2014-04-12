#include "res.h"
#include <QDebug>

namespace brisa {
namespace upnp {
namespace controlpoint {

Res::Res(QObject *parent) : QObject(parent) { }

Res::Res(const Res &res) : QObject(res.parent()) {
    *this = res;
}

Res &Res::operator=(const Res &res) {
    if (this != &res) {
        this->properties = res.getProperties();
    }
    return *this;
}

Res::~Res() { }

void Res::setValue(const QString &value) {
    this->value = value;
}

QString Res::getValue() {
    return this->value;
}

bool Res::addProperty(const QString &name, const QString &value) {
    this->properties.insert(name.toLower(), value);
    return true; // TODO: check value, cannot be empty;
}

QString Res::getProperty(const QString &name) {
    return this->properties.value(name);
}

QHash<QString, QString> Res::getProperties() const {
    return this->properties;
}

}
}
}
