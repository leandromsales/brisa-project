#include "eletronicdoordevice.h"
#include "switchdoorservice.h"

EletronicDoorDevice::EletronicDoorDevice() :
            BrisaDevice("urn:schemas-upnp-org:device:BrisaEletronicDoor:1",
                    "Brisa Eletronic Door",
                    "Brisa Team. Embedded Laboratory and INdT Brazil",
                    "https://garage.maemo.org/projects/brisa",
                    "An UPnP Eletronic Door", "Brisa Eletronic Door", "1.0",
                    "https://garage.maemo.org/projects/brisa", "1.0",
                    getCompleteUuid()) {
    this->addService(new SwitchDoorService());
}
