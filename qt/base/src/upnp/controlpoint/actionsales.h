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

    /*!
     * Add an attribute and your value in QHash \a attributes.
     */
    bool setAttribute(const QString &attributeName, const QString &attributeValue);
    /*!
     * Return value of \param attributeName.
     */
    QString getAttribute(const QString &attributeName);
    /*!
     * Return a QHash contains all attributes.
     */
    QHash<QString, QString> getAttributes() const;

    /*!
     * Set QHash \a argumentList with value of \param arguments.
     */
    void setArguments(const QHash<QString, ActionArgument*> &arguments);
    /*!
     * Return a QHash contains all arguments.
     */
    QHash<QString, ActionArgument*> getArguments() const;
    /*!
     * Insert an argument in argument list.
     */
    void addArgument(ActionArgument *argument);

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
