#ifndef SIMPLEDEVICE_H
#define SIMPLEDEVICE_H

#include <QApplication>
#include <QDesktopWidget>

#include "upnp/device/Device"
#include "functions.h"
#include "simpledevice_ui.h"

#define DEVICE_TYPE              "urn:schemas-upnp-org:device:SimpleDevice:1"
#define DEVICE_FRIENDLY_NAME     "Simple Device BRisa"
#define DEVICE_MANUFACTURER      "Brisa Team. Embedded Laboratory and INdT Brazil"
#define DEVICE_MANUFACTURER_URL  "https://garage.maemo.org/projects/brisa"
#define DEVICE_MODEL_DESCRIPTION "The Most Basic Sample Device with BRisa"
#define DEVICE_MODEL_NAME        "Simple Device BRisa"
#define DEVICE_MODEL_NUMBER      "1.0"
#define DEVICE_MODEL_URL         "https://garage.maemo.org/projects/brisa"
#define DEVICE_SERIAL_NUMBER     "1.0"
#define DEVICE_UDN               "uuid:367a4fa7-cf68-4cc8-844d-0af4c615cecb"

using namespace brisa::upnp::device;

class SimpleDevice : public QWidget
{
    Q_OBJECT

public:

    SimpleDevice();

public slots:

    /*
     * Sempre que a variável de estado Status for alterada irá executar esse Slot
     */
    void statechanged(brisa::upnp::StateVariable *var);

private:

    Device simpleDevice;
    brisa::upnp::StateVariable *status;
    brisa::upnp::StateVariable *target;

    //Ui Variables
    QLabel *statusLabel;

};

#endif // SIMPLEDEVICE_H
