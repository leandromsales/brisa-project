#ifndef ACTION_H
#define ACTION_H

#include "brisaargument.h"

#include <QString>
#include <QList>
#include <QMap>
#include <QObject>
#include <QMetaMethod>

typedef QMap<QString, QString> BrisaInArgument, BrisaOutArgument;

namespace brisa {
namespace upnp {

namespace device {
class Service;
}

class StateVariable;

/*!
 * \class Brisa::BrisaAction brisaaction.h BrisaUpnp/BrisaAction
 *
 * \brief Template method class that represents each service's action.
 *
 * Create a class derived from BrisaAction and reimplement the method run().
 * That method is the one which is called when this action is invoked and
 * defines the action's behavior.
 *
 * The method run() receives a QMap<QString, QString> with the input arguments.
 * They are organized as key -- the argument name -- and value -- the argument
 * value. It has to return a QMap<QString, QString> with the output arguments
 * organized the same way.
 *
 * If any of the returned output arguments is not defined in the service
 * description file, BrisaAction will show an error message at the debug
 * output stream and send an error message to the control point.
 */
class Action: public QObject {
    Q_OBJECT
public:
    /*!
     * Constructs an action with given \a name, \a parent and \a service that it is related to.
     */
    Action(QString name = "", device::Service *service = 0, QObject *parent = 0);

    /*!
     * Constructs an action with given \a name, \a parent and \a service and a method to be invoked
	 * when the action is executed.
     */
	Action(QString name, device::Service *service, const QMetaMethod &method, QObject *parent);

    /*!
     * Constructs a new action based on \a action.
     */
    Action(const Action &action);

    /*!
     * Destroys the action. It has to be overridden for properly destroying the
     * derived actions when necessary.
     */
    virtual ~Action();

    /*!
     * Sets a new name to the action.
     */
    void setName(QString name);
    /*!
     * Returns the action's name.
     */
    QString getName() const;

    /*!
     * Sets a new service for this action.
     */
    void setService(device::Service *service);
    /*!
     * Returns the service that this action is related to.
     */
    device::Service* getService() const;

    /*!
     * Returns the related service's state variable with the given \a name. If it cannot find its
     * related service or the state variable, then it returns a null pointer.
     */
    StateVariable* getStateVariable(const QString &name) const;

    /*!
     * Returns this action's list of arguments.
     */
    QList<Argument*> getArgumentList() const;

    /*!
     * Adds an argument with given \a name, \a direction and \a relatedStateVariable
     * to this action's list of arguments.
     */
    void addArgument(QString name = "", QString direction = "",
            QString relatedStateVariable = "");
    /*!
     * Adds given \a argument to this action's list of arguments.
     */
    void addArgument(Argument *argumentA);
    /*!
     * Adds given list of \a arguments to this action's list of arguments.
     */
    void addArguments(const QList<Argument*> argumentA);


    /*!
     * Clears this action's argument list.
     */
    void clearArgumentList();

    /*!
     * Remove given \a argument from this action's list of arguments.
     */

    bool removeArgument(Argument* argumentA);

    /*!
     * Validates \a inArguments, \a outArguments and runs the action. \a outArguments
     * is an output parameter. This method returns true in case of successful
     * running of the action, else returns false.
     */
    bool call(/*const */BrisaInArgument *inArguments, BrisaOutArgument *&outArguments);

    /*!
     * Sets the method to be invoked when the action is executed.
     */
    void setMethod(const QMetaMethod &method, device::Service *service);

    /*!
     * Returns the method to be invoked when the action is executed.
     */
    QMetaMethod getMethod() const;

private:

    /*!
     * Returns the related input argument with the given \a name.
     */
    Argument *getInArgument(const QString &name);

    /*!
     * Returns the related output argument with the given \a name.
     */
    Argument *getOutArgument(const QString &name);

    /*!
     * Returns the related argument with the given \a name and \a direction.
     */
    Argument *getArgument(const QString &name, const QString &direction);

    /*!
     * \property argumentList
     *
     * \brief list of related arguments
     */
    QList<Argument*> argumentList;

    /*!
     * \property name
     *
     * \brief action's name
     */
    QString name;

    /*!
     * \property service
     *
     * \brief the service that this action is related to
     */
    device::Service *service;

    /*!
     * \property method
     *
     * \brief action's method from the related service to be invoked then this action
     * is executed.
     */
    QMetaMethod method;

};

}
}
#endif /* _ACTION_H */
