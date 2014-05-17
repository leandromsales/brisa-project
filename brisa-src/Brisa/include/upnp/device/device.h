#ifndef DEVICE_H
#define DEVICE_H

#include <QXmlDefaultHandler>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QtDebug>

#include "network/brisanetwork.h"
#include "shared/webserver/webserver.h"
#include "shared/ssdp/brisassdpserver.h"
#include "upnp/device/devicexmlhandler.h"
#include "upnp/device/service.h"
#include "upnp/brisaicon.h"

namespace brisa {
//class shared::ssdp::BrisaSSDPServer;
namespace upnp {
//class Webserver;
namespace device {

/*!
 *  \class Brisa::BrisaDevice brisadevice.h BrisaUpnp/BrisaDevice
 *  \brief UPnP device implementation
 *
 *  BrisaDevice provides a easy and fast way to create UPnP devices. Simply create a new BrisaDevice
 *  and call start() to join the network and be visible to available control points.
 *
 *  To add a service to the device, just create a new BrisaService with the chosen actions and
 *  events and add it to the device by calling \a addService(). The service will be automatically
 *  added to the device and the appropriate webserver urls paths will be created.
 *
 *  Embedded devices are also supported by BrisaDevice. Create a new BrisaDevice and call
 *  \a addEmbeddedDevice(), the embedded device will be announced when the root device joins the
 *  network.
 *
 *  To stop the device and leave the network, simply call the \a stop() method, ssdp messages will
 *  also be sent for any embedded devices.
 *
 *  \sa Brisa::BrisaService
 */
class Device: public QObject {
Q_OBJECT

public:
    /*!
     *  Creates a BrisaDevice with the given parent QObject.
     */
    Device(QObject *parent = 0);

    /*!
     *  Copy constructor.
     */
    Device(const Device &dev);

    /*!
     *  Creates a BrisaDevice with the given device information.
     */
    Device(const QString &deviceType, const QString &friendlyName = "",
            const QString &manufacturer = "", const QString &manufacturerURL = "",
	    const QString &modelDescription = "", const QString &modelName = "",
	    const QString &modelNumber = "", const QString &modelURL = "",
	    const QString &serialNumber = "", const QString &UDN = "",
	    const QString &UPC = "", const QString &presentationURL = "",
	    QObject *parent = 0);

    /*!
     *  Destructor for BrisaDevice.
     *  Stops the device if running.
     */
    virtual ~Device();

    /*!
     *  Assigns dev to this BrisaDevice and returns a copy.
     */
    Device &operator=(const Device &dev);

    typedef enum {
        Major,
        Minor,
        UrlBase,
        DeviceType,
        FriendlyName,
        Manufacturer,
        ManufacturerUrl,
        ModelDescription,
        ModelName,
        ModelNumber,
        ModelUrl,
        SerialNumber,
        Udn,
        Upc,
        PresentationUrl,
        FileAddress,
        Location,
        Server,
        IpAddress,
        Port,
        Running
    } xmlTags;

    /*!
     *  Attribute setter.
     *  \sa getAttribute()
     */
    void setAttribute(xmlTags key, const QString &value);

    /*!
     *  Attribute getter.
     *  \sa setAttribute()
     */
    QString getAttribute(xmlTags key) const;

    /*!
     *  Call this method to add a icon to the device.
     */
    void addIcon(const QString &mimetype = "", const QString &width = "",
            const QString &height = "", const QString &depth = "",
            const QString &url = "");

    /*!
     *  Creates and adds a service to the device with the given information.
     */
    void addService(const QString &serviceType = "", const QString &serviceId =
            "", const QString &SCPDURL = "", const QString &controlURL = "",
            const QString &eventSubURL = "");

    /*!
     *  Overloads addService().
     *  Create a \a BrisaService and add it to the device. We recommend using this method for better
     *  object orientation.
     *  \sa Brisa::BrisaService
     */
    void addService(Service *serv);

    /*!
     *  Creates and adds a embedded device with the given information to the device.
     */
    void addEmbeddedDevice(const QString &deviceType = "",
            const QString &friendlyName = "", const QString &manufacturer = "",
            const QString &manufacturerURL = "",
            const QString &modelDescription = "",
            const QString &modelName = "", const QString &modelNumber = "",
            const QString &modelURL = "", const QString &serialNumber = "",
            const QString &UDN = "", const QString &UPC = "",
            const QString &presentationURL = "");

    /*!
     *  Overloads addEmbeddedDevice()
     *  Create a new \a BrisaDevice and call this method to add it as a embedded device to a root
     *  device. We recommend using this method for better object orientation.
     */
    void addEmbeddedDevice(Device *newEmbeddedDevice);

    /*!
     *  Returns the icon list.
     *
     *  \sa getEmbeddedDeviceList() , getServiceList()
     */
    QList<BrisaIcon *> getIconList() const;

    /*!
     *  Returns the service list.
     *
     *  \sa getEmbeddedDeviceList() , getIconList()
     */
    QList<Service *> getServiceList() const;

    /*!
     *  Returns the embedded device list.
     *
     *  \sa getIconList() , getServiceList()
     */
    QList<Device *> getEmbeddedDeviceList() const;

    /*!
     *  Getter for BrisaService in the service list.
     */
    Service *getServiceById(const QString &serviceId);

    /*!
     *  Getter for BrisaService in the service list.
     */
    Service *getServiceByType(const QString &serviceType);

    /*!
     *  Clears the device information, including services, icons and embedded devices.
     */
    void clear();

    /*!
     *  Sends the ssdp:alive messages for root device, embedded devices and services according to the
     *  UPnP 1.0 especification.
     */
    void doNotify();

    /*!
     *  Sends the ssdp:byebye messages for root device, embedded devices and services according to the
     *  UPnP 1.0 especification.
     */
    void doByeBye();

    /*!
     *  Call this method to join the network and start the device.
     *
     *  \sa stop()
     */
    void start();

    /*!
     *  Stops the device and leaves the network.
     *
     *  \sa start()
     */
    void stop();

    int threadsNumber() const;
    void setThreadsNumber(int);

    QByteArray addWebservice(QByteArray pathSuffix, brisa::shared::webserver::WebService *service);
    void removeWebservice(const QByteArray &path);

public slots:
    /*!
     *  Connects to the msearchRequestReceived() signal comming from the ssdp module. It parses
     *  the search type and responds accordingly.
     *
     *  \sa respondMSearchAll()
     */
    void respondMSearch(const QString &st, const QString &senderIp,
            quint16 senderPort);

private:
    /*!
     *  Creates a temporary XML file containning the device information.
     *  \sa deviceGenerator()
     */
    void xmlGenerator();

    /*!
     *  Creates the UrlBase attribute according to local IP address and a random port.
     */
    void buildUrlBase();

    /*!
     *  Private function to discover the network address currently used by the machine where control
     *  point is running and select an free port to use with WebServer.
     */
    void discoverNetworkAddress();

    /*!
     *  Generates the webserver tree according to the services present in the device.
     *  Eg:
     *     Suppose we have two services, "Gate" and "LightSwitch". So /Gate and /LightSwitch will be
     *  valid URLs in the webserver.
     */
    void buildWebServerTree();

    /*!
     *  Starts the webserver.
     */
    void startWebServer();

    /*!
     *  Responds the msearch in case of a "ssdp:all" search type.
     *
     *  \sa respondMSearch()
     */
    void respondMSearchAll(const QString &senderIp, quint16 senderPort,
            const QString &sDate);

    /*!
     *  Returns true if the device is running.
     */
    bool isRunning();

    QList<BrisaIcon*> iconList;
    QList<Service *> serviceList;
    QList<Device *> embeddedDeviceList;

    QString major;
    QString minor;
    QString urlBase;
    QString deviceType;
    QString friendlyName;
    QString manufacturer;
    QString manufacturerUrl;
    QString modelDescription;
    QString modelName;
    QString modelNumber;
    QString modelUrl;
    QString serialNumber;
    QString udn;
    QString upc;
    QString presentationUrl;

    QString location;
    QString server;
    QString ipAddress;
    quint16 port;

    QString attribute;
    QString fileAddress;

    QTemporaryFile descriptionFile;

    brisa::shared::ssdp::BrisaSSDPServer *ssdp;
    brisa::shared::webserver::Webserver *webserver;
    bool running;
};

}
}
}

#endif /* _DEVICE_H */

