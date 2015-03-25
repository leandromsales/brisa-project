#include "functions.h"

using namespace brisa;
using namespace brisa::upnp;
using namespace brisa::upnp::device;

Functions::Functions() : Service(SERVICE_TYPE, SERVICE_ID,
                                 SERVICE_XML_PATH, SERVICE_CONTROL,
                                 SERVICE_EVENT_SUB)
{}

Functions::~Functions()
{}

OutArgument *Functions::getStatus()
{
    OutArgument *outArgs = new OutArgument();
    outArgs->insert("Estado", this->getVariable("Status")->getAttribute(StateVariable::Value));
    return outArgs;
}

OutArgument *Functions::setStatus(InArgument * const inArguments)
{
    this->getVariable("Target")->setAttribute(StateVariable::Value, (*inArguments)["Nome"]);
    this->getVariable("Status")->setAttribute(StateVariable::Value, (*inArguments)["Nome"]);
    return new OutArgument();
}
