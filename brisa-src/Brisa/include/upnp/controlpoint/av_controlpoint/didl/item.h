#ifndef ITEM_H
#define ITEM_H

#include <QObject>
#include <QHash>

#include "res.h"

namespace brisa {
namespace upnp {
namespace controlpoint {

class Item : public QObject
{
    Q_OBJECT
public:
    explicit Item(QObject *parent = 0);
    explicit Item(const Item &);
    ~Item();
    Item & operator=(const Item &);

    void setRes(Res *);
    Res *getRes();

    bool setAttribute(const QString &, const QString &);
    QString getAttribute(const QString &);
    QHash<QString, QString> getAttributes() const;

    bool addProperty(const QString &, const QString &);
    QString getProperty(const QString &);
    QHash<QString, QString> getProperties() const;

private:
    Res *res;
    QHash<QString, QString> attributes;
    QHash<QString, QString> properties;

signals:

public slots:

};

}
}
}

#endif // ITEM_H
