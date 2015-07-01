#ifndef ARCONTROLPOINT_H
#define ARCONTROLPOINT_H

#include <QMainWindow>
#include <BrisaUpnp/BrisaControlPoint>
#define DEVICE_TYPE  "urn:schemas-upnp-org:device:AirDevice:1"
#define SERVICE_TYPE "urn:schemas-upnp-org:service:AirOperations:1"
#define DEVICE_UDN   "uuid:363a4fa7-cf68-4cc8-844d-0af4c615cecb"

using namespace Brisa;

namespace Ui {
    class AirControlPoint;
}

class AirControlPoint : public QMainWindow {
    Q_OBJECT
public:
    AirControlPoint(QWidget *parent = 0);
    ~AirControlPoint();

protected:
    void changeEvent(QEvent *e);

private:
    bool setTime;
    Ui::AirControlPoint *ui;
    BrisaControlPoint *airControlPoint;
    QList<BrisaControlPointDevice *> airDevices;
    void _ChangeTemperature(BrisaControlPointDevice *l,QString name);
    void addAir(BrisaControlPointDevice *air);
    QString removeFromList(QString udn);
    BrisaControlPointDevice *getDeviceByUDN(QString UDN);
    void restart();
    void _ChangeStatus(BrisaControlPointDevice *c, bool status);
    bool on;

public slots:
    void onNewDevice(BrisaControlPointDevice *c);
    void onRemovedDevice(QString Udn);

private slots:
    void on_pushButton_2_clicked();
    void on_li_devices_clicked(QModelIndex index);
    void on_b_temp_clicked();
    void on_pushButton_clicked();
    void getTargetResponse(BrisaOutArgument arguments, QString method);
    void decrementTime();
};

#endif //ARCONTROLPOINT_H
