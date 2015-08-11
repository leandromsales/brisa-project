#include "lightSwitcher.h"
#include "ui_lightSwitcher.h"

LightSwitcher::LightSwitcher(QMainWindow *parent)
    : QMainWindow(parent)
{
    setupUi(this);
    contSplashScreen= 0;
    splash = new QSplashScreen(QPixmap("../../pixmaps/brisa/BrisaSplashScreenLightSwitcher.png"));

    controlPoint = new BrisaControlPoint();
    connect(controlPoint, SIGNAL(deviceGone(QString)), this, SLOT(removeDevice(QString)));
    connect(controlPoint, SIGNAL(deviceFound(BrisaControlPointDevice*))
            , this, SLOT(deviceFoundDump(BrisaControlPointDevice*)));

    connect(getTargetButton, SIGNAL(clicked()), this, SLOT(getTargetAction()));
    connect(getStatusButton, SIGNAL(clicked()), this, SLOT(getStatusAction()));
    connect(setTargetButton, SIGNAL(clicked()), this, SLOT(setTargetAction()));

    controlPoint->start();
    controlPoint->discover();
}

LightSwitcher::~LightSwitcher()
{
    controlPoint->stop();
    delete controlPoint;
}

void LightSwitcher::deviceFoundDump(BrisaControlPointDevice *device)
{
    if(device->getAttribute(BrisaControlPointDevice::DeviceType) ==
            "urn:schemas-upnp-org:device:BinaryLight:1") {
        bool add = true;
        for(int i = 0; i < devicesList.size(); i++) {
            if(devicesList[i]->getAttribute(BrisaControlPointDevice::Udn) ==
                    device->getAttribute(BrisaControlPointDevice::Udn))
                add = false;
        }
        if(add) {
            devicesList.append(device);
            comboBox->addItem(device->getAttribute(BrisaControlPointDevice::Udn));
        }
    }
}

BrisaControlPointDevice* LightSwitcher::getDeviceByUdn(QString udn)
{
    foreach(BrisaControlPointDevice *i, this->devicesList) {
        if (i->getAttribute(BrisaControlPointDevice::Udn) == udn)
            return i;
    }
    return NULL;
}

void LightSwitcher::removeDevice(QString udn)
{
    for(int i = 0; i < comboBox->count(); i++) {
        if(comboBox->itemText(i) == udn)
            comboBox->removeItem(i);
    }
    foreach(BrisaControlPointDevice *dev, devicesList){
        if(dev->getAttribute(BrisaControlPointDevice::Udn) == udn) {
            devicesList.removeAll(dev);
        }
    }
}

void LightSwitcher::getTargetAction()
{
    BrisaControlPointDevice *device = getDeviceByUdn(comboBox->currentText());
    if(device) {
        BrisaControlPointService *service = device->getServiceByType("urn:schemas-upnp-org:service:SwitchPower:1");
        QMap<QString, QString> param;
        connect(service, SIGNAL(requestFinished(QString, QString)), this, SLOT(getTargetResponse(QString, QString)));
        service->call("GetTarget", param);
    }
}

void LightSwitcher::getTargetResponse(QString response, QString method)
{
    if(method == "GetTarget") {
        if (response == QString("1")) {
            targetLabel->setPixmap(QPixmap(QString::fromUtf8("../../pixmaps/brisa/onlight1.png")));
        } else if (response == QString("0")) {
            targetLabel->setPixmap(QPixmap(QString::fromUtf8("../../pixmaps/brisa/offlight1.png")));
        } else {
            targetLabel->setPixmap(QPixmap(""));
        }
    }
}

void LightSwitcher::getStatusAction()
{
    BrisaControlPointDevice *device = getDeviceByUdn(comboBox->currentText());
    if(device) {
        BrisaControlPointService *service = device->getServiceByType("urn:schemas-upnp-org:service:SwitchPower:1");
        QMap<QString, QString> param;
        connect(service, SIGNAL(requestFinished(QString, QString)), this, SLOT(getStatusResponse(QString, QString)));
        service->call("GetStatus", param);
    }
}

void LightSwitcher::getStatusResponse(QString response, QString method)
{
    if(method == "GetStatus") {
        if (response == "1") {
            statusLabel->setPixmap(QPixmap(QString::fromUtf8("../../pixmaps/brisa/onlight1.png")));
        } else if (response == "0") {
            statusLabel->setPixmap(QPixmap(QString::fromUtf8("../../pixmaps/brisa/offlight1.png")));
        } else {
            statusLabel->setPixmap(QPixmap(""));
        }
    }
}

void LightSwitcher::setTargetAction()
{
    BrisaControlPointDevice *device = getDeviceByUdn(comboBox->currentText());
    if(device) {
        BrisaControlPointService *service = device->getServiceByType("urn:schemas-upnp-org:service:SwitchPower:1");
        QMap<QString, QString> param;
        if(checkBox->checkState() == Qt::Checked) {
            param["NewTargetValue"] = "1";
        } else {
            param["NewTargetValue"] = "0";
        }
        service->call("SetTarget", param);
    }
}
void LightSwitcher::processSplashScreen()
{
    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;

    switch (contSplashScreen) {
    case 1:
        splash->showMessage(QObject::tr("Setting up Brisa Configuration..."),topRight, Qt::blue);
        break;
    case 2:
        splash->showMessage(QObject::tr("Creating Brisa Configuration..."),topRight, Qt::blue);
        break;
    }

    if (contSplashScreen > 2) {
        this->show();
        splash->finish(this);
        emit timeStop();
        delete splash;
    }
    contSplashScreen++;
}
