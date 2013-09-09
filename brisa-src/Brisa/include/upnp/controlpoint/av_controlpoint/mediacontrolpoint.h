#ifndef MEDIACONTROLPOINT_H
#define MEDIACONTROLPOINT_H

#include <QObject>

#include "controlpoint_media_globals.h"
#include "mediaservercontrolpointdevice.h"
#include "mediarenderercontrolpointdevice.h"
#include "upnp/shared/ssdp/ssdp.h"

//#include "teste.h"

class MediaControlPoint: public QObject {
Q_OBJECT
public:
	explicit MediaControlPoint(QObject *parent = 0);
	~MediaControlPoint();

	const MediaServerControlPointDevice *getMediaServer(const QString &);
	const MediaRendererControlPointDevice *getMediaRenderer(const QString &);
	QList<MediaServerControlPointDevice *> getMediaServers();
	QList<MediaRendererControlPointDevice *> getMediaRenderers();
	uint getMediaServersCount();
	uint getMediaRenderersCount();

private:
	uint mediaServerCount;
	uint mediaRendererCount;
	//teste *testen;

	SSDP *ssdp;
	QHash<QString, MediaServerControlPointDevice*> mss;
	QHash<QString, MediaRendererControlPointDevice*> mrs;

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
