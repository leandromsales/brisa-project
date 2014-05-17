#ifndef ACTIONSALES_H
#define ACTIONSALES_H

#include <QObject>
#include <QHash>

#include "actionargument.h"

namespace brisa {
namespace upnp {
namespace controlpoint {

class ActionSales : public QObject
{
    Q_OBJECT
public:
    explicit ActionSales(QObject *parent = 0);
    explicit ActionSales(const QString &, QObject *parent = 0);
    explicit ActionSales(const ActionSales &);
    ~ActionSales();
    ActionSales & operator=(const ActionSales &);

    bool setAttribute(const QString &, const QString &);
    QString getAttribute(const QString &);
    QHash<QString, QString> getAttributes() const;

    void setArguments(const QHash<QString, ActionArgument*> &);
    QHash<QString, ActionArgument*> getArguments() const;
    void addArgument(ActionArgument *);

private:
    QHash<QString, QString> attributes;
    QHash<QString, ActionArgument*> argumentList;

signals:

public slots:

};

}
}
}

#endif // ACTIONSALES_H
