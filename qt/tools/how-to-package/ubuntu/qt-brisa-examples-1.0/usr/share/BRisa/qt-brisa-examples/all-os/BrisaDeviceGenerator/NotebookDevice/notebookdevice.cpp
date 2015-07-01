#include "notebookdevice.h"
#include "powernotebookservice.h"

NotebookDevice::NotebookDevice() :
    BrisaDevice("urn:schemas-upnp-org:device:NoteBook:1", "Brisa NoteBook",
            "Brisa Team. Embedded Laboratory and INdT Brazil",
            "https://garage.maemo.org/projects/brisa", "An UPnP NoteBook",
            "NoteBook", "1.0", "https://garage.maemo.org/projects/brisa",
            "1.0", getCompleteUuid()) {
    this->addService(new PowerNotebookService());

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(reducePower()));

    timer->start(5000);
}

void NotebookDevice::reducePower() {
    int
            level =
                    this->getServiceById("PowerNotebook")->getVariable("Level")->getValue().toInt();
    int charge = this->getServiceById("PowerNotebook")->getVariable(
            "BatteryCharge")->getValue().toInt();

    int newCharge = charge - (level * 10);

    this->getServiceById("PowerNotebook")->getVariable("BatteryCharge")->setAttribute(
            BrisaStateVariable::Value, newCharge >= 0 ? newCharge : 0);
}
