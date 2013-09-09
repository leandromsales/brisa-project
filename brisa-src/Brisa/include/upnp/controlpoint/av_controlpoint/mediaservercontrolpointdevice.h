#ifndef MEDIASERVERCONTROLPOINTDEVICE_H
#define MEDIASERVERCONTROLPOINTDEVICE_H

#include <QObject>

#include "controlpoint_media_globals.h"
#include "upnp/controlpoint/device.h"

class MediaServerControlPointDevice : public Device
{
    Q_OBJECT
public:
    explicit MediaServerControlPointDevice(QObject *parent = 0);
    explicit MediaServerControlPointDevice(const QString & = QString(""),
                               const QString & = QString(""),
                               QObject *parent = 0);
signals:
    
public slots:
    
};

#endif // MEDIASERVERCONTROLPOINTDEVICE_H
