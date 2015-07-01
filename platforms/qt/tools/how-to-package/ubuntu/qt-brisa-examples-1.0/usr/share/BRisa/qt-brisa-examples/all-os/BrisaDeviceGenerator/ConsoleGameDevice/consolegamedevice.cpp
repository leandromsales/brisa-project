#include "consolegamedevice.h"
#include "gameconsoleservice.h"

ConsoleGameDevice::ConsoleGameDevice() :
    BrisaDevice("urn:schemas-upnp-org:device:GameConsole:1",
            "Brisa Game Console",
            "Brisa Team. Embedded Laboratory and INdT Brazil",
            "https://garage.maemo.org/projects/brisa", "An UPnP Game Console",
            "GameConsole", "1.0", "https://garage.maemo.org/projects/brisa",
            "1.0", getCompleteUuid()) {
    this->addService(new GameConsoleService());
}
