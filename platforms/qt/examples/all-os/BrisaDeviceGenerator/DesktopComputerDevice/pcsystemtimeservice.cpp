#include "pcsystemtimeservice.h"
#include "getsystemtime.h"

PcSystemTimeService::PcSystemTimeService() :
    BrisaService("urn:schemas-upnp-org:service:PcSystemTime:1", "PcSystemTime",
            "/PcSystemTime/PcSystemTime-scpd.xml", "/PcSystemTime/control",
            "/PcSystemTime/eventSub") {
    this->setDescriptionFile(
            "../BrisaDeviceGenerator/DesktopComputerDevice/PcSystemTime-scpd.xml");
    this->addAction(new GetSystemTime(this));
}

PcSystemTimeService::~PcSystemTimeService() {
}
