#include "functions.h"

using namespace brisa;
using namespace brisa::upnp;
using namespace brisa::upnp::device;

Functions::Functions(BRisaApplicationManager *manager, QString portIP) : Service(SERVICE_TYPE, SERVICE_ID,
                                 SERVICE_XML_PATH, SERVICE_CONTROL,
                                 SERVICE_EVENT_SUB)
{
    appManager = manager; m_portIP = portIP;
}

Functions::~Functions() {}

OutArgument *Functions::getListOfApps()
{
    QJsonDocument jsonDoc(appManager->toJson());
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
        QJsonDocument jsonDoc(app->toJsonObject());
        outArgs->insert("InfoOfApp", jsonDoc.toJson());
    } else outArgs->insert("InfoOfApp", "App doesn't exist!");
    return outArgs;
}

OutArgument *Functions::getApp(InArgument * const inArguments)
{
    QString appName = (*inArguments)["SelectedApp"];
    BRisaApplication *app = appManager->getAppByName(appName);
    OutArgument *outArgs = new OutArgument();
    if(app) {
        QJsonObject jsonApp;
        QString ret = m_portIP + "/apps/" + app->get_title() + ".compe";
        jsonApp.insert("path",ret);
        outArgs->insert("TheApp",QJsonDocument(jsonApp).toJson());
    } else
        outArgs->insert("TheApp", "App Doesn't exists!");

    return outArgs;
}
