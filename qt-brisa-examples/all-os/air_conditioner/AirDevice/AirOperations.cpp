#include "airOperations.h"

AirOperation::AirOperation():BrisaService(SERVICE_TYPE,
                                        SERVICE_ID,
                                                     SERVICE_XML_PATH,
                                                     SERVICE_CONTROL,
                                                     SERVICE_EVENT_SUB){}

BrisaOutArgument* AirOperation::settemp(BrisaInArgument* const inArguments, BrisaAction* const action){
    qDebug() << "changed temp";
    action->getStateVariable("Temp")->setAttribute(BrisaStateVariable::Value,inArguments->value("NewTargetValue"));
    BrisaOutArgument* outArg = new BrisaOutArgument();
    return outArg;
}

BrisaOutArgument* AirOperation::gettemp(BrisaInArgument* const inArguments, BrisaAction* const action){
    Q_UNUSED(inArguments);
    qDebug() << "get temp";
    BrisaOutArgument *outArg = new BrisaOutArgument();
    outArg->insert("RetTempValue", action->getStateVariable("Temp")->getAttribute(BrisaStateVariable::Value));
       return outArg;

}

BrisaOutArgument* AirOperation::setstatus(BrisaInArgument* const inArguments, BrisaAction* const action){
    action->getStateVariable("Status")->setAttribute(BrisaStateVariable::Value,inArguments->value("NewTargetValue"));
    BrisaOutArgument* outArg = new BrisaOutArgument();
    return outArg;
}


BrisaOutArgument* AirOperation::getstatus(BrisaInArgument* const inArguments, BrisaAction* const action){
    Q_UNUSED(inArguments);
    qDebug() << "get status ";
    BrisaOutArgument *outArg = new BrisaOutArgument();
    outArg->insert("ResultStatus", action->getStateVariable("Status")->getAttribute(BrisaStateVariable::Value));
    return outArg;

}
