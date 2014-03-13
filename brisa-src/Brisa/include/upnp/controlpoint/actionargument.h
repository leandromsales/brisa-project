#ifndef ACTIONARGUMENT_H
#define ACTIONARGUMENT_H

#include <QObject>
#include <QHash>

#include "statevariable.h"

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

    bool setAttribute(const QString &, const QString &);
    QString getAttribute(const QString &);
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
