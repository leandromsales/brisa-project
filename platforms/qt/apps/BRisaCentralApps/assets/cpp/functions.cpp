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

OutArgument *Functions::getListOfApps()
{
    QString listOfApps;

    listOfApps = "{\nApps :\n[";
    foreach (QObject *obj, appManager->getListApps()) {

        listOfApps += "\n{\n";

        BRisaApplication *app = (BRisaApplication *) obj;
        listOfApps += "Title: " + app->get_title() + ",\n";
        listOfApps += "Icon: " + app->get_iconPath() + "\n";

        listOfApps += "},";
    }

    listOfApps.remove(listOfApps.length() - 1, 1);
    listOfApps += "\n]\n}";

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
        appInfo += "{\nTitle:" + app->get_title() + ",\n";
        appInfo += "Icon:" + app->get_iconPath() + ",\n";
        appInfo += "Description:" + app->get_description() + ",\n";
        appInfo += "Url:" + app->get_url() + ",\n";
        appInfo += "Services:[";

        QList<ServiceApp *> services = app->get_services();

        foreach (ServiceApp *s, services) {
            appInfo += "\n" + s->get_title() + " : " + s->get_description() + ",";
        }

        appInfo.remove(appInfo.length() - 1, 1);
        appInfo += "\n]\n}";
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
