#ifndef ITEM_H
#define ITEM_H

#include <QObject>
#include <QHash>

#include "res.h"

namespace brisa {
namespace upnp {
namespace controlpoint {
namespace av {
namespace didl {

class Item : public QObject
{
    Q_OBJECT
public:
    explicit Item(QObject *parent = 0);
    explicit Item(const Item &);
    ~Item();
    Item & operator=(const Item &);

    /*!
     * Put value of \param res in variable \a resource.
     */
    void setRes(Res *res);
    /*!
     * Return content of \a resource.
     */
    Res *getRes();

    /*!
     * Add an attribute and your value in QHash \a attributes.
     */
    bool setAttribute(const QString &, const QString &);
    /*!
     * Return value of \param attributeName.
     */
    QString getAttribute(const QString &attributeName);
    /*!
     * Return a QHash contains all attributes.
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
    QString getProperty(const QString &);
    /*!
     * Return QHash contains all properties.
     */
    QHash<QString, QString> getProperties() const;

private:
    Res *res;
    QHash<QString, QString> attributes;
    QHash<QString, QString> properties;

signals:

public slots:

};

}  // namespace didl
}  // namespace av
}
}
}

#endif // ITEM_H
