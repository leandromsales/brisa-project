#include "brisaaction.h"
#include "upnp/device/brisaservice.h"
#include <QDebug>


namespace brisa {
namespace upnp {

using namespace device;

BrisaAction::BrisaAction(QString name, Service *service, QObject *parent):
	QObject(parent),
    name(name),
    service(service)
{

}

BrisaAction::BrisaAction(QString name, Service *service, const QMetaMethod &method, QObject *parent):
	QObject(parent),
    name(name),
    service(service),
    method(method)
{

}

BrisaAction::BrisaAction(const BrisaAction &action) :
                         QObject(action.parent()),
                         name(action.getName()),
                         service(action.getService())
{
    this->method = action.getMethod();
    QList<BrisaArgument *> argumentList = action.getArgumentList();
    foreach (BrisaArgument *argument, argumentList) {
        this->argumentList.append(new BrisaArgument(*argument));
    }
}


BrisaAction::~BrisaAction()
{
    qDeleteAll(this->argumentList);
    this->argumentList.clear();
}

void BrisaAction::addArgument(QString name, QString direction, QString relatedStateVariable)
{
    BrisaArgument *argumentSwap = new BrisaArgument(name, direction, relatedStateVariable);
    this->argumentList.append(argumentSwap);
}

void BrisaAction::addArgument(BrisaArgument *argumentA)
{
    this->argumentList.append(argumentA);
}

bool BrisaAction::removeArgument(BrisaArgument* argumentA)
{
	return this->argumentList.removeOne(argumentA);
}

void BrisaAction::addArguments(const QList<BrisaArgument*> arguments)
{
    for (QList<BrisaArgument*>::const_iterator i = arguments.begin(); i != arguments.end(); ++i) {
        this->addArgument(*i);
    }
}

void BrisaAction::setName(QString name)
{
    this->name = name;
}

QString BrisaAction::getName() const
{
    return this->name;
}

void BrisaAction::setService(Service *service)
{
    this->service = service;
}

Service* BrisaAction::getService() const
{
    return this->service;
}

BrisaStateVariable* BrisaAction::getStateVariable(const QString &name) const
{
    if (!this->service) {
        qDebug() << this->getName() << " action couldn't find its related Service.";
        return 0;
    }

    BrisaStateVariable *stateVariable = service->getVariable(name);
    if (!stateVariable) {
        qDebug() << this->getName() << " action couldn't find the State Variable.";
    }

    return stateVariable;
}

QList<BrisaArgument *> BrisaAction::getArgumentList() const
{
    return this->argumentList;
}

void BrisaAction::clearArgumentList()
{
    this->argumentList.clear();
}

bool BrisaAction::call(BrisaInArgument *inArguments, BrisaOutArgument *&outArguments)
{
    // Checking IN variables
    for (BrisaInArgument::const_iterator i = inArguments->begin(); i != inArguments->end(); ++i) {
        BrisaArgument *arg = this->getInArgument(i.key());
        if (!arg) {
            qDebug() << "Error: action " << this->getName() << " has no IN argument named '" << i.key() << "'.";
            return false;
        }
    }

    // use the run() invocation.
    if (this->method.methodIndex() >= 0) { // checking if action has a QMetaMethod specified
        bool execResult = this->method.invoke(this->service,
                                              Qt::DirectConnection,
                                              Q_RETURN_ARG(BrisaOutArgument *, outArguments),
                                              Q_ARG(BrisaInArgument *, inArguments),
                                              Q_ARG(BrisaAction *, this));
        if (!execResult) {
            //Marden//qDebug() << "Error invoking defined action expressed by the service method " << this->method.methodSignature();
            return false;
        }
    } else {
        qDebug() << "Error: there is not method to be invoked by this action.";
        return false;
    }

    // Check OUT variables
    for (BrisaOutArgument::const_iterator i = outArguments->begin(); i != outArguments->end(); ++i) {
        BrisaArgument *arg = this->getOutArgument(i.key());
        if (!arg) {
            qDebug() << "Error: action " << this->getName() << " has no OUT expected argument named '" << i.key() << "'.";
            return false;
        }
    }

    return true;
}

BrisaArgument* BrisaAction::getInArgument(const QString &name)
{
    return this->getArgument(name, "in");
}

BrisaArgument* BrisaAction::getOutArgument(const QString &name)
{
    return this->getArgument(name, "out");
}

BrisaArgument* BrisaAction::getArgument(const QString &name, const QString &direction)
{
    foreach (BrisaArgument *argument, this->getArgumentList()) {
        if (argument->getAttribute(BrisaArgument::Direction) == direction && argument->getAttribute(BrisaArgument::ArgumentName) == name) {
                return argument;
        }
    }

    return 0;
}

void BrisaAction::setMethod(const QMetaMethod &method, Service *service)
{
    this->method = method;
	this->setService(service);
}

QMetaMethod BrisaAction::getMethod() const
{
    return this->method;
}

}
}
