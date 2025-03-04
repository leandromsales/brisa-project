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

OutArgument* SwitchPower::GetStatus(InArgument *const inArguments, Action *const action)
{
    Q_UNUSED(inArguments);
    Q_UNUSED(action);
    OutArgument *outArgs = new OutArgument();
    outArgs->insert("ResultStatus", this->getVariable("Status")->
                    getAttribute(StateVariable::Value));
    return outArgs;
}

OutArgument* SwitchPower::GetTarget(InArgument *const inArguments, Action *const action)
{
    Q_UNUSED(inArguments);
    Q_UNUSED(action);
    OutArgument *outArgs = new OutArgument();
    outArgs->insert("RetTargetValue", this->getVariable("Target")->
                    getAttribute(StateVariable::Value));
    return outArgs;
}

OutArgument* SwitchPower::SetTarget(InArgument *const inArguments, Action *const action)
{
    Q_UNUSED(action);
    this->getVariable("Target")->setAttribute(StateVariable::Value,
                                                   (*inArguments)["NewTargetValue"]);
    this->getVariable("Status")->setAttribute(StateVariable::Value,
                                                   (*inArguments)["NewTargetValue"]);
    return new OutArgument();
}

