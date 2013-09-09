#ifndef MEDIARENDERERCONTROLPOINTDEVICE_H
#define MEDIARENDERERCONTROLPOINTDEVICE_H

#include <QObject>

#include "controlpoint_media_globals.h"
#include "upnp/controlpoint/device.h"

class MediaRendererControlPointDevice : public Device
{
    Q_OBJECT
public:
    explicit MediaRendererControlPointDevice(QObject *parent = 0);
    explicit MediaRendererControlPointDevice(const QString & = QString(""),
                                 const QString & = QString(""),
                                 QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // MEDIARENDERERCONTROLPOINTDEVICE_H
