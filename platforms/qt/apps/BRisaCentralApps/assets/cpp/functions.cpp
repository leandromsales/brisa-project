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
    QJsonArray jsonListOfApps;

    foreach (QObject *obj, appManager->get_apps()) {

        QJsonObject jsonApp;

        BRisaApplication *app = (BRisaApplication *) obj;

        jsonApp.insert("Title",QJsonValue(app->get_title()));
        jsonApp.insert("Icon",app->get_iconPath());

        jsonListOfApps.append(jsonApp);
    }

    QJsonObject mainJsonObj;
    mainJsonObj.insert("Apps",QJsonValue(jsonListOfApps));
    QJsonDocument jsonDoc(mainJsonObj);

    this->getVariable("ListApps")->setAttribute(StateVariable::Value, jsonDoc.toJson());

    OutArgument *outArgs = new OutArgument();
    outArgs->insert("ListOfApps", this->getVariable("ListApps")->getAttribute(StateVariable::Value));
    return outArgs;
}

OutArgument *Functions::getAppInfo(InArgument * const inArguments)
{
    QString appName = (*inArguments)["SelectedApp"];

    BRisaApplication *app = appManager->getAppByName(appName);
    OutArgument *outArgs = new OutArgument();

    if(app) {

        QJsonObject jsonApp;
        jsonApp.insert("Title", app->get_title());
        jsonApp.insert("Icon",app->get_iconPath());
        jsonApp.insert("Description",app->get_description());
        jsonApp.insert("Url",app->get_url());

        QList<ServiceApp *> services = app->get_services()->toList();
        QJsonArray jsonServices;

        foreach (ServiceApp *s, services) {
            QJsonObject jsonService;
            jsonService.insert(s->get_title(), QJsonValue(s->get_description()));
            jsonServices.append(jsonService);
        }

        jsonApp.insert("Services",QJsonValue(jsonServices));
        QJsonDocument jsonDoc(jsonApp);
        outArgs->insert("InfoOfApp", jsonDoc.toJson());

    } else {
        outArgs->insert("InfoOfApp", "App doesn't exist!");
    }

    return outArgs;
}

OutArgument *Functions::getApp(InArgument * const inArguments)
{
    QString appName = (*inArguments)["SelectedApp"];

    BRisaApplication *app = appManager->getAppByName(appName);
    OutArgument *outArgs = new OutArgument();

    if(app) {

        QJsonObject jsonApp;
        jsonApp.insert("Url", QJsonValue(app->get_url()));

        QJsonDocument jsonDoc(jsonApp);

        outArgs->insert("TheApp",jsonDoc.toJson());

    } else
        outArgs->insert("TheApp", "App Doesn't exists!");

    return outArgs;
}
