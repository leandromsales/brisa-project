#ifndef AIRDEVICE_H
#define AIRDEVICE_H

#include <QMainWindow>
#include <BrisaUpnp/BrisaAction>
#include <BrisaUpnp/BrisaDevice>


using namespace Brisa;


#define DEVICE_TYPE              "urn:schemas-upnp-org:device:AirDevice:1"
#define DEVICE_FRIENDLY_NAME     "Air Device"
#define DEVICE_MANUFACTURER      "Babylon and Associate"
#define DEVICE_MANUFACTURER_URL  "https://garage.maemo.org/projects/brisa"
#define DEVICE_MODEL_DESCRIPTION "An UPnP Air Device"
#define DEVICE_MODEL_NAME        "Air Device"
#define DEVICE_MODEL_NUMBER      "1.0"
#define DEVICE_MODEL_URL         "https://garage.maemo.org/projects/brisa"
#define DEVICE_SERIAL_NUMBER     "1.0"
#define DEVICE_UDN               "uuid:363a4fa7-cf68-4cc8-844d-0af4c615cecb"


namespace Ui {
    class AirDevice;
}

class AirDevice : public QMainWindow {
    Q_OBJECT
public:
    AirDevice(QWidget *parent = 0);
    ~AirDevice();



protected:
    void changeEvent(QEvent *e);

private:
    Ui::AirDevice *ui;
    BrisaDevice airDevice;


    BrisaStateVariable *on;
    BrisaStateVariable *temp;
signals:

public slots:

    void change_color(BrisaStateVariable *);
    void change_temp(BrisaStateVariable *);

};

#endif // AIRDEVICE_H
