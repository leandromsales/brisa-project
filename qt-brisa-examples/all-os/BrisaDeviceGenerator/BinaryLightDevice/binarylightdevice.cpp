#include "binarylightdevice.h"
#include "switchlightservice.h"

BinaryLightDevice::BinaryLightDevice() :
            BrisaDevice("urn:schemas-upnp-org:device:BrisaBinaryLight:1",
                    "Brisa Binary Light Device",
                    "Brisa Team. Embedded Laboratory and INdT Brazil",
                    "https://garage.maemo.org/projects/brisa",
                    "An UPnP Binary Light Device", "Brisa Binary Light Device",
                    "1.0", "https://garage.maemo.org/projects/brisa", "1.0",
                    getCompleteUuid()) {
    this->addService(new SwitchLightService());
}
