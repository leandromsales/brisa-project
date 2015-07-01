#include "item.h"
#include <QDebug>

namespace brisa {
namespace upnp {
namespace controlpoint {
namespace av {
namespace didl {

Item::Item(QObject *parent) :
    QObject(parent) { }

Item::Item(const Item &Item) : QObject(Item.parent()) {
    *this = Item;
}

Item &Item::operator=(const Item &item) {
    if (this != &item) {
        this->attributes = item.getAttributes();
        this->properties = item.getProperties();
    }
    return *this;
}

Item::~Item() {
    delete this->res;
}

void Item::setRes(Res *res) {
    this->res = res;
}

Res *Item::getRes() {
    return this->res;
}

bool Item::setAttribute(const QString &attributeName, const QString &attributeValue) {
    this->attributes[attributeName.toLower()] = attributeValue;
    return true; // use this semantic for future validation
}

QString Item::getAttribute(const QString &attributeName) {
    return this->attributes.value(attributeName.toLower());
}

QHash<QString, QString> Item::getAttributes() const {
    return this->attributes;
}

bool Item::addProperty(const QString &name, const QString &value) {
    bool notEmpty = true;
    if (name.isEmpty() || value.isEmpty()) {
        notEmpty = false;
    } else {
        this->properties.insert(name.toLower(), value);
    }
    return notEmpty;
}

QString Item::getProperty(const QString &name) {
    return this->properties.value(name);
}

QHash<QString, QString> Item::getProperties() const {
    return this->properties;
}

}  // namespace didl
}  // namespace av
}
}
}
