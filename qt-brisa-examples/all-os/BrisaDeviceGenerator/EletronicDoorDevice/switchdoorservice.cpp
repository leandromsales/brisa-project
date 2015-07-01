#include "switchdoorservice.h"
#include "close.h"
#include "open.h"
#include "getdoorstatus.h"

SwitchDoorService::SwitchDoorService() :
    BrisaService("urn:schemas-upnp-org:service:SwitchDoor:1", "SwitchDoor",
            "/SwitchDoor/SwitchDoor-scpd.xml", "/SwitchDoor/control",
            "/SwitchDoor/eventSub") {
    this->setDescriptionFile(
            "../BrisaDeviceGenerator/EletronicDoorDevice/SwitchDoor-scpd.xml");
    this->addAction(new Open(this));
    this->addAction(new Close(this));
    this->addAction(new GetDoorStatus(this));
}

SwitchDoorService::~SwitchDoorService() {
}
