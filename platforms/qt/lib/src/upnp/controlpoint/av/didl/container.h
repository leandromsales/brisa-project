#ifndef CONTAINER_H
#define CONTAINER_H

#include <QObject>
#include <QHash>

#include "item.h"

namespace brisa {
namespace upnp {
namespace controlpoint {
namespace av {
namespace didl {

class Container : public QObject
{
    Q_OBJECT
public:
    explicit Container(QObject *parent = 0);
    explicit Container(const Container &);
    ~Container();
    Container & operator=(const Container &);

    /*!
     * Add an attribute called \param attributeName with value \param attibruteValue
     * to QHash \a attributes.
     */
    bool setAttribute(const QString &attributeName, const QString &attributeValue);
    /*!
     * Return value of attribute \param attributeName.
     */
    QString getAttribute(const QString &attributeName);
    /*!
     * Return all attributtes as a QHash.
     */
    QHash<QString, QString> getAttributes() const;

    /*!
     * Add a property to item. This property will be save on QHash.
     * If arguments are empty strings, function returns false. Otherwise,
     * returns true.
     */
    bool addProperty(const QString &, const QString &);
    /*!
     * Return property with name of argument.
     */
    QString getProperty(const QString &name);
    /*!
     * Return QHash contains all properties.
     */
    QHash<QString, QString> getProperties() const;

    /*!
     * If \param container isn't a null pointer, add it to item,
     * using a QList, and return true. Otherwise, return false.
     */
    bool addContainer(Container *container);
    /*!
     * Return all containers in QList.
     */
    QList<Container*> getContainers() const;

    /*!
     * If \param item isn't a null pointer, add it to QList, and return true.
     * Otherwise, return false.
     */
    bool addItem(Item *);
    /*!
     * Return all items.
     */
    QList<Item*> getItems() const;

private:
    QHash<QString, QString> attributes;
    QHash<QString, QString> properties;
    QList<Container*> containers;
    QList<Item*> items;

signals:

public slots:

};

}  // namespace didl
}  // namespace av
}
}
}

#endif // CONTAINER_H
