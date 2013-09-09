#include "mediarenderercontrolpointdevice.h"

MediaRendererControlPointDevice::MediaRendererControlPointDevice(QObject *parent) : Device(parent) { }

MediaRendererControlPointDevice::MediaRendererControlPointDevice(const QString &uuid, const QString &location, QObject *parent) :
    Device(uuid, location, parent) { }
