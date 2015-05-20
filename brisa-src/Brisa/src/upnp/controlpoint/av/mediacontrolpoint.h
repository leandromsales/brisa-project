#ifndef MEDIACONTROLPOINT_H
#define MEDIACONTROLPOINT_H

#include <QObject>

#include "controlpoint_media_globals.h"
#include "mediaserverdevice.h"
#include "mediarenderercpdevice.h"
#include "../../../shared/ssdp/ssdpclient.h"
#include "../msearchclientcp.h"

namespace brisa {
namespace upnp {
namespace controlpoint {
namespace av {

class MediaControlPoint: public QObject {
Q_OBJECT
public:
	MediaControlPoint(QObject *parent = 0, QString st = "ssdp:all", int mx = 5);
	~MediaControlPoint();

    /*!
     * Return a media server device with UUID in argument.
     */
	const MediaServerDevice *getMediaServer(const QString &);
    /*!
     * Return a media renderer device with UUID in argument.
     */
	const MediaRendererDevice *getMediaRenderer(const QString &);
    /*!
     * Return all media server devices.
     */
	QList<MediaServerDevice *> getMediaServers();
    /*!
     * Return all media renderer devices.
     */
	QList<MediaRendererDevice *> getMediaRenderers();
    /*!
     * Return \a mediaServerCount.
     */
    uint getMediaServersCount();
    /*!
     * Return \a mediaRendererCount.
     */
	uint getMediaRenderersCount();
    /*!
     * Call \a discover() from \a MSearchClientCP, which send discover
     * messages to network.
     */
    void discover();
    /*!
     * Start SSDP, discover message and media devices.
     */
	void start();
    /*!
     * Stop SSDP, discover message and media devices.
     */
	void stop();
    /*!
     * Check if server already is running.
     */
	bool isRunning();

private:
	uint mediaServerCount;
	uint mediaRendererCount;

    brisa::shared::ssdp::SSDPClient *ssdp;
	MSearchClientCP *msearch;
	QHash<QString, MediaServerDevice*> mss;
	QHash<QString, MediaRendererDevice*> mrs;
	bool running;

signals:

	void newMediaServerDevice(const MediaServerDevice *);
	void newMediaRendererDevice(const MediaRendererDevice *);
	void leaveMediaServerDevice(QString);
	void leaveMediaRendererDevice(QString);

private slots:
    void handleNewSSDPMessage(QString); //TODO Explain it better
	void handleSSDPSetupEvents(QString);
	void handleMediaServerReady(DeviceSales *device);
	void handleMediaRendererReady(DeviceSales * device);
	void handleErrorParsingDeviceDescription(DeviceSales *device, quint8 errorCode);


};

}  // namespace av
}
}
}

#endif // MEDIACONTROLPOINT_H
