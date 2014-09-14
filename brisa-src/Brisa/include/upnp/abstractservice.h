#ifndef ABSTRACTSERVICE_H
#define ABSTRACTSERVICE_H

#include <QMap>
#include <QString>
#include <QObject>

#include "upnp/statevariable.h"
#include "upnp/action.h"
#include "shared/soap/qtsoap.h"

#include "shared/webserver/webservice.h"

namespace brisa {
namespace upnp {

enum UPnPErrorCodes {
	UPNP_SUCCESS = 0,
	UPNP_INVALID_ACTION = 401,
	UPNP_INVALID_ARGUMENTS = 402,
	UPNP_ACTION_FAILED = 501,
	UPNP_INVALID_VALUE_ARGUMENT = 600,
	UPNP_ARGUMENT_VALUE_OUT_OF_RANGE = 601,
	UPNP_OPTIONAL_ACTION_NOT_IMPLEMENTED = 602,
	UPNP_OUT_OF_MEMORY = 603,
	UPNP_HUMAN_INTERVENTION_REQUIRED = 604,
	UPNP_STRING_ARGUMENT_TOO_LONG = 605
};



/*!
 * \class brisa::AbstractService abstractservice.h upnp/AbstractService
 *
 * \brief An abstract class for the control point side and device side service
 */
class AbstractService: public brisa::shared::webserver::WebService {
Q_OBJECT

public:

    AbstractService(QObject *parent = 0);
    /*!
     *
     * Constructs an abstract service with given \a serviceType, \a serviceId,
     * \a scpdUrl, \a controlUrl, \a eventSubUrl, \a host and \a parent.
     */
    AbstractService(const QString &serviceType, const QString &serviceId =
            "", const QString &scpdUrl = "", const QString &controlUrl = "",
            const QString &eventSubUrl = "", const QString &host = "",
            QObject *parent = 0);

    /*!
     * \internal
     *
     * Constructs and abstract service from given \a serv.
     */
    AbstractService(AbstractService &service);

    /*!
     * \internal
     *
     * Destroys the and abstract service.
     */
    virtual ~AbstractService();

    typedef enum {
        Major,
        Minor,
        FileAddress,
        ServiceType,
        ServiceId,
        ScpdUrl,
        ControlUrl,
        EventSubUrl,
        Host,
        Port
    } xmlTags;

    /*!
     * \internal
     *
     * Set its attribute \a key to the given \a value.
     */
    void setAttribute(xmlTags key, const QString &value);

    /*!
     * \internal
     *
     * Get the attribute \a key 's value.
     */
    QString getAttribute(xmlTags key);

    /*!
     * \internal
     *
     * Creates an Action with the given \a name and adds it to the service's
     * action list.
     */
    void addAction(const QString &name);

    /*!
     * \internal
     *
     * Adds the given \a action to the service's action list.
     */
    void addAction(Action *action);

    /*!
     * \internal
     *
     * Returns the service's action with the given \a name. If it has no action
     * with that name, it returns 0.
     */
    Action *getAction(const QString &name);

    /*!
     * \internal
     *
     * Returns the service's action list.
     */
    QList<Action *> getActionList();

    /*!
     * Clears this Actions's list.
     */
    void clearActionList();

    /*!
     * \internal
     *
     * Adds the given \a stateVariable to the service's state variables list.
     */
    void addStateVariable(StateVariable *stateVariable);

    /*!
     * \internal
     *
     * Creates a StateVariable that \a sendEvents with the given \a name,
     * \a datatype, \a defaultValue, \a maximum value, \a minimum value and \a step,
     * and adds it to the service's state variables list.
     */
    void
            addStateVariable(const QString &sendEvents, const QString &name,
                    const QString &datatype, const QString &defaultValue,
                    const QString &maximum, const QString &minimum,
                    const QString &step);

    /*!
     * \internal
     *
     * Returns the service's state variables list.
     */
    const QList<StateVariable *> getStateVariableList();

    /*!
     * Clears this State variable's list.
     */
    void clearStateVariableList();

    /*!
     * \internal
     *
     * Set the service's attributes to their default values.
     */
    void clear();


    /*!
     * \internal
     *
     * Return string literal for a given upnp error code.
     */
	QString errorCodeToString(int errorCode);

    void setUdn(QString udn);

signals:
    void requestFinished(OutArgument arguments, QString methodName);

    void requestError(QString errorMessage, QString methodName);

protected:
    QList<Action *> actionList;
    QList<StateVariable *> stateVariableList;

    QString controlUrl;
    QString eventSubUrl;
    QString fileAddress;
    QString major;
    QString minor;
    QString scpdUrl;
    QString serviceType;
    QString serviceId;

    QString host;
    int port;

    QtSoapHttpTransport* http;

    /*!
     * SEQ number used on multicast event messages.
     */
    quint32 *nextMulticastSeq;

    quint32 initialSeq;

    QString udn;
};

}
}

#endif /* _ABSTRACTSERVICE_H */
