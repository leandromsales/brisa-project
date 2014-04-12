#include "mediarendererdevice.h"
#include "upnp/device/brisadevice.h"
#include "mediarendererdeviceservice.h"

namespace brisa {
namespace upnp {
namespace device {
namespace av {

MediaRendererDevice::MediaRendererDevice(QObject *parent) :
    QObject(parent) {

    this->device = new BrisaDevice("urn:schemas-upnp-org:device:MediaRenderer:1", // deviceType
                                   "UPnPlayer Renderer", // friendlyName
                                   "COMPE. Federal University of Alagoas. Brazil.", // manufacturer
                                   "http://www.upnplayer.com", // manufacturerURL
                                   "A UPnP Media Renderer", // modelDescription
                                   "UPnPlayer Renderer", // modelName
                                   "1.0", // modelNumber
                                   "http://www.upnplayer.com", // modelURL
                                   "0000001", // serialNumber
                                   getCompleteUuid(), // UDN
                                   "", // UPC
                                   "" // presentationURL
                                   );
    MediaRendererDeviceService *service = new MediaRendererDeviceService();
    //service->setDescriptionFile("qrc:///upnp/device/av_device/TVOperations.xml");
    service->setDescriptionFile("/Users/leandrosales/my/projects/repositorio/upnplayer.sym-bug.again/upnp/device/av_device/TVOperations.xml");
    this->device->addService(service);
}

void MediaRendererDevice::start() {
    this->device->start();
}

void MediaRendererDevice::stop() {
    this->device->stop();
}

MediaRendererDevice::~MediaRendererDevice() {
    delete this->device;
}

}  // namespace av
}
}
}
