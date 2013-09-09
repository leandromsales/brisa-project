#include "brisadevicexmlhandler.h"
#include "brisadevice.h"

#include <QDebug>

using namespace Brisa;

void BrisaDeviceXMLHandler::xmlGenerator(BrisaDevice *device, QFile *file) {
    file->open(QIODevice::ReadWrite | QIODevice::Text);

    this->writer = new QXmlStreamWriter(file);
    this->writer->setAutoFormatting(true);

    this->writer->writeStartDocument();

    this->writer->writeStartElement("root");
    this->writer->writeAttribute("xmlns", "urn:schemas-upnp-org:device-1-0");

    this->writer->writeStartElement("specVersion");
    this->writer->writeTextElement("major", device->getAttribute(
            BrisaDevice::Major));
    this->writer->writeTextElement("minor", device->getAttribute(
            BrisaDevice::Minor));
    this->writer->writeEndElement(); //specversion

    this->writer->writeTextElement("URLBase", device->getAttribute(
            BrisaDevice::UrlBase));
    this->writeDevice(device);
    this->writer->writeEndElement(); //root

    this->writer->writeEndDocument();

    file->close();
    delete this->writer;
}

void BrisaDeviceXMLHandler::writeDevice(BrisaDevice *device) {
    this->writer->writeStartElement("device");
    this->writer->writeTextElement("deviceType", device->getAttribute(
            BrisaDevice::DeviceType));
    this->writer->writeTextElement("friendlyName", device->getAttribute(
            BrisaDevice::FriendlyName));
    this->writer->writeTextElement("manufacturer", device->getAttribute(
            BrisaDevice::Manufacturer));
    this->writer->writeTextElement("manufacturerURL", device->getAttribute(
            BrisaDevice::ManufacturerUrl));
    this->writer->writeTextElement("modelDescription", device->getAttribute(
            BrisaDevice::ModelDescription));
    this->writer->writeTextElement("modelName", device->getAttribute(
            BrisaDevice::ModelName));
    this->writer->writeTextElement("modelNumber", device->getAttribute(
            BrisaDevice::ModelNumber));
    this->writer->writeTextElement("modelURL", device->getAttribute(
            BrisaDevice::ModelUrl));
    this->writer->writeTextElement("serialNumber", device->getAttribute(
            BrisaDevice::SerialNumber));
    this->writer->writeTextElement("UDN",
            device->getAttribute(BrisaDevice::Udn));
    this->writer->writeTextElement("UPC",
            device->getAttribute(BrisaDevice::Upc));
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
    QList<BrisaService*> serviceList = device->getServiceList();
    foreach(BrisaService *s, serviceList)
        {
            this->writer->writeStartElement("service");
            this->writer->writeTextElement("serviceType", s->getAttribute(
                    BrisaService::ServiceType));
            this->writer->writeTextElement("serviceId", s->getAttribute(
                    BrisaService::ServiceId));
            this->writer->writeTextElement("SCPDURL", s->getAttribute(
                    BrisaService::ScpdUrl));
            this->writer->writeTextElement("controlURL", s->getAttribute(
                    BrisaService::ControlUrl));
            this->writer->writeTextElement("eventSubURL", s->getAttribute(
                    BrisaService::EventSubUrl));
            this->writer->writeEndElement(); //service
        }
    this->writer->writeEndElement(); //serviceList

    this->writer->writeStartElement("deviceList");
    QList<BrisaDevice*> embeddedDeviceList = device->getEmbeddedDeviceList();
    foreach(BrisaDevice *d, embeddedDeviceList)
        {
            writeDevice(d);
        }
    this->writer->writeEndElement(); //deviceList

    this->writer->writeTextElement("presentationURL", device->getAttribute(
            BrisaDevice::PresentationUrl));

    this->writer->writeEndElement(); //device
}

