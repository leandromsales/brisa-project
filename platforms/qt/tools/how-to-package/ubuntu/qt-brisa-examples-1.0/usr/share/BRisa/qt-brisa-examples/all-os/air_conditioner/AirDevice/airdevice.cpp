#include "airdevice.h"
#include "ui_airdevice.h"
#include "airOperations.h"

AirDevice::AirDevice(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AirDevice),
    airDevice(DEVICE_TYPE, DEVICE_FRIENDLY_NAME,
                DEVICE_MANUFACTURER, DEVICE_MANUFACTURER_URL,
                DEVICE_MODEL_DESCRIPTION, DEVICE_MODEL_NAME, DEVICE_MODEL_NUMBER,
                DEVICE_MODEL_URL, DEVICE_SERIAL_NUMBER, getCompleteUuid())
{
    ui->setupUi(this);
    //preparing device
    AirOperation* airOperations = new AirOperation();
    airOperations->setDescriptionFile("airOperations.xml");
    airDevice.addService(airOperations);
    airDevice.start();

    temp = airDevice.getServiceByType("urn:schemas-upnp-org:service:AirOperations:1")->getVariable("Temp");
    on = airDevice.getServiceByType("urn:schemas-upnp-org:service:AirOperations:1")->getVariable("Status");
    connect(temp,SIGNAL(changed(BrisaStateVariable*)),this,SLOT(change_temp(BrisaStateVariable*)));
    connect(on,SIGNAL(changed(BrisaStateVariable*)),this,SLOT(change_color(BrisaStateVariable*)));
    ui->lcdTemp->display(temp->getValue().toInt());
}

void AirDevice::change_color(BrisaStateVariable* lig){
    if(lig->getValue().toBool()){
       ui->button->setPixmap(QPixmap(":/figs/green.png"));
   }else{
       ui->button->setPixmap(QPixmap(":/figs/red.png"));
   }
}

void AirDevice::change_temp(BrisaStateVariable*  t){
    ui->lcdTemp->display(t->getValue().toInt());
}

AirDevice::~AirDevice()
{
    delete ui;
}

void AirDevice::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

