#include "brisadevicexmlhandler.h"
#include "brisadevice.h"

#include <QDebug>

namespace brisa {
namespace upnp {
namespace device {

void DeviceXMLHandler::xmlGenerator(Device *device, QFile *file) {
    file->open(QIODevice::ReadWrite | QIODevice::Text);

    this->writer = new QXmlStreamWriter(file);
    this->writer->setAutoFormatting(true);

    this->writer->writeStartDocument();

    this->writer->writeStartElement("root");
    this->writer->writeAttribute("xmlns", "urn:schemas-upnp-org:device-1-0");

    this->writer->writeStartElement("specVersion");
    this->writer->writeTextElement("major", device->getAttribute(
            Device::Major));
    this->writer->writeTextElement("minor", device->getAttribute(
            Device::Minor));
    this->writer->writeEndElement(); //specversion

    this->writer->writeTextElement("URLBase", device->getAttribute(
            Device::UrlBase));
    this->writeDevice(device);
    this->writer->writeEndElement(); //root

    this->writer->writeEndDocument();

    file->close();
    delete this->writer;
}

void DeviceXMLHandler::writeDevice(Device *device) {
    this->writer->writeStartElement("device");
    this->writer->writeTextElement("deviceType", device->getAttribute(
            Device::DeviceType));
    this->writer->writeTextElement("friendlyName", device->getAttribute(
            Device::FriendlyName));
    this->writer->writeTextElement("manufacturer", device->getAttribute(
            Device::Manufacturer));
    this->writer->writeTextElement("manufacturerURL", device->getAttribute(
            Device::ManufacturerUrl));
    this->writer->writeTextElement("modelDescription", device->getAttribute(
            Device::ModelDescription));
    this->writer->writeTextElement("modelName", device->getAttribute(
            Device::ModelName));
    this->writer->writeTextElement("modelNumber", device->getAttribute(
            Device::ModelNumber));
    this->writer->writeTextElement("modelURL", device->getAttribute(
            Device::ModelUrl));
    this->writer->writeTextElement("serialNumber", device->getAttribute(
            Device::SerialNumber));
    this->writer->writeTextElement("UDN",
            device->getAttribute(Device::Udn));
    this->writer->writeTextElement("UPC",
            device->getAttribute(Device::Upc));
    this->writer->writeStartElement("iconList");
    QList<BrisaIcon*> iconList = device->getIconList();
    foreach(BrisaIcon *i, iconList)
        {
            this->writer->writeStartElement("icon");
            this->writer->writeTextElement("mimetype", i->getAttribute(
                    BrisaIcon::Mimetype));
            this->writer->writeTextElement("width", i->getAttribute(
                    BrisaIcon::Width));
            this->writer->writeTextElement("height", i->getAttribute(
                    BrisaIcon::Height));
            this->writer->writeTextElement("depth", i->getAttribute(
                    BrisaIcon::Depth));
            this->writer->writeTextElement("url",
                    i->getAttribute(BrisaIcon::Url));
            this->writer->writeEndElement(); //icon
        }
    this->writer->writeEndElement(); //iconList

    this->writer->writeStartElement("serviceList");
    QList<Service*> serviceList = device->getServiceList();
    foreach(Service *s, serviceList)
        {
            this->writer->writeStartElement("service");
            this->writer->writeTextElement("serviceType", s->getAttribute(
                    Service::ServiceType));
            this->writer->writeTextElement("serviceId", s->getAttribute(
                    Service::ServiceId));
            this->writer->writeTextElement("SCPDURL", s->getAttribute(
                    Service::ScpdUrl));
            this->writer->writeTextElement("controlURL", s->getAttribute(
                    Service::ControlUrl));
            this->writer->writeTextElement("eventSubURL", s->getAttribute(
                    Service::EventSubUrl));
            this->writer->writeEndElement(); //service
        }
    this->writer->writeEndElement(); //serviceList

    this->writer->writeStartElement("deviceList");
    QList<Device*> embeddedDeviceList = device->getEmbeddedDeviceList();
    foreach(Device *d, embeddedDeviceList)
        {
            writeDevice(d);
        }
    this->writer->writeEndElement(); //deviceList

    this->writer->writeTextElement("presentationURL", device->getAttribute(
            Device::PresentationUrl));

    this->writer->writeEndElement(); //device
}

}
}
}
