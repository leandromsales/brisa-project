/*
 * MyService.cpp
 *
 *  Created on: Sep 21, 2013
 *      Author: rodrigopex
 */
#include "SwitchPower.hpp"

using namespace brisa::upnp;
using namespace brisa::upnp::device;

SwitchPower::SwitchPower() :
        Service(SERVICE_TYPE,
                     SERVICE_ID,
                     SERVICE_XML_PATH,
                     SERVICE_CONTROL,
                     SERVICE_EVENT_SUB)
{
}

BrisaOutArgument* SwitchPower::GetStatus(BrisaInArgument *const inArguments, Action *const action)
{
    Q_UNUSED(inArguments);
    Q_UNUSED(action);
    BrisaOutArgument *outArgs = new BrisaOutArgument();
    outArgs->insert("ResultStatus", this->getVariable("Status")->
                    getAttribute(StateVariable::Value));
    return outArgs;
}

BrisaOutArgument* SwitchPower::GetTarget(BrisaInArgument *const inArguments, Action *const action)
{
    Q_UNUSED(inArguments);
    Q_UNUSED(action);
    BrisaOutArgument *outArgs = new BrisaOutArgument();
    outArgs->insert("RetTargetValue", this->getVariable("Target")->
                    getAttribute(StateVariable::Value));
    return outArgs;
}

BrisaOutArgument* SwitchPower::SetTarget(BrisaInArgument *const inArguments, Action *const action)
{
    Q_UNUSED(action);
    this->getVariable("Target")->setAttribute(StateVariable::Value,
                                                   (*inArguments)["NewTargetValue"]);
    this->getVariable("Status")->setAttribute(StateVariable::Value,
                                                   (*inArguments)["NewTargetValue"]);
    return new BrisaOutArgument();
}

