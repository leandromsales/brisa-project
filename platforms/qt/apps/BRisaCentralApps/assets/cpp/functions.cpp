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

    } else {
        outArgs->insert("InfoOfApp", "App doesn't exist!");
    }

    return outArgs;
}

OutArgument *Functions::getApp(InArgument * const inArguments)
{
    qsrand(time(0));
    QString appName = (*inArguments)["SelectedApp"];

    BRisaApplication *app = appManager->getAppByName(appName);
    OutArgument *outArgs = new OutArgument();

    if(app) {
        QJsonObject jsonApp;
        quint16 port = (qrand()%55535)+10000;
        m_server = new BCATcpServer(app->compePath(),port);
        QString ret = m_server->ip() + ":" + QString::number(port) + app->compePath().replace("file:///","/");
        outArgs->insert("TheApp",ret);
    } else
        outArgs->insert("TheApp", "App Doesn't exists!");

    return outArgs;
}
