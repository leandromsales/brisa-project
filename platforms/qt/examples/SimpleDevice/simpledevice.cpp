#include "simpledevice.h"

using namespace brisa::upnp;

SimpleDevice::SimpleDevice() : simpleDevice(DEVICE_TYPE, DEVICE_FRIENDLY_NAME,
                                            DEVICE_MANUFACTURER, DEVICE_MANUFACTURER_URL,
                                            DEVICE_MODEL_DESCRIPTION, DEVICE_MODEL_NAME, DEVICE_MODEL_NUMBER,
                                            DEVICE_MODEL_URL, DEVICE_SERIAL_NUMBER, getCompleteUuid())
{
    SimpleDevice_Ui::setupUi(this);

    statusLabel = SimpleDevice_Ui::setStatusLabel();

    this->layout()->addWidget(statusLabel);

    Functions *functions = new Functions();
    functions->setDescriptionFile(":/src/functions.xml");

    simpleDevice.addService(functions);


    simpleDevice.start();

    this->status = simpleDevice.getServiceByType("urn:schemas-upnp-org:service:Functions:1")->getVariable("Status");
    this->target = simpleDevice.getServiceByType("urn:schemas-upnp-org:service:Functions:1")->getVariable("Target");

    bool x = connect(status,
                     SIGNAL(changed(brisa::upnp::StateVariable *)),
                     this,
                     SLOT(statechanged(brisa::upnp::StateVariable *)));
    Q_ASSERT(x);

    qDebug() << "Connected: " << x;

    this->statechanged(status);

}

void SimpleDevice::statechanged(StateVariable *var)
{
    statusLabel->setText(var->getValue().toString());
}

