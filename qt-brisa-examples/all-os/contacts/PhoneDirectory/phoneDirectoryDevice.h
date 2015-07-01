#ifndef PHONEDIRECTORYDEVICE_H
#define PHONEDIRECTORYDEVICE_H

#include <QMainWindow>
#include <BrisaUpnp/BrisaAction>
#include <BrisaUpnp/BrisaDevice>

using namespace Brisa;

#define DEVICE_TYPE              "urn:schemas-upnp-org:device:PhoneDirectoryDevice:1"
#define DEVICE_FRIENDLY_NAME     "Phone Directory Device"
#define DEVICE_MANUFACTURER      "Brisa Team. Compe Laboratory at UFAL"
#define DEVICE_MANUFACTURER_URL  "https://garage.maemo.org/projects/brisa"
#define DEVICE_MODEL_DESCRIPTION "An UPnP Phone Directory Device"
#define DEVICE_MODEL_NAME        "Phone Directory Device"
#define DEVICE_MODEL_phone      "1.0"
#define DEVICE_MODEL_URL         "https://garage.maemo.org/projects/brisa"
#define DEVICE_SERIAL_phone     "1.0"
#define DEVICE_UDN               "uuid:7f428161-dd0b-48ec-8429-8ac6d2d15e22"


namespace Ui {
    class PhoneDirectoryDevice;
}

class PhoneDirectoryDevice : public QMainWindow {
    Q_OBJECT
public:
    PhoneDirectoryDevice(QWidget *parent = 0);
    ~PhoneDirectoryDevice();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::PhoneDirectoryDevice *ui;
    QMap<QString, QString> items;
    BrisaDevice phoneDirectoryDevice;
    BrisaStateVariable *phone;
    BrisaStateVariable *name;
    QDir dir;
    QString search(QString key);
    void list();
    void updateFile(QString key, QString value);
    void updateTableView();
    void removeFromFile();
private slots:
    void on_actionAdd_User_triggered();
    void on_actionRemove_User_triggered();
    void on_edt_search_returnPressed();
public slots:
    void updateName(BrisaStateVariable *name);

};

#endif // PHONEDIRECTORYDEVICE_H
