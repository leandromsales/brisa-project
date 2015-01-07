#include "action.h"
#include "device/service.h"
#include <QDebug>


namespace brisa {
namespace upnp {

using namespace device;

Action::Action(QString name, Service *service, QObject *parent):
    QObject(parent),
    name(name),
    service(service)
{

}

Action::Action(QString name, Service *service, const QMetaMethod &method, QObject *parent):
    QObject(parent),
    name(name),
    service(service),
    method(method)
{

}

Action::Action(const Action &action) :
                         QObject(action.parent()),
                         name(action.getName()),
                         service(action.getService())
{
    this->method = action.getMethod();
    QList<Argument *> argumentList = action.getArgumentList();
    foreach (Argument *argument, argumentList) {
        this->argumentList.append(new Argument(*argument));
    }
}


Action::~Action()
{
    qDeleteAll(this->argumentList);
    this->argumentList.clear();
}

void Action::addArgument(QString name, QString direction, QString relatedStateVariable)
{
    Argument *argumentSwap = new Argument(name, direction, relatedStateVariable);
    this->argumentList.append(argumentSwap);
}

void Action::addArgument(Argument *argumentA)
{
    this->argumentList.append(argumentA);
}

bool Action::removeArgument(Argument* argumentA)
{
    return this->argumentList.removeOne(argumentA);
}

void Action::addArguments(const QList<Argument*> arguments)
{
    for (QList<Argument*>::const_iterator i = arguments.begin(); i != arguments.end(); ++i) {
        this->addArgument(*i);
    }
}

void Action::setName(QString name)
{
    this->name = name;
}

QString Action::getName() const
{
    return this->name;
}

void Action::setService(Service *service)
{
    this->service = service;
}

Service* Action::getService() const
{
    return this->service;
}

StateVariable* Action::getStateVariable(const QString &name) const
{
    if (!this->service) {
        qDebug() << this->getName() << " action couldn't find its related Service.";
        return 0;
    }

    StateVariable *stateVariable = service->getVariable(name);
    if (!stateVariable) {
        qDebug() << this->getName() << " action couldn't find the State Variable.";
    }

    return stateVariable;
}

QList<Argument *> Action::getArgumentList() const
{
    return this->argumentList;
}

void Action::clearArgumentList()
{
    this->argumentList.clear();
}

bool Action::call(InArgument *inArguments, OutArgument *&outArguments)
{
    // Checking IN variables
    for (InArgument::const_iterator i = inArguments->begin(); i != inArguments->end(); ++i) {
        Argument *arg = this->getInArgument(i.key());
        if (!arg) {
            qDebug() << "Error: action " << this->getName() << " has no IN argument named '" << i.key() << "'.";
            return false;
        }
    }

    // use the run() invocation.
    if (this->method.methodIndex() >= 0) { // checking if action has a QMetaMethod specified
        bool execResult = this->method.invoke(this->service,
                                              Qt::DirectConnection,
                                              Q_RETURN_ARG(OutArgument *, outArguments),
                                              Q_ARG(InArgument *, inArguments),
                                              Q_ARG(Action *, this));
        if (!execResult) {
            qDebug() << "Error invoking defined action expressed by the service method " << this->method.methodSignature();
            return false;
        }
    } else {
        qDebug() << "Error: there is not method to be invoked by this action.";
        return false;
    }

    // Check OUT variables
    for (OutArgument::const_iterator i = outArguments->begin(); i != outArguments->end(); ++i) {
        Argument *arg = this->getOutArgument(i.key());
        if (!arg) {
            qDebug() << "Error: action " << this->getName() << " has no OUT expected argument named '" << i.key() << "'.";
            return false;
        }
    }

    return true;
}

Argument* Action::getInArgument(const QString &name)
{
    return this->getArgument(name, "in");
}

Argument* Action::getOutArgument(const QString &name)
{
    return this->getArgument(name, "out");
}

Argument* Action::getArgument(const QString &name, const QString &direction)
{
    foreach (Argument *argument, this->getArgumentList()) {
        if (argument->getAttribute(Argument::Direction) == direction && argument->getAttribute(Argument::ArgumentName) == name) {
                return argument;
        }
    }

    return 0;
}

void Action::setMethod(const QMetaMethod &method, Service *service)
{
    this->method = method;
    this->setService(service);
}

QMetaMethod Action::getMethod() const
{
    return this->method;
}

}
}
