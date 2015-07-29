#ifndef CONTROLPOINT_H
#define CONTROLPOINT_H

#define DEVICES_PATH "Devices"
#define ACTION_EXT ".action"
#define RETURN_EXT ".return"
#define DESCRIPTION_EXT ".description"
#define NO_ARGS_REQUIRED "NO_ARGS_REQUIRED"
#define EVENTS_PATH "Events"
#define EVENTS_FILE_NAME "events"
#define MULTICAST_FILE_NAME "multicast_events"
#define EVENT_EXT ".event"
#define EVENTLOG_EXT ".eventLog"
#define SUBSCRIBE_LINE "subscribe"
#define UNSUBSCRIBE_LINE "unsubscribe"

#include <BRisa/BrisaUpnp/BrisaControlPoint>
#include <QListWidgetItem>

#include "filemanager.h"
#include "../common/logger.h"

using namespace Brisa;

class ControlPoint:public QObject
{
    Q_OBJECT

public:
    explicit ControlPoint(QDir *root, bool onOpen = true, QObject *parent = 0);
    ~ControlPoint();

    void start();
    void restart();

    void subscribe(BrisaControlPointService *service, int timeout = -1);
    void unsubscribe(BrisaControlPointService *service);

private:

    const char *c_str(QString&) const;

    QDir *rootDir;
    bool invokeOnOpen; //onOpen
    BrisaControlPoint *controlPoint;
    QList<BrisaControlPointDevice *> devices;
    QMap<QString, QString> fileActionMap;
    QMap<int, QList<QString> > eventToDevice;

    void addDeviceFolders(BrisaControlPointDevice *d);
    void removeDeviceFolders(QString deviceName);
    void deleteDir(QDir &dir);

    void createEventsFiles(BrisaControlPointService *service, QDir *cDir, QTextStream &outToDesc);

    QStringList getActions(BrisaControlPointService *s);
    QStringList getServices(BrisaControlPointDevice *c);
    BrisaControlPointDevice* getDeviceByUDN(QString UDN);
    BrisaControlPointDevice* getDeviceByName(QString UDN);

    bool isValidDeviceFile(const QString &filepath);
    QString getTime();

public slots:
    void onNewDevice(BrisaControlPointDevice *c);
    void onRemovedDevice(QString Udn);
    void multicastEventReceived(QString variable, QString newValue);
    void multicasteventReceivedRaw(BrisaOutArgument raw);

    void unicastEventReceived(BrisaEventProxy *subscription,
                                            QMap<QString, QString> variables);

private slots:
    void serviceCall(BrisaOutArgument arguments, QString method);
    void requestError(QString errorMessage, QString methodName);    

public Q_SLOTS:
    void fileWrite(const QString &filepath);
    void fileOpen(const QString &filepath);
    void fsUmounted();


};


#endif // CONTROLPOINT_H
