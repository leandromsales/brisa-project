#ifndef UPNPCALCULATOR_H
#define UPNPCALCULATOR_H

#include <QMainWindow>
#include <QString>
#include <QIcon>
#include <QtCore>
#include <QCoreApplication>
#include <QtDebug>
#include <QSplashScreen>
#include "ui_calculator.h"
#include "functions.h"
#include "upnp/device/device.h"


#define DEVICE_TYPE              "urn:schemas-upnp-org:device:UPnPCalculator:1"
#define DEVICE_FRIENDLY_NAME     "UPnP Calculator"
#define DEVICE_MANUFACTURER      "Brisa Team. Embedded Laboratory and INdT Brazil"
#define DEVICE_MANUFACTURER_URL  "https://garage.maemo.org/projects/brisa"
#define DEVICE_MODEL_DESCRIPTION "An UPnP Calculator"
#define DEVICE_MODEL_NAME        "UPnP Calculator"
#define DEVICE_MODEL_NUMBER      "1.0"
#define DEVICE_MODEL_URL         "https://garage.maemo.org/projects/brisa"
#define DEVICE_SERIAL_NUMBER     "1.0"
#define DEVICE_UDN               "uuid:367a4fa7-cf68-4cc8-844d-0af4c615cecb"

namespace Ui {
class UpnpCalculator;
}

class UpnpCalculator : public QWidget, public Ui::UpnpCalculatorForm
{
    Q_OBJECT

public:
    explicit UpnpCalculator(QWidget *parent = 0);
    ~UpnpCalculator();
    QSplashScreen *splash;

signals:
   void timeStop();

public slots:
   void statechanged(brisa::upnp::StateVariable *);
   void labelChanged();
   void processSplashScreen();

private:
    Ui::UpnpCalculator *ui;
    brisa::upnp::device::Device upnpCalculator;
    brisa::upnp::StateVariable *status;
    brisa::upnp::StateVariable *target;
    int contSplashScreen;
};

#endif // UPNPCALCULATOR_H
