#ifndef STATEVARIABLESALES_H
#define STATEVARIABLESALES_H

#include <QObject>
#include <QHash>
#include <QList>

namespace brisa {
namespace upnp {
namespace controlpoint {

class StateVariableSales : public QObject
{
    Q_OBJECT
public:
    explicit StateVariableSales(QObject *parent = 0);
    explicit StateVariableSales(const QString &,
                           const QString &,
                           bool,
                           QObject *parent = 0);
    explicit StateVariableSales(const StateVariableSales &);
    ~StateVariableSales();
    StateVariableSales & operator=(const StateVariableSales &);

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

#endif // STATEVARIABLESALES_H
