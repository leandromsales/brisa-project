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

    /*!
     * Set value of \a sendEvent.
     */
    void setSendEvent(bool sendEvent);
    /*!
     * Get value of \a sendEvent.
     */
    bool getSendEvent() const;

    /*!
     * Add an attribute and your value in QHash \a attributes.
     */
    bool setAttribute(const QString &, const QString &);
    /*!
     * Return value of \param attributeName.
     */
    QString getAttribute(const QString &);
    /*!
     * Return a QHash contains all attributes.
     */
    QHash<QString, QString> getAttributes() const;

    /*!
     * Add an allowed value to list \a allowedValueList.
     */
    void addAllowedValueList(const QString &);
    /*!
     * Return a QList contains allowed value list.
     */
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
