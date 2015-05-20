#include "container.h"

#include <QDebug>

namespace brisa {
namespace upnp {
namespace controlpoint {
namespace av {
namespace didl {

Container::Container(QObject *parent) :
    QObject(parent) { }

Container::Container(const Container &Container) : QObject(Container.parent()) {
    *this = Container;
}

Container &Container::operator=(const Container &container) {
    if (this != &container) {
        this->attributes = container.getAttributes();
        this->properties = container.getProperties();
        this->containers = container.getContainers();
        this->items = container.getItems();
    }
    return *this;
}

Container::~Container() {
    qDeleteAll(this->containers);
    qDeleteAll(this->items);
}

bool Container::setAttribute(const QString &attributeName, const QString &attributeValue) {
    this->attributes[attributeName.toLower()] = attributeValue;
    return true; // use this semantic for future validation
}

QString Container::getAttribute(const QString &attributeName) {
    return this->attributes.value(attributeName.toLower());
}

QHash<QString, QString> Container::getAttributes() const {
    return this->attributes;
}

bool Container::addProperty(const QString &name, const QString &value) {
    bool notEmpty = true;
    if (name.isEmpty() || value.isEmpty()) {
        notEmpty = false;
    } else {
        this->properties.insert(name.toLower(), value);
    }
    return notEmpty;
}

QString Container::getProperty(const QString &name) {
    return this->properties.value(name.toLower());
}

QHash<QString, QString> Container::getProperties() const {
    return this->properties;
}

bool Container::addContainer(Container *container) {
    bool notEmpty = true;
    if (!container) {
        this->containers.append(container);
    } else {
        notEmpty = false;
    }
    return notEmpty;
}

QList<Container*> Container::getContainers() const {
    return this->containers;
}

bool Container::addItem(Item *item) {
    bool notEmpty = true;
    if (!item) {
        this->items.append(item);
    } else {
        notEmpty = false;
    }
    return notEmpty;
}

QList<Item*> Container::getItems() const {
    return this->items;
}

}  // namespace didl
}  // namespace av
}
}
}
