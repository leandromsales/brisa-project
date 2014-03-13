#ifndef STATEVARIABLE_H
#define STATEVARIABLE_H

#include <QObject>
#include <QHash>
#include <QList>

namespace brisa {
namespace upnp {
namespace controlpoint {

class StateVariable : public QObject
{
    Q_OBJECT
public:
    explicit StateVariable(QObject *parent = 0);
    explicit StateVariable(const QString &,
                           const QString &,
                           bool,
                           QObject *parent = 0);
    explicit StateVariable(const StateVariable &);
    ~StateVariable();
    StateVariable & operator=(const StateVariable &);

    void setSendEvent(bool sendEvent);
    bool getSendEvent() const;

    bool setAttribute(const QString &, const QString &);
    QString getAttribute(const QString &);
    QHash<QString, QString> getAttributes() const;

    void addAllowedValueList(const QString &);
    QList<QString> getAllowedValueList();

private:
    bool sendEvent;

    QHash<QString, QString> attributes;
    QList<QString> allowedValueList;

signals:

public slots:

};

}
}
}

#endif // STATEVARIABLE_H
