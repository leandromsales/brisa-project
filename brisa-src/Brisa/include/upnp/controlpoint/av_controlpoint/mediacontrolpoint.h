#ifndef MEDIACONTROLPOINT_H
#define MEDIACONTROLPOINT_H

#include <QObject>

#include "controlpoint_media_globals.h"
#include "mediaservercontrolpointdevice.h"
#include "mediarenderercontrolpointdevice.h"
//#include "upnp/shared/ssdp/ssdp.h"
#include "upnp/shared/ssdp/brisassdpclient.h"
#include "upnp/controlpoint/brisamsearchclientcp.h"

//#include "teste.h"
using namespace Brisa;

class MediaControlPoint: public QObject {
Q_OBJECT
public:
	MediaControlPoint(QObject *parent = 0, QString st = "ssdp:all", int mx = 5);
	~MediaControlPoint();

	const MediaServerControlPointDevice *getMediaServer(const QString &);
	const MediaRendererControlPointDevice *getMediaRenderer(const QString &);
	QList<MediaServerControlPointDevice *> getMediaServers();
	QList<MediaRendererControlPointDevice *> getMediaRenderers();
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
	BrisaSSDPClient *ssdp;
	BrisaMSearchClientCP *msearch;
	QHash<QString, MediaServerControlPointDevice*> mss;
	QHash<QString, MediaRendererControlPointDevice*> mrs;
	bool running;

signals:

	void newMediaServerDevice(const MediaServerControlPointDevice *);
	void newMediaRendererDevice(const MediaRendererControlPointDevice *);
	void leaveMediaServerDevice(QString);
	void leaveMediaRendererDevice(QString);

private slots:
	void handleNewSSDPMessage(QString);
	void handleSSDPSetupEvents(QString);
	void handleMediaServerReady(Device *device);
	void handleMediaRendererReady(Device * device);
	void handleErrorParsingDeviceDescription(Device *device, quint8 errorCode);


};

#endif // MEDIACONTROLPOINT_H
