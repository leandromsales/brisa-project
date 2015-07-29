#ifndef ACTIONARGUMENT_H
#define ACTIONARGUMENT_H

#include <QObject>
#include <QHash>

#include "statevariablesales.h"

namespace brisa {
namespace upnp {
namespace controlpoint {

class ActionArgument : public QObject
{
    Q_OBJECT
public:
    explicit ActionArgument(QObject *parent = 0);
    explicit ActionArgument(const QString &,
                            const QString &,
                            const QString &,
                            QObject *parent = 0);
    explicit ActionArgument(const ActionArgument &);
    ~ActionArgument();
    ActionArgument & operator=(const ActionArgument &);

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

private:
    QHash<QString, QString> attributes;

signals:

public slots:

};

}
}
}

#endif // ACTIONARGUMENT_H
