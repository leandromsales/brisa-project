#include "bcadevice.h"

using namespace brisa::upnp;

BCADevice::BCADevice(BRisaApplicationManager *manager = 0) : Device(DEVICE_TYPE, DEVICE_FRIENDLY_NAME,
                                   DEVICE_MANUFACTURER, DEVICE_MANUFACTURER_URL,
                                   DEVICE_MODEL_DESCRIPTION, DEVICE_MODEL_NAME, DEVICE_MODEL_NUMBER,
                                   DEVICE_MODEL_URL, DEVICE_SERIAL_NUMBER, getCompleteUuid())
{
    appManager = manager;

    Functions *functions = new Functions(appManager);
    functions->setDescriptionFile(":/src/src/functions.xml");

    this->addService(functions);

    this->start();

    this->listApps = this->getServiceByType("urn:schemas-upnp-org:service:Functions:1")->getVariable("ListApps");
    this->appInfo = this->getServiceByType("urn:schemas-upnp-org:service:Functions:1")->getVariable("AppInfo");
    this->app = this->getServiceByType("urn:schemas-upnp-org:service:Functions:1")->getVariable("App");

}

void BCADevice::printAllApps()
{
    foreach (QObject *obj, appManager->get_apps()) {
        BRisaApplication *app = (BRisaApplication *)obj;
        qDebug() << "Title : " << app->get_title() ;
        qDebug() << "Description : " << app->get_description() ;
    }
}



