#ifndef LIGHT_H
#define LIGHT_H

#include <QWidget>
#include <QString>
#include <QIcon>
#include <QtCore>
#include <QCoreApplication>
#include <QtDebug>
#include "upnp/device/Device"
#include <QSplashScreen>
#include "ui_light.h"
#include "switchPower.h"


#define DEVICE_TYPE              "urn:schemas-upnp-org:device:BinaryLight:1"
#define DEVICE_FRIENDLY_NAME     "Binary Light Device"
#define DEVICE_MANUFACTURER      "Brisa Team. Embedded Laboratory and INdT Brazil"
#define DEVICE_MANUFACTURER_URL  "https://garage.maemo.org/projects/brisa"
#define DEVICE_MODEL_DESCRIPTION "An UPnP Binary Light Device"
#define DEVICE_MODEL_NAME        "Binary Light Device"
#define DEVICE_MODEL_NUMBER      "1.0"
#define DEVICE_MODEL_URL         "https://garage.maemo.org/projects/brisa"
#define DEVICE_SERIAL_NUMBER     "1.0"
#define DEVICE_UDN               "uuid:367a4fa7-cf68-4cc8-844d-0af4c615cecb"

namespace Ui
{
    class Widget;
}

class Widget : public QWidget , public Ui::BinaryLightForm
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
     QSplashScreen *splash;

signals:
    void timeStop();

public slots:
    void on_pushButton1_toggled();
    void lampclicked();
    void statechanged(brisa::upnp::StateVariable *);
    void processSplashScreen();

private:
    QIcon  icon1;
    QIcon  icon2;
    brisa::upnp::device::Device binaryLight;
    brisa::upnp::StateVariable *status;
    brisa::upnp::StateVariable *target;
    int contSplashScreen;
    
};

#endif // LIGHT_H
