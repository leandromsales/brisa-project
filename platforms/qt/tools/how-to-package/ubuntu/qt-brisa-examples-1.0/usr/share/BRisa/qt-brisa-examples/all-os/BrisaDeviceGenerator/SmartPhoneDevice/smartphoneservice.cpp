#include "smartphoneservice.h"
#include "call.h"
#include "hangup.h"
#include "getcallstatus.h"
#include "getlastnumbercalled.h"

SmartPhoneService::SmartPhoneService() :
    BrisaService("urn:schemas-upnp-org:service:PowerCell:1", "SmartPhone",
            "/SmartPhone/SmartPhone-scpd.xml", "/SmartPhone/control",
            "/SmartPhone/eventSub") {
    this->setDescriptionFile(
            "../BrisaDeviceGenerator/SmartPhoneDevice/SmartPhone-scpd.xml");
    this->addAction(new Call(this));
    this->addAction(new HangUp(this));
    this->addAction(new GetCallStatus(this));
    this->addAction(new GetLastNumberCalled(this));
}

SmartPhoneService::~SmartPhoneService() {
}
