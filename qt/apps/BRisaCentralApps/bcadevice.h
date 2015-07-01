#ifndef BCADEVICE_H
#define BCADEVICE_H

#include <QWidget>
#include <QDebug>

#include "upnp/device/Device"
#include "functions.h"

#include "brisaapplicationmanager.h"

#define DEVICE_TYPE              "urn:schemas-upnp-org:device:BCA:1"
#define DEVICE_FRIENDLY_NAME     "BRisa Central Applications"
#define DEVICE_MANUFACTURER      "Brisa Team. Embedded Laboratory and INdT Brazil"
#define DEVICE_MANUFACTURER_URL  "https://garage.maemo.org/projects/brisa"
#define DEVICE_MODEL_DESCRIPTION "A Central of Applications Based on Context"
#define DEVICE_MODEL_NAME        "BRisa Central Applications"
#define DEVICE_MODEL_NUMBER      "1.0"
#define DEVICE_MODEL_URL         "https://garage.maemo.org/projects/brisa"
#define DEVICE_SERIAL_NUMBER     "1.0"
#define DEVICE_UDN               "uuid:367a4fa7-cf68-4cc8-844d-0af4c615cecb"

using namespace brisa::upnp::device;

class BCADevice : public Device
{
    Q_OBJECT

public:

    BCADevice(BRisaApplicationManager *manager);

    void printAllApps();

private:

    BRisaApplicationManager *appManager;

    brisa::upnp::StateVariable *listApps;
    brisa::upnp::StateVariable *appInfo;
    brisa::upnp::StateVariable *app;

};

#endif // BCADEVICE_H
