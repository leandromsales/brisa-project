#include "aircontrolpoint.h"
#include "ui_aircontrolpoint.h"
#include <QMessageBox>
#include <QTimer>

AirControlPoint::AirControlPoint(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AirControlPoint),
    airControlPoint(new BrisaControlPoint)
{
    ui->setupUi(this);

    connect(this->airControlPoint,SIGNAL(deviceFound(BrisaControlPointDevice*)),this,SLOT(onNewDevice(BrisaControlPointDevice*)));
    connect(this->airControlPoint,SIGNAL(deviceGone(QString)),this,SLOT(onRemovedDevice(QString)));
    airControlPoint->start();
    airControlPoint->discover();
    bool setTime = false;

}

AirControlPoint::~AirControlPoint()
{
    delete ui;
}

void AirControlPoint::changeEvent(QEvent *e)
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

void AirControlPoint::restart(){
    airControlPoint->stop();
    airControlPoint->start();
    airControlPoint->discover();
}


void AirControlPoint::onNewDevice(BrisaControlPointDevice *c){

    if(c->getAttribute(BrisaControlPointDevice::DeviceType).compare(DEVICE_TYPE))
        return;


    for(int i = 0; i < airDevices.size(); i++) {
        if(airDevices[i]->getAttribute(BrisaControlPointDevice::Udn) == c->getAttribute(BrisaControlPointDevice::Udn)) {
            airDevices[i] = c;
            return;
        }
    }

    this->airDevices.append(c);
    this->addAir(c);
}

//this slot add new device into device list when new device found
void AirControlPoint::addAir(BrisaControlPointDevice* air){
    QString item = air->getAttribute(BrisaControlPointDevice::FriendlyName) + " - "+ air->getAttribute(BrisaControlPointDevice::Udn);
    QListWidgetItem* device = new QListWidgetItem();
    device->setData(Qt::UserRole,QString(air->getAttribute(BrisaControlPointDevice::Udn)));
    device->setText(item);
    qDebug() << device;
    this->ui->li_devices->addItem(device);
}

//this slot remove device from device list if the device leave
void AirControlPoint::onRemovedDevice(QString udn){
    airDevices.removeOne(getDeviceByUDN(udn));
    qDebug() << "item removed :" + removeFromList(udn);
    qDebug() << udn;
    restart();
}

QString AirControlPoint::removeFromList(QString udn){
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

BrisaControlPointDevice* AirControlPoint::getDeviceByUDN(QString UDN)
{
    for(int i = 0; i < airDevices.size(); i++) {
        if(UDN.compare(airDevices[i]->getAttribute(BrisaControlPointDevice::Udn)) == 0)
            return airDevices[i];
    }
    return NULL;
}

void AirControlPoint::_ChangeTemperature(BrisaControlPointDevice *c, QString temperature){
    BrisaControlPointService *service = c->getServiceByType(SERVICE_TYPE);
    QMap<QString,QString> temp;
    qDebug() << "before Getting target";
    temp["NewTargetValue"] = temperature;
    qDebug() << "Getting target";
    service->call("setTemp",temp);
}

void AirControlPoint::_ChangeStatus(BrisaControlPointDevice *c, bool status){
    BrisaControlPointService *service = c->getServiceByType(SERVICE_TYPE);
    QMap<QString,QString> temp;
    qDebug() << " before change";
    temp["NewTargetValue"] = QString((!status ? "1":"0") );
    qDebug() << "after change";
    service->call("setStatus",temp);
    qDebug() << "after service call in change status";

}

void AirControlPoint::on_pushButton_clicked()
{

    if(ui->li_devices->selectedItems().count() == 1){
        QString deviceSelected = ui->li_devices->currentItem()->text().split(" - ")[1];
        if(!deviceSelected.isEmpty()){
            BrisaControlPointDevice *device = getDeviceByUDN(deviceSelected);
            _ChangeStatus(device,on);

            if( ui->timeEdit->time() == QTime(0,0,0,0) )
                setTime = false;


            if(ui->timeEdit->time() != QTime(0,0,0,0) && !setTime)
            {
                setTime = true;
                qDebug() << ((ui->timeEdit->time().hour() * 360000) + (ui->timeEdit->time().minute()*60000) + (ui->timeEdit->time().second()*1000));
                QTimer::singleShot((ui->timeEdit->time().hour() * 360000) + (ui->timeEdit->time().minute()*60000) + (ui->timeEdit->time().second()*1000),this,SLOT(on_pushButton_clicked()));
                decrementTime();

            }
        }
    }
    if(on)
    {
       qDebug() << "set status off";
       ui->pushButton->setIcon(QIcon(":/figs/red.jpg"));
       on = false;
    }
    else
    {
        qDebug() << "set status on";
        ui->pushButton->setIcon(QIcon(":/figs/green.jpg"));
        on = true;
    }
}

void AirControlPoint::on_b_temp_clicked()
{
    if(ui->li_devices->selectedItems().count() == 1){
        QString temperature = ui->insert_temp->text();
        QString deviceSelected = ui->li_devices->currentItem()->text().split(" - ")[1];
        if(!temperature.isEmpty() && !deviceSelected.isEmpty()){
            BrisaControlPointDevice *device = getDeviceByUDN(deviceSelected);
            _ChangeTemperature(device,temperature);
        }
    }
}

void AirControlPoint::on_li_devices_clicked(QModelIndex index)
{
    if(ui->li_devices->selectedItems().count() == 1){
        QString deviceSelected = ui->li_devices->currentItem()->text().split(" - ")[1];
        qDebug() << deviceSelected + "device";
        if(!deviceSelected.isEmpty()){
            BrisaControlPointDevice *device = getDeviceByUDN(deviceSelected);
            BrisaControlPointService *service = device->getServiceByType(SERVICE_TYPE);
            qDebug() << SERVICE_TYPE;
            QMap<QString,QString> temp;
            qDebug() << "before connect";
            connect(service, SIGNAL(requestFinished(BrisaOutArgument, QString)), this, SLOT(getTargetResponse(BrisaOutArgument,QString)));
            qDebug() << "after connect";
            service->call("getTemp",temp);
            qDebug() << "after service call";
        }
    }
}

void AirControlPoint::getTargetResponse(BrisaOutArgument arguments, QString method)
{


    if(method == "getTemp") {


         //response.remove(0,15);

         ui->insert_temp->setText(arguments["RetTempValue"]);
     }
     if(method == "getStatus") {
     //RetTempValue = 2

          if(arguments["ResultStatus"] == "0")
          {
              ui->pushButton->setIcon(QIcon(":/figs/red.jpg"));
             on = false;
          }
          else
          {
              ui->pushButton->setIcon(QIcon(":/figs/green.jpg"));
              on = true;
          }
      }
}

void AirControlPoint::on_pushButton_2_clicked()
{
    if(ui->li_devices->selectedItems().count() == 1){
        QString deviceSelected = ui->li_devices->currentItem()->text().split(" - ")[1];
        if(!deviceSelected.isEmpty()){
            BrisaControlPointDevice *device = getDeviceByUDN(deviceSelected);
            BrisaControlPointService *service = device->getServiceByType(SERVICE_TYPE);
            QMap<QString,QString> status;
            connect(service, SIGNAL(requestFinished(QString, QString)), this, SLOT(getTargetResponse(QString,QString)));
            service->call("getStatus",status);
        }
    }
}

void AirControlPoint::decrementTime(){
    if(ui->timeEdit->time() != QTime(0,0,0,0)){
        qDebug() << "decremented";
        ui->timeEdit->setTime(ui->timeEdit->time().addSecs(-1));
        QTimer::singleShot(1000,this,SLOT(decrementTime()));      
    }
}
