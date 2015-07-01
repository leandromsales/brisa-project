#include "phoneDirectoryControlPoint.h"
#include "ui_phoneDirectoryControlPoint.h"
#include <QMessageBox>
using namespace Brisa;

PhoneDirectoryControlPoint::PhoneDirectoryControlPoint(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PhoneDirectoryControlPoint),
    phoneDirectoryControlPoint(new BrisaControlPoint)
{
    ui->setupUi(this);
    phoneDirectoryControlPoint->start();
    phoneDirectoryControlPoint->discover();
    connect(this->phoneDirectoryControlPoint,SIGNAL(deviceFound(BrisaControlPointDevice*)),this,SLOT(onNewDevice(BrisaControlPointDevice*)));
    connect(this->phoneDirectoryControlPoint,SIGNAL(deviceGone(QString)),this,SLOT(onRemovedDevice(QString)));
   ;

}

PhoneDirectoryControlPoint::~PhoneDirectoryControlPoint()
{
    delete ui;
}

void PhoneDirectoryControlPoint::changeEvent(QEvent *e)
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

void PhoneDirectoryControlPoint::restart(){
    phoneDirectoryControlPoint->stop();
    phoneDirectoryControlPoint->start();
    phoneDirectoryControlPoint->discover();
}

//this slot occours on new device found
void PhoneDirectoryControlPoint::onNewDevice(BrisaControlPointDevice *c){
    //get only phoneDirectoryDevice
    if(c->getAttribute(BrisaControlPointDevice::DeviceType).compare(DEVICE_TYPE))
        return;

    //replace the first device found
    for(int i = 0; i < phoneDirectoryDevices.size(); i++) {
        if(phoneDirectoryDevices[i]->getAttribute(BrisaControlPointDevice::Udn) == c->getAttribute(BrisaControlPointDevice::Udn)) {
            phoneDirectoryDevices[i] = c;
            return;
        }
    }
    //if not found device, add device
    this->phoneDirectoryDevices.append(c);
    this->addPhoneDirectory(c);
}

//this slot add new device into device list when new device found
void PhoneDirectoryControlPoint::addPhoneDirectory(BrisaControlPointDevice* phoneDirectory){
    QString item = phoneDirectory->getAttribute(BrisaControlPointDevice::FriendlyName) + " - "+ phoneDirectory->getAttribute(BrisaControlPointDevice::Udn);
    QListWidgetItem* device = new QListWidgetItem();
    device->setData(Qt::UserRole,QString(phoneDirectory->getAttribute(BrisaControlPointDevice::Udn)));
    device->setText(item);
    qDebug() << device;
    this->ui->li_devices->addItem(device);
}

//this slot remove device from device list if the device leave
void PhoneDirectoryControlPoint::onRemovedDevice(QString udn){
    phoneDirectoryDevices.removeOne(getDeviceByUDN(udn));
    qDebug() << "item removed :" + removeFromList(udn);
    qDebug() << udn;
    restart();
}

QString PhoneDirectoryControlPoint::removeFromList(QString udn){
    qDebug() << "udn to remove:" + udn.split("::")[0];
    int i = 0;
    for(; i < ui->li_devices->count(); i++){
        QListWidgetItem* currentItem = ui->li_devices->item(i);
        QString currentUDN = currentItem->data(Qt::UserRole).toString().section(",",0,0);
        qDebug() << "current Device udn:" + currentUDN;
        if(currentUDN == udn.split("::")[0])
            break;
    }
    qDebug() << ui->li_devices->item(i);
    return ui->li_devices->takeItem(i)->text();
}


//this method returns Device by udn
BrisaControlPointDevice* PhoneDirectoryControlPoint::getDeviceByUDN(QString UDN)
{
    for(int i = 0; i < phoneDirectoryDevices.size(); i++) {
        if(UDN.compare(phoneDirectoryDevices[i]->getAttribute(BrisaControlPointDevice::Udn)) == 0)
            return phoneDirectoryDevices[i];
    }
    return NULL;
}

void PhoneDirectoryControlPoint::_Search(BrisaControlPointDevice *c, QString key){
    BrisaControlPointService *service = c->getServiceByType(SERVICE_TYPE);
    connect(service, SIGNAL(requestFinished(BrisaOutArgument,QString)), this, SLOT(serviceCall(BrisaOutArgument, QString)));
    QMap<QString,QString> params;
    params["newName"] = key;
    service->call("search",params);
}



//this method returns result after operation on Device
void PhoneDirectoryControlPoint::serviceCall(BrisaOutArgument result, QString search){
    Q_UNUSED(search);
    //qDebug() << result;
    if(result["newPhone"] != "")
        ui->edt_Result->setText(result["newPhone"]);
    else{
        QMessageBox::information(this,"Contact Inexisting","Contact not Found!");
        qDebug() << "passed";
    }
    restart();
}

void PhoneDirectoryControlPoint::on_edt_search_returnPressed()
{
    if(ui->li_devices->selectedItems().count() == 1){
        QString key = ui->edt_search->text();
        QString UDN = ui->li_devices->currentItem()->text().split(" - ")[1];
        if(!key.isEmpty() && !UDN.isEmpty()){
            BrisaControlPointDevice *device = getDeviceByUDN(UDN);
            _Search(device,key);
        }
    }
}
