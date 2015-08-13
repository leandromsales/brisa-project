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

    listOfApps = "{ \"Apps\":[";
    foreach (QObject *obj, appManager->getListApps()) {

        listOfApps += "{";

        BRisaApplication *app = (BRisaApplication *) obj;
        listOfApps += "\"Title\":\"" + app->getTitle() + "\",";
        listOfApps += "\"Icon\":\"" + app->getIconPath() + "\"";

        listOfApps += "},";
    }

    listOfApps.remove(listOfApps.length() - 1, 1);
    listOfApps += "]}";

    this->getVariable("ListApps")->setAttribute(StateVariable::Value, listOfApps);

    OutArgument *outArgs = new OutArgument();
    outArgs->insert("ListOfApps", this->getVariable("ListApps")->getAttribute(StateVariable::Value));
    return outArgs;
}

OutArgument *Functions::getAppInfo(InArgument * const inArguments)
{
    QString appName = (*inArguments)["SelectedApp"];
    QString appInfo = "";

    BRisaApplication *app = appManager->getAppByName(appName);
    OutArgument *outArgs = new OutArgument();

    if(app) {
        appInfo += "{\"Title\":\"" + app->getTitle() + "\",";
        appInfo += "\"Icon\":\"" + app->getIconPath() + "\",";
        appInfo += "\"Description\":\"" + app->getDescription() + "\",";
        appInfo += "\"Url\":\"" + app->getUrl() + "\",";
        appInfo += "\"Services\":[";

        QList<ServiceApp *> services = app->getServices();

        if(!services.isEmpty())
            appInfo += "{";

        foreach (ServiceApp *s, services) {
            appInfo += "\"" + s->getTitle() + "\":\"" + s->getDescription() + "\",";
        }

        if(!services.isEmpty()){

            appInfo.remove(appInfo.length() - 1, 1);
            appInfo += "}";

        }

        appInfo += "]}";
        outArgs->insert("InfoOfApp", appInfo);
    } else {
        outArgs->insert("InfoOfApp", "App doesn't exist!");
    }

    return outArgs;
}

OutArgument *Functions::getApp(InArgument * const inArguments)
{
    QString appName = (*inArguments)["SelectedApp"];

    OutArgument *outArgs = new OutArgument();
    outArgs->insert("TheApp", "Pass the url of the App");
    return outArgs;
}
