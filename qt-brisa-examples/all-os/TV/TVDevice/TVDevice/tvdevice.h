#ifndef TVDEVICE_H
#define TVDEVICE_H

#include <QtGui/QMainWindow>
#include <QIcon>
#include <QtDebug>
#include <BrisaUpnp/BrisaAction>
#include <BrisaUpnp/BrisaDevice>
#include <QSplashScreen>

using namespace Brisa;

#define DEVICE_TYPE              "urn:schemas-upnp-org:device:TVDevice:1"
#define DEVICE_FRIENDLY_NAME     "TV Device"
#define DEVICE_MANUFACTURER      "IC - UFAL"
#define DEVICE_MANUFACTURER_URL  "https://garage.maemo.org/projects/brisa"
#define DEVICE_MODEL_DESCRIPTION "An UPnP TV Device"
#define DEVICE_MODEL_NAME        "TV Device"
#define DEVICE_MODEL_NUMBER      "1.0"
#define DEVICE_MODEL_URL         "https://garage.maemo.org/projects/brisa"
#define DEVICE_SERIAL_NUMBER     "1.0"
#define DEVICE_UDN               "uuid:93ff89c4-261e-463d-b79e-d52cb241c171"


namespace Ui
{
    class TVDeviceClass;
}

class TVDevice : public QMainWindow
{
    Q_OBJECT

public:
    QSplashScreen *splash;

    TVDevice(QWidget *parent = 0);
    ~TVDevice();

signals:
    void timeStop();

private:
    Ui::TVDeviceClass *ui;
    BrisaDevice tvDevice;
    BrisaStateVariable *selectedChanel;
    BrisaStateVariable *operation;

    QList<QString> imgChanels;
    QIcon  icon;
    QIcon  icon2;
    QIcon  icon3;

    int currentChanel;
    int contSplashScreen;
    bool on;

    void changeChanel(QString commandControl);
    void offOn();

public slots:
    void updateTV(BrisaStateVariable *result);
    void offOnTV(BrisaStateVariable *result);
    void changeChanel(BrisaStateVariable *result);
    void processSplashScreen();

};

#endif // TVDEVICE_H
