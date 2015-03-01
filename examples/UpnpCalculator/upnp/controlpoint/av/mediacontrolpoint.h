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

	const MediaServerDevice *getMediaServer(const QString &);
	const MediaRendererDevice *getMediaRenderer(const QString &);
	QList<MediaServerDevice *> getMediaServers();
	QList<MediaRendererDevice *> getMediaRenderers();
	uint getMediaServersCount();
	uint getMediaRenderersCount();
	void discover();
	void start();
	void stop();
	bool isRunning();

private:
	uint mediaServerCount;
	uint mediaRendererCount;
	//teste *testen;

//	SSDP *ssdp;
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
	void handleNewSSDPMessage(QString);
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
