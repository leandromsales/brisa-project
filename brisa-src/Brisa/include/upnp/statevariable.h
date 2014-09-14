#ifndef STATEVARIABLE_H
#define STATEVARIABLE_H

#include <QString>
#include <QVariant>
#include <QObject>
#include <QtDebug>

namespace brisa {
namespace upnp {


/*!
 * \class brisa::StateVariable statevariable.h upnp/StateVariable
 *
 * \brief Represents the service's state variables.
 */
class StateVariable: public QObject {
    Q_OBJECT
public:

    /*!
     * Constructs a StateVariable that \a sendEvents with the given \a name,
     * \a datatype, \a defaultValue, \a maximum value, \a minimum value, value
     * \a step, and \a parent.
     */
    StateVariable(QString sendEvents = "",
                       QString name = "",
                       QString datatype = "",
                       QString defaultValue = "",
                       QString maximum = "",
                       QString minimum = "",
                       QString step = "",
                       QString multicast = "",
                       QObject *parent = 0);

    /*!
     * Constructs a StateVariable from the given \a variable.
     */
    StateVariable(const StateVariable &);

    /*!
     * Sets this variable equals to \a variable.
     */
    StateVariable &operator=(const StateVariable &);

    typedef enum {
        Name,
        name,
        SendEvents,
        sendEvents,
        DataType,
        dataType,
        DefaultValue,
        defaultValue,
        AllowedValue,
        allowedValue,
        Minimum,
        minimum,
        Maximum,
        maximum,
        Step,
        step,
        Value,
        value,
        Multicast,
        multicast
    } StateVariableAttribute;

    /*!
     * Sets its attribute \a attr to the given \a value.
     */
    void setAttribute(StateVariableAttribute attr, QVariant newValue);

    /*!
     * Returns \a attr value as a QString.
     */
    QString getAttribute(StateVariableAttribute attr, int index = 0) const;

    /*!
     * Returns the stored value as a QVariant.
     */
    QVariant getValue() const;

    /*!
     *  \internal
     *  Returns the stored type as a valid UPnP type represented as a QVariant::Type.
     */
    QVariant::Type getDataType() const;

    /*!
     * Returns true if the variable is set to send events.
     */
    bool sendEventsChange() const;

    /*!
     * Adds a value to the list of values that can be set to its Value attribute.
     */
    void addAllowedValue(QString allowedVal);

    /*!
     * Returns the list of values that can be set to its Value attribute.
     */
    QList<QString> getAllowedValueList();

    bool multicastEvents() const;

    /*!
     * Clears this variable's attributtes.
     */
    void clear();

    void setMulticastSeq(quint32 *seq);

    quint32 getNextMulticastSeq();

    QString getMulticastUsn();

    void setMulticastUsn(QString usn);

    QString getMulticastUdn();

    void setMulticastUdn(QString udn);

    QString getMulticastSvcid();

    void setMulticastSvcid(QString svcid);

signals:
    void changed(StateVariable *);

private:

    /*!
     * \internal
     * Returns true if the given value is valid.
     */
    bool validateNewValue(const QVariant &value);

    QList<QString> allowedValueList;

    bool _sendEvents;
    bool _multicast;
    QString _name;
    QString _defaultValue;
    QString _maximum;
    QString _minimum;
    QString _step;
    QString _dataType;
    QVariant _value;

    quint32 *nextMulticastSeq;
    QString multicastUsn;
    QString multicastUdn;
    QString multicastSvcid;

};

}
}

#endif /* _STATEVARIABLE_H */
