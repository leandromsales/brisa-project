#include "phoneDirectoryFunctions.h"

PhoneDirectoryFunctions::PhoneDirectoryFunctions() : BrisaService(SERVICE_TYPE,
                                                                SERVICE_ID,
                                                                SERVICE_XML_PATH,
                                                                SERVICE_CONTROL,
                                                                SERVICE_EVENT_SUB){}



BrisaOutArgument* PhoneDirectoryFunctions::search(BrisaInArgument* const inArguments, BrisaAction* const action){
    action->getStateVariable("name")->setAttribute(BrisaStateVariable::Value,inArguments->value("newName"));
    BrisaOutArgument* outArg = new BrisaOutArgument();
    outArg->insert("newPhone", action->getStateVariable("phone")->getAttribute(BrisaStateVariable::Value));
    return outArg;
}
