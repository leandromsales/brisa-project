#include "container.h"

#include <QDebug>

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
    this->properties.insert(name.toLower(), value);
    return true; // TODO: check value, cannot be empty;
}

QString Container::getProperty(const QString &name) {
    return this->properties.value(name.toLower());
}

QHash<QString, QString> Container::getProperties() const {
    return this->properties;
}

bool Container::addContainer(Container *container) {
    this->containers.append(container);
    return true; // TODO: check value, cannot be empty;
}

QList<Container*> Container::getContainers() const {
    return this->containers;
}

bool Container::addItem(Item *item) {
    this->items.append(item);
    return true; // TODO: check value, cannot be empty;
}

QList<Item*> Container::getItems() const {
    return this->items;
}
