#ifndef ACTION_H
#define ACTION_H

#include <QObject>
#include <QHash>

#include "actionargument.h"

namespace brisa {
namespace upnp {
namespace controlpoint {

class Action : public QObject
{
    Q_OBJECT
public:
    explicit Action(QObject *parent = 0);
    explicit Action(const QString &, QObject *parent = 0);
    explicit Action(const Action &);
    ~Action();
    Action & operator=(const Action &);

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

#endif // ACTION_H
