#include "controlpoint.h"

using namespace Brisa;

ControlPoint::ControlPoint(QObject *parent) :
    QObject(parent)
{
    this->cp = new BrisaControlPoint(this);
    connect(this->cp, SIGNAL(deviceFound(BrisaControlPointDevice*)), this, SLOT(newDevice(BrisaControlPointDevice*)));
    connect(this->cp, SIGNAL(deviceGone(QString)), this, SLOT(deleteDevice(QString)));
}

ControlPoint::~ControlPoint()
{
    delete this->cp;
}

void ControlPoint::start()
{
    this->cp->start();
}

void ControlPoint::discover()
{
    this->cp->discover();
}

void ControlPoint::newDevice(BrisaControlPointDevice *device)
{
    if (device->getAttribute(BrisaControlPointDevice::DeviceType).split(":").at(3) == "MediaServer") {
        this->mediaServers.insert(device->getAttribute(BrisaControlPointDevice::Udn), device);
        emit mediaServerFound(device);
        emit mediaServersChanged(this->mediaServers);
    }
}

void ControlPoint::deleteDevice(QString udn)
{
    if (this->mediaServers.contains(udn)) {
        delete this->mediaServers.value(udn);
        this->mediaServers.remove(udn);
        emit mediaServerGone(udn);
        emit mediaServersChanged(this->mediaServers);
    }
}

void ControlPoint::browse(QString udn)
{
    BrisaControlPointDevice *ms = this->mediaServers.value(udn);
}

void ControlPoint::search(QString udn)
{
    BrisaControlPointDevice *ms = this->mediaServers.value(udn);
}

void ControlPoint::parseBrowse(QString result)
{

}

void ControlPoint::parseSearch(QString result)
{

}
