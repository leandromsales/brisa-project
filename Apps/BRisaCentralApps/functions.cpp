#include "functions.h"

using namespace brisa;
using namespace brisa::upnp;
using namespace brisa::upnp::device;

Functions::Functions(BRisaApplicationManager *manager) : Service(SERVICE_TYPE, SERVICE_ID,
                                 SERVICE_XML_PATH, SERVICE_CONTROL,
                                 SERVICE_EVENT_SUB)
{
    appManager = manager;
}

Functions::~Functions()
{}

OutArgument *Functions::getListOfApps()
{
    QString listOfApps;

    foreach (QObject *obj, appManager->getListApps()) {
        BRisaApplication *app = (BRisaApplication *) obj;
        listOfApps += "Title: " + app->getTitle() + "\n";
        listOfApps += "Icon: " + app->getIconPath() + "\n";
    }

    this->getVariable("ListApps")->setAttribute(StateVariable::Value, listOfApps);

    OutArgument *outArgs = new OutArgument();
    outArgs->insert("ListOfApps", this->getVariable("ListApps")->getAttribute(StateVariable::Value));
    return outArgs;
}

OutArgument *Functions::getInfoOfApp()
{
    OutArgument *outArgs = new OutArgument();
    outArgs->insert("InfoOfApp", this->getVariable("AppInfo")->getAttribute(StateVariable::Value));
    return outArgs;
}

OutArgument *Functions::getApp()
{
    OutArgument *outArgs = new OutArgument();
    outArgs->insert("TheApp", this->getVariable("App")->getAttribute(StateVariable::Value));
    return outArgs;
}
