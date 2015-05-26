#include "mediarendererdeviceservice.h"

namespace brisa {
namespace upnp {
namespace device {
namespace av {

MediaRendererDeviceService::MediaRendererDeviceService(QObject *parent) :
    Service(SERVICE_TYPE,
                 SERVICE_ID,
                 SERVICE_XML_PATH,
                 SERVICE_CONTROL,
                 SERVICE_EVENT_SUB,
                 "http://192.168.1.4:34223",
                 parent) { }

OutArgument* MediaRendererDeviceService::setchanel(InArgument* const inArguments,
                                                        Action* const action) {
    action->getStateVariable("SelectedChanel")->setAttribute(StateVariable::Value,
                                                             inArguments->value("NewChanel"));
    qDebug() << "in setchanel TVoperation";
    action->getStateVariable("Operation")->setAttribute(StateVariable::Value,
                                                        inArguments->value("ChangeChanel"));

    OutArgument* outArgs = new OutArgument();

    outArgs->insert("Result", action->getStateVariable("Operation")->getAttribute(StateVariable::Value));

    return outArgs;

}

int MediaRendererDeviceService::preAction(InArgument* const inArguments,
                                          Action* const action, QString &errorDescription) {
   Q_UNUSED(errorDescription);
   qDebug() << "NewStatus IN argument is: " << inArguments->value("OffOnTV");
   //errorDescription = "UPnP-UP Access denied for action " + action->getName();
   //return 801;
   return UPNP_SUCCESS;
}

OutArgument* MediaRendererDeviceService::offon(InArgument* const inArguments,
                                               Action* const action){
    qDebug() << "in offon TVoperation";
    action->getStateVariable("Operation")->setAttribute(StateVariable::Value, "OffOnTV");
    OutArgument* outArgs = new OutArgument();
    outArgs->insert("Result",
                    action->getStateVariable("Operation")->getAttribute(StateVariable::Value));
    return outArgs;
}

int MediaRendererDeviceService::postAction(InArgument* const inArguments,
                                           OutArgument* const outArguments,
                                           Action* const action,
                                           QString &errorDescription) {
   Q_UNUSED(errorDescription);
   qDebug() << "NewStatus IN argument is: " << inArguments->value("NewChanel");
   qDebug() << "outArg1 out argument is: " << outArguments->value("Result");
   //errorDescription = "error description";
   //return UPNP_ACTION_FAILED;
   return UPNP_SUCCESS;
}

}  // namespace av
}
}
}
