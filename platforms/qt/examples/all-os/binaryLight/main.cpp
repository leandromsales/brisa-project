#include <QtCore>
#include <QCoreApplication>
#include <QtDebug>

#include <BrisaUpnp/BrisaDevice>
#include <BrisaUtils/BrisaLog>

using namespace Brisa;

#include "switchPower.h"

#define DEVICE_TYPE              "urn:schemas-upnp-org:device:BinaryLight:1"
#define DEVICE_FRIENDLY_NAME     "Binary Light Device"
#define DEVICE_MANUFACTURER      "Brisa Team. Embedded Laboratory and INdT Brazil"
#define DEVICE_MANUFACTURER_URL  "https://garage.maemo.org/projects/brisa"
#define DEVICE_MODEL_DESCRIPTION "An UPnP Binary Light Device"
#define DEVICE_MODEL_NAME        "Binary Light Device"
#define DEVICE_MODEL_NUMBER      "1.0"
#define DEVICE_MODEL_URL         "https://garage.maemo.org/projects/brisa"
#define DEVICE_SERIAL_NUMBER     "1.0"
#define DEVICE_UDN               "uuid:367a4fa7-cf68-4cc8-844d-0af4c615cecb"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    brisaLogInitialize();

    BrisaDevice binaryLight(DEVICE_TYPE,
                            DEVICE_FRIENDLY_NAME,
                            DEVICE_MANUFACTURER,
                            DEVICE_MANUFACTURER_URL,
                            DEVICE_MODEL_DESCRIPTION,
                            DEVICE_MODEL_NAME,
                            DEVICE_MODEL_NUMBER,
                            DEVICE_MODEL_URL,
                            DEVICE_SERIAL_NUMBER,
                            DEVICE_UDN);

    SwitchPower switchPower;
    switchPower.setDescriptionFile("SwitchPower-scpd.xml");

    binaryLight.addService(&switchPower);
    binaryLight.start();

    return a.exec();
}

