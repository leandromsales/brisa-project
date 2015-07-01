#ifndef PHONEDIRECTORYCONTROLPOINT_H
#define PHONEDIRECTORYCONTROLPOINT_H

#include <QMainWindow>
#include <BRisa/BrisaUpnp/BrisaControlPoint>

#define DEVICE_TYPE  "urn:schemas-upnp-org:device:PhoneDirectoryDevice:1"
#define SERVICE_TYPE "urn:schemas-upnp-org:service:PhoneDirectoryFunctions:1"
#define DEVICE_UDN   "uuid:7f428161-dd0b-48ec-8429-8ac6d2d15e22"

using namespace Brisa;

namespace Ui {
    class PhoneDirectoryControlPoint;
}

class PhoneDirectoryControlPoint : public QMainWindow {
    Q_OBJECT
public:
    PhoneDirectoryControlPoint(QWidget *parent = 0);
    ~PhoneDirectoryControlPoint();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::PhoneDirectoryControlPoint *ui;
    BrisaControlPoint *phoneDirectoryControlPoint;
    QList<BrisaControlPointDevice *> phoneDirectoryDevices;
    void _Search(BrisaControlPointDevice *l,QString name);
    void addPhoneDirectory(BrisaControlPointDevice *PhoneDirectory);
    void restart();
    QString removeFromList(QString udn);
    BrisaControlPointDevice *getDeviceByUDN(QString UDN);

public slots:
    void onNewDevice(BrisaControlPointDevice *c);
    void onRemovedDevice(QString Udn);
    void serviceCall(BrisaOutArgument result, QString operation);


private slots:
    void on_edt_search_returnPressed();
};

#endif // PHONEDIRECTORYCONTROLPOINT_H
