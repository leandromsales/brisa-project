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

BrisaOutArgument* MediaRendererDeviceService::setchanel(BrisaInArgument* const inArguments,
                                                        BrisaAction* const action) {
    action->getStateVariable("SelectedChanel")->setAttribute(BrisaStateVariable::Value,inArguments->value("NewChanel"));
    qDebug() << "in setchanel TVoperation";
    action->getStateVariable("Operation")->setAttribute(BrisaStateVariable::Value, inArguments->value("ChangeChanel"));

    BrisaOutArgument* outArgs = new BrisaOutArgument();

    outArgs->insert("Result", action->getStateVariable("Operation")->getAttribute(BrisaStateVariable::Value));

    return outArgs;

}

int MediaRendererDeviceService::preAction(BrisaInArgument* const inArguments, BrisaAction* const action, QString &errorDescription) {
   Q_UNUSED(errorDescription);
   qDebug() << "PREACTION METHOD CALLED FOR ACTION " << action->getName();
   qDebug() << "NewStatus IN argument is: " << inArguments->value("OffOnTV");
   //errorDescription = "UPnP-UP Access denied for action " + action->getName();
   //return 801;
   return UPNP_SUCCESS;
}

BrisaOutArgument* MediaRendererDeviceService::offon(BrisaInArgument* const inArguments, BrisaAction* const action){
    qDebug() << "in offon TVoperation";
    action->getStateVariable("Operation")->setAttribute(BrisaStateVariable::Value, "OffOnTV");
    BrisaOutArgument* outArgs = new BrisaOutArgument();
    outArgs->insert("Result", action->getStateVariable("Operation")->getAttribute(BrisaStateVariable::Value));
    return outArgs;
}

int MediaRendererDeviceService::postAction(BrisaInArgument* const inArguments, BrisaOutArgument* const outArguments, BrisaAction* const action, QString &errorDescription) {
   Q_UNUSED(errorDescription);
   qDebug() << "POSTACTION METHOD CALLED FOR ACTION " << action->getName();
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
