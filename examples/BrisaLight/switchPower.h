#ifndef _SWITCHPOWER_H_
#define _SWITCHPOWER_H_

#define SERVICE_TYPE "urn:schemas-upnp-org:service:SwitchPower:1"
#define SERVICE_ID "SwitchPower"
#define SERVICE_XML_PATH "/SwitchPower/SwitchPower-scpd.xml"
#define SERVICE_CONTROL "/SwitchPower/control"
#define SERVICE_EVENT_SUB "/SwitchPower/eventSub"

#include "upnp/action.h"
#include "upnp/device/service.h"
#include <QtDebug>

class SwitchPower : public brisa::upnp::device::Service
{
    Q_OBJECT
public:
    SwitchPower();

private slots:
    OutArgument* getstatus(InArgument * const inArguments,
                                brisa::upnp::Action * const action);

//    OutArgument* gettarget(InArgument * const inArguments,
//                               brisa::upnp::Action * const action);

//    OutArgument* settarget(InArgument*const, brisa::upnp::Action*const);
};

#endif /* _SWITCHPOWER_H_ */
