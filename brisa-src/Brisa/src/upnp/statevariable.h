#ifndef STATEVARIABLE_H
#define STATEVARIABLE_H

#include <QString>
#include <QVariant>
#include <QObject>
#include <QtDebug>

namespace brisa {
namespace upnp {


/*!
 * \class brisa::upnp::StateVariable statevariable.h upnp/StateVariable
 *
 * \brief Represents the service's state variables.
 */
class StateVariable: public QObject {
    Q_OBJECT
public:

    /*!
     * Constructs a StateVariable that \param sendEvents with the given \param name,
     * \param datatype, \param defaultValue, \param maximum value, \param minimum value, value
     * \param step, and \param parent.
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
     * Constructs a StateVariable from the given \param variable.
     */
    StateVariable(const StateVariable &);

    /*!
     * Sets this variable equals to \param variable.
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
     * Sets its attribute \param attr to the given \param value.
     */
    void setAttribute(StateVariableAttribute attr, QVariant newValue);

    /*!
     * Returns \param attr value as a QString.
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

    // TO DO: esses sets todos deveriam ser privados, as chamadas externas deveriam
    // acontecer através do setAttribute.
    /*!
     * Set value of \a nextMulticastSeq with \param seq.
     */
    void setMulticastSeq(quint32 *seq);
    /*!
     * Return value of \a nextMulticastSeq.
     */
    quint32 getNextMulticastSeq();
    /*!
     * Return value of \a multicastUsn.
     */
    QString getMulticastUsn();
    /*!
     * Set value of \a multicastUsn with \param usn.
     */
    void setMulticastUsn(QString usn);
    /*!
     * Return value of \a multicastUdn.
     */
    QString getMulticastUdn();
    /*!
     * Set value of \a multicastUdn with \param udn.
     */
    void setMulticastUdn(QString udn);
    /*!
     * Return value of \a multicastSvcid.
     */
    QString getMulticastSvcid();
    /*!
     * Set value of \a multicastSvcid with \param svcid.
     */
    void setMulticastSvcid(QString svcid);

signals:
    void changed(brisa::upnp::StateVariable *);

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
