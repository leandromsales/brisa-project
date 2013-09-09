#include "brisaabstractservice.h"

using namespace Brisa;

BrisaAbstractService::BrisaAbstractService(QObject *parent) : WebService(parent)
{
    this->initialSeq = 0;
    this->nextMulticastSeq = &this->initialSeq;
    this->http = new QtSoapHttpTransport(this);
    this->major = "1";
    this->minor = "1";
}

BrisaAbstractService::BrisaAbstractService(const QString &serviceType,
        const QString &serviceId, const QString &scpdUrl,
        const QString &controlUrl, const QString &eventSubUrl,
        const QString &host, QObject *parent) :
    WebService(parent), controlUrl(controlUrl), eventSubUrl(eventSubUrl),
            fileAddress(), scpdUrl(scpdUrl), serviceType(serviceType),
            serviceId(serviceId)
{

    //Fixes SCPD URL Error: "http://192.168.1.1:49152wps_scpd.xml"
    if(!this->scpdUrl.startsWith("/")) {
        this->scpdUrl.push_front("/");
    }

    this->initialSeq = 0;
    this->nextMulticastSeq = &this->initialSeq;
    this->http = new QtSoapHttpTransport(this);
    this->major = "1";
    this->minor = "1";

    QUrl url(host);
    this->host = url.host();
    this->port = url.port();

    this->http->setHost(host, port);
}

BrisaAbstractService::BrisaAbstractService(BrisaAbstractService &serv) :
    WebService(serv.parent()),

    stateVariableList(serv.getStateVariableList()),

    controlUrl(serv.getAttribute(BrisaAbstractService::ControlUrl)),
            eventSubUrl(serv.getAttribute(BrisaAbstractService::EventSubUrl)),
            fileAddress(serv.getAttribute(BrisaAbstractService::FileAddress)),
            scpdUrl(serv.getAttribute(BrisaAbstractService::ScpdUrl)),
            serviceType(serv.getAttribute(BrisaAbstractService::ServiceType)),
            serviceId(serv.getAttribute(BrisaAbstractService::ServiceId)),

            host(serv.getAttribute(BrisaAbstractService::Host)), port(
                    serv.getAttribute(BrisaAbstractService::Port).toInt()) {
    this->major = serv.getAttribute(BrisaAbstractService::Major);
    this->minor = serv.getAttribute(BrisaAbstractService::Minor);

    this->actionList = serv.getActionList();
    // Copy serv's actions
    for (QList<BrisaAction *>::iterator it = this->actionList.begin(); it
            != this->actionList.end(); ++it) {
        *it = new BrisaAction(*(*it));
    }
}

BrisaAbstractService::~BrisaAbstractService() {
    qDeleteAll(this->actionList);
    this->actionList.clear();
    qDeleteAll(this->stateVariableList);
    this->stateVariableList.clear();
    delete http;
}

void BrisaAbstractService::setAttribute(xmlTags key, const QString &value) {
    switch (key) {
    case Major:
        this->major = value;
        break;
    case Minor:
        this->minor = value;
        break;
    case FileAddress:
        this->fileAddress = value;
        break;
    case ServiceType:
        this->serviceType = value;
        break;
    case ServiceId:
        this->serviceId = value;
        break;
    case ScpdUrl:
        this->scpdUrl = value;
        break;
    case ControlUrl:
        this->controlUrl = value;
        break;
    case EventSubUrl:
        this->eventSubUrl = value;
        break;
    case Host:
        this->host = value;
        this->http->setHost(host, port);
        break;
    case Port:
        this->port = value.toInt();
        this->http->setHost(host, port);
        break;
    }
}

QString BrisaAbstractService::getAttribute(xmlTags key) {
    switch (key) {
    case Major:
        return major;
        break;
    case Minor:
        return minor;
        break;
    case FileAddress:
        return this->fileAddress;
        break;
    case ServiceType:
        return this->serviceType;
        break;
    case ServiceId:
        return this->serviceId;
        break;
    case ScpdUrl:
        return this->scpdUrl;
        break;
    case ControlUrl:
        return this->controlUrl;
        break;
    case EventSubUrl:
        return this->eventSubUrl;
        break;
    case Host:
        return this->host;
        break;
    case Port:
        return QString::number(this->port);
        break;
    default:
        return QString();
        break;
    }
}

void BrisaAbstractService::addAction(const QString &name) {
    // must not add actions named as an already added action
    if (this->getAction(name))
        return;

    this->addAction(new BrisaAction(name));
}

void BrisaAbstractService::addAction(BrisaAction *action) {
    // must not add actions named as an already added action
    if (this->getAction(action->getName()))
        return;

    this->actionList.append(action);
}

BrisaAction *BrisaAbstractService::getAction(const QString &name) {
    for (QList<BrisaAction *>::iterator i = this->actionList.begin(); i
            != this->actionList.end(); ++i) {
        if ((*i)->getName() == name)
            return *i;
    }

    return 0;
}

QList<BrisaAction *> BrisaAbstractService::getActionList() {
    return this->actionList;
}

void BrisaAbstractService::clearActionList(){
    this->actionList.clear();
}


void BrisaAbstractService::addStateVariable(BrisaStateVariable *stateVariable) {
    this->stateVariableList.append(stateVariable);
    if(stateVariable->multicastEvents()) {
        stateVariable->setMulticastSeq(this->nextMulticastSeq);
        stateVariable->setMulticastUsn(this->serviceType);
        stateVariable->setMulticastUdn(this->udn);
        stateVariable->setMulticastSvcid(this->serviceId);
    }
}

void BrisaAbstractService::addStateVariable(const QString &sendEvents,
        const QString &name, const QString &datatype,
        const QString &defaultValue, const QString &maximum,
        const QString &minimum, const QString &step) {
    BrisaStateVariable *stateVariableSwap = new BrisaStateVariable(sendEvents,
            name, datatype, defaultValue, maximum, minimum, step);
    this->addStateVariable(stateVariableSwap);
}

const QList<BrisaStateVariable *> BrisaAbstractService::getStateVariableList() {
    return this->stateVariableList;
}

void BrisaAbstractService::clearStateVariableList(){
    this->stateVariableList.clear();
}

void BrisaAbstractService::clear() {
    this->controlUrl.clear();
    this->eventSubUrl.clear();
    this->fileAddress.clear();
    this->major.clear();
    this->minor.clear();
    this->scpdUrl.clear();
    this->serviceType.clear();
    this->serviceId.clear();

    this->host.clear();
    this->port = 0;
}

QString BrisaAbstractService::errorCodeToString(int errorCode) {
	switch (errorCode) {
		case UPNP_INVALID_ACTION:
				return "Invalid Action";
		case UPNP_INVALID_ARGUMENTS:
				return "Invalid Arguments";
		case UPNP_ACTION_FAILED:
				return "Action Failed";
		case UPNP_INVALID_VALUE_ARGUMENT:
				return "Invalid Value Argument";
		case UPNP_ARGUMENT_VALUE_OUT_OF_RANGE:
				return "Argument Value Out of Range";
		case UPNP_OPTIONAL_ACTION_NOT_IMPLEMENTED:
				return "Optional Action not Implemented";
		case UPNP_OUT_OF_MEMORY:
				return "Out of Memory";
		case UPNP_HUMAN_INTERVENTION_REQUIRED:
				return "Human Intervention Required";
		case UPNP_STRING_ARGUMENT_TOO_LONG:
				return "String Argument Too Long";
	}
	return "";
}

void BrisaAbstractService::setUdn(QString udn)
{
    this->udn = udn;
}
