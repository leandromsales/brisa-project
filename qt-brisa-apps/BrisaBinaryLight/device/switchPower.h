#ifndef _SWITCHPOWER_H_
#define _SWITCHPOWER_H_

#define SERVICE_TYPE "urn:schemas-upnp-org:service:SwitchPower:1"
#define SERVICE_ID "SwitchPower"
#define SERVICE_XML_PATH "/SwitchPower/SwitchPower-scpd.xml"
#define SERVICE_CONTROL "/SwitchPower/control"
#define SERVICE_EVENT_SUB "/SwitchPower/eventSub"

#include <BrisaAction>
#include <BrisaService>
#include <QtDebug>

using Brisa::BrisaAction;
using Brisa::BrisaService;

class SwitchPower : public BrisaService
{
    Q_OBJECT
public:
    SwitchPower();

private slots:
    BrisaOutArgument* getstatus(BrisaInArgument * const inArguments,
                                BrisaAction * const action);

    BrisaOutArgument* gettarget(BrisaInArgument * const inArguments,
                                BrisaAction * const action);

    BrisaOutArgument* settarget(BrisaInArgument*const,BrisaAction*const);
};

#endif /* _SWITCHPOWER_H_ */
