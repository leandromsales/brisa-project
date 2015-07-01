#include "smartphonedevice.h"
#include "smartphoneservice.h"

SmartPhoneDevice::SmartPhoneDevice() :
    BrisaDevice("urn:schemas-upnp-org:device:SmartPhone:1", "Brisa SmartPhone",
            "Brisa Team. Embedded Laboratory and INdT Brazil",
            "https://garage.maemo.org/projects/brisa", "An UPnP SmartPhone",
            "SmartPhone", "1.0", "https://garage.maemo.org/projects/brisa",
            "1.0", getCompleteUuid()) {
    this->addService(new SmartPhoneService());
}
