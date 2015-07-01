#include "switchlightservice.h"
#include "getstatus.h"
#include "turnon.h"
#include "turnoff.h"

SwitchLightService::SwitchLightService() :
    BrisaService("urn:schemas-upnp-org:service:SwitchPower:1", "SwitchPower",
            "/SwitchPower/SwitchPower-scpd.xml", "/SwitchPower/control",
            "/SwitchPower/eventSub") {
    this->setDescriptionFile(
            "../BrisaDeviceGenerator/BinaryLightDevice/SwitchPower-scpd.xml");
    this->addAction(new GetStatus(this));
    this->addAction(new TurnOn(this));
    this->addAction(new TurnOff(this));
}

SwitchLightService::~SwitchLightService() {
}
