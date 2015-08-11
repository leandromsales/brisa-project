#include "switcher.h"

using namespace std;

#define BINARY_LIGHT_DEVICE_TYPE "urn:schemas-upnp-org:device:BinaryLight:1"
#define DIMMABLE_LIGHT_DEVICE_TYPE "urn:schemas-upnp-org:device:DimmableLight:1"
#define SWITCH_POWER_SERVICE_TYPE "urn:schemas-upnp-org:service:SwitchPower:1"


LightSwitcher::LightSwitcher(QObject *parent) : QObject(parent) {
    controlPoint = new BrisaControlPoint();

    connect(controlPoint,
            SIGNAL(deviceFound(BrisaControlPointDevice*)),
            this,
            SLOT(deviceFound(BrisaControlPointDevice*)));

}

LightSwitcher::~LightSwitcher() {
    delete controlPoint;
}

void LightSwitcher::start() {
    controlPoint->start();
}

void LightSwitcher::deviceFound(BrisaControlPointDevice *device) {
    // Check if it is a BinaryLight or a DimmableLight
    if (device->getAttribute(BrisaControlPointDevice::DeviceType).compare(BINARY_LIGHT_DEVICE_TYPE) &&
        device->getAttribute(BrisaControlPointDevice::DeviceType).compare(DIMMABLE_LIGHT_DEVICE_TYPE))
        return;

    // Dump device information
    dumpDeviceInfo(device);

    // Find SwitchPower service
    BrisaControlPointService* service = device->getServiceByType(SWITCH_POWER_SERVICE_TYPE);

    if (!service) {
        qWarning() << "SwichPower service not found.";
        return;
    }

    // Perform a GetStatus remote call. The response is received on the
    // serviceCall slot.
    dev = device;
    QObject::connect(service, SIGNAL(requestFinished(QString, QString)), this, SLOT(serviceCall(QString, QString)));

    QMap<QString, QString> b;
    service->call("GetStatus", b);

    // Send a test subscription
    BrisaEventProxy *subscription = controlPoint->getSubscriptionProxy(service);

    connect(subscription,
            SIGNAL(eventNotification(BrisaEventProxy*,
                                     QMap<QString, QString>)),
            this,
            SLOT(lightStatusNotification(BrisaEventProxy*,
                                         QMap<QString, QString>)));

    qDebug() << "Subscribing to events...";
    subscription->subscribe(60);
}

void LightSwitcher::lightStatusNotification(BrisaEventProxy *subscription,
                                            QMap<QString, QString> eventingVariables)
{
    qDebug() << "Event on subscription " << subscription->getSid();

    foreach (QString key, eventingVariables.keys()) {
        qDebug() << "Variable: " << key << eventingVariables.value(key);
    }
}

void LightSwitcher::serviceCall(QString root, QString method)
{
    if (method == "GetStatus" || method == "GetTarget") {
        BrisaControlPointService* service = dev->getServiceByType("urn:schemas-upnp-org:service:SwitchPower:1");
        QMap<QString, QString> a;

        if (!root.compare("0")) {
            qDebug()<<"Binary light status is off";
            a["NewTargetValue"] = "1";
        } else {
            qDebug()<<"Binary light status is on";
            a["NewTargetValue"] = "0";
        }

        service->call("SetTarget", a);
    }
}

void LightSwitcher::dumpDeviceInfo(BrisaControlPointDevice *device)
{
    qDebug() << "Binary Light found!";
    qDebug() << "Friendly name: " << device->getAttribute(BrisaControlPointDevice::FriendlyName);
    qDebug() << "Manufacturer: " << device->getAttribute(BrisaControlPointDevice::Manufacturer);
    qDebug() << "Manufacturer URL: " << device->getAttribute(BrisaControlPointDevice::ManufacturerUrl);
    qDebug() << "Model description: " << device->getAttribute(BrisaControlPointDevice::ModelName);
    qDebug() << "Model number: " << device->getAttribute(BrisaControlPointDevice::ModelNumber);
    qDebug() << "Model URL: " << device->getAttribute(BrisaControlPointDevice::ModelUrl);
    qDebug() << "Serial number: " << device->getAttribute(BrisaControlPointDevice::SerialNumber);
    qDebug() << "UDN: " << device->getAttribute(BrisaControlPointDevice::Udn);
    qDebug() << "UPC: " << device->getAttribute(BrisaControlPointDevice::Upc);
    qDebug() << "Presentation URL: " << device->getAttribute(BrisaControlPointDevice::PresentationUrl);
}
