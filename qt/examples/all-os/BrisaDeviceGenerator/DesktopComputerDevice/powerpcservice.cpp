#include "powerpcservice.h"
#include "off.h"
#include "on.h"
#include "getpcstatus.h"

PowerPcService::PowerPcService() :
    BrisaService("urn:schemas-upnp-org:service:PowerPc:1", "PowerPc",
            "/PowerPc/PowerPc-scpd.xml", "/PowerPc/control",
            "/PowerPc/eventSub") {
    this->setDescriptionFile(
            "../BrisaDeviceGenerator/DesktopComputerDevice/PowerPc-scpd.xml");
    this->addAction(new Off(this));
    this->addAction(new On(this));
    this->addAction(new GetPcStatus(this));
}

PowerPcService::~PowerPcService() {
}
