#include "powernotebookservice.h"
#include "getbatterycharge.h"
#include "getlevel.h"
#include "setlevel.h"

PowerNotebookService::PowerNotebookService() :
    BrisaService("urn:schemas-upnp-org:service:PowerNotebook:1",
            "PowerNotebook", "/PowerNotebook/Notebook-scpd.xml",
            "/PowerNotebook/control", "/PowerNotebook/eventSub") {
    this->setDescriptionFile(
            "../BrisaDeviceGenerator/NotebookDevice/Notebook-scpd.xml");
    this->addAction(new GetBatteryCharge(this));
    this->addAction(new GetLevel(this));
    this->addAction(new SetLevel(this));
}

PowerNotebookService::~PowerNotebookService() {
}
