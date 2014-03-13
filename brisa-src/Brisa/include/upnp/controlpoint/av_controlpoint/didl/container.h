#ifndef CONTAINER_H
#define CONTAINER_H

#include <QObject>
#include <QHash>

#include "item.h"

namespace brisa {
namespace upnp {
namespace controlpoint {

class Container : public QObject
{
    Q_OBJECT
public:
    explicit Container(QObject *parent = 0);
    explicit Container(const Container &);
    ~Container();
    Container & operator=(const Container &);

    bool setAttribute(const QString &, const QString &);
    QString getAttribute(const QString &);
    QHash<QString, QString> getAttributes() const;

    bool addProperty(const QString &, const QString &);
    QString getProperty(const QString &);
    QHash<QString, QString> getProperties() const;

    bool addContainer(Container *);
    QList<Container*> getContainers() const;

    bool addItem(Item *);
    QList<Item*> getItems() const;

private:
    QHash<QString, QString> attributes;
    QHash<QString, QString> properties;
    QList<Container*> containers;
    QList<Item*> items;

signals:

public slots:

};

}
}
}

#endif // CONTAINER_H
