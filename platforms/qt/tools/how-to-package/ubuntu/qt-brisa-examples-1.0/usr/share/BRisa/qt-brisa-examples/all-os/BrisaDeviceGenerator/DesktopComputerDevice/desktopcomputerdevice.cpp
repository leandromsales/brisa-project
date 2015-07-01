#include "desktopcomputerdevice.h"
#include "powerpcservice.h"
#include "pcsystemtimeservice.h"

DesktopComputerDevice::DesktopComputerDevice() :
            BrisaDevice("urn:schemas-upnp-org:device:DesktopPc:1",
                    "Brisa Desktop Computer",
                    "Brisa Team. Embedded Laboratory and INdT Brazil",
                    "https://garage.maemo.org/projects/brisa",
                    "An UPnP Dektop Computer", "Dektop Computer", "1.0",
                    "https://garage.maemo.org/projects/brisa", "1.0",
                    getCompleteUuid()) {
    this->addService(new PowerPcService());
    this->addService(new PcSystemTimeService());
}
