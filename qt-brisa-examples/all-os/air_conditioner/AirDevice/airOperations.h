#ifndef AIROPERATIONS_H
#define AIROPERATIONS_H

#define SERVICE_TYPE "urn:schemas-upnp-org:service:AirOperations:1"
#define SERVICE_ID "AirOperations"
#define SERVICE_XML_PATH "/AirOperations/airOperations.xml"
#define SERVICE_CONTROL "/AirOperations/control"
#define SERVICE_EVENT_SUB "/AirOperations/eventSub"
#include <BrisaUpnp/BrisaAction>
#include <BrisaUpnp/BrisaService>
#include <BrisaUpnp/BrisaStateVariable>
#include <QtDebug>


using Brisa::BrisaService;
using Brisa::BrisaAction;
using Brisa::BrisaStateVariable;

// The Service
class AirOperation : public BrisaService
{
    Q_OBJECT
public:
    AirOperation();
    Q_INVOKABLE
            BrisaOutArgument* settemp(BrisaInArgument* const inArgument, BrisaAction* const action);
    Q_INVOKABLE
            BrisaOutArgument* gettemp(BrisaInArgument* const inArgument, BrisaAction* const action);
    Q_INVOKABLE
            BrisaOutArgument* setstatus(BrisaInArgument* const inArgument, BrisaAction* const action);
    Q_INVOKABLE
            BrisaOutArgument* getstatus(BrisaInArgument* const inArgument, BrisaAction* const action);
};

#endif // AIROPERATIONS_H
