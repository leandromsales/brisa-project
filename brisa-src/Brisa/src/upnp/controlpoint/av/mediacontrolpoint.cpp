#include "upnp/controlpoint/av/mediacontrolpoint.h"

namespace brisa {
using namespace shared::ssdp;
namespace upnp {
namespace controlpoint {
namespace av {

MediaControlPoint::MediaControlPoint(QObject *parent, QString st, int mx) :
		QObject(parent) {
	this->mediaServerCount = 0;
	this->mediaRendererCount = 0;
	this->running = false;

    this->ssdp = new SSDPClient(this);
    msearch = new MSearchClientCP(this, st, mx);

	bool c = connect(ssdp, SIGNAL(messageReceived(QString)), this,
			SLOT(handleNewSSDPMessage(QString)));
	 connect(msearch, SIGNAL(messageReceived(QString)), this,
				SLOT(handleNewSSDPMessage(QString)));
	Q_ASSERT(c);

	qDebug() << "MediaControlPoint started...";
}

MediaControlPoint::~MediaControlPoint() {
	if (!isRunning())
		this->stop();

	delete this->ssdp;
	delete this->msearch;
}

void MediaControlPoint::handleNewSSDPMessage(QString message) {
	//TODO Comment this

	int headerType = 0; // 1=http; 2=notify
//	qDebug() << message;

	if (message.startsWith("HTTP/1.1 200 OK")) {
		headerType = 1;
	} else if (message.startsWith("NOTIFY * HTTP/1.1")) {
		headerType = 2;
	} else {
//        qDebug() << "=============== NEW MESSAGE ================";
//        qDebug() << "WRONG SEARCH MESSAGE";
//        qDebug() << message;
//        qDebug() << "============ END OF NEW MESSAGE ============";
		return;
	}

	QString uuid = "";

	QRegExp rexp("[a-f0-9-]{36}", Qt::CaseInsensitive);
	if (rexp.indexIn(message) >= 0) {
		uuid = rexp.cap(0);
	}

	QMap<QString, QString> response = ssdp->getMapFromMessage(message);
	QString nts = response["nts"];

	if (nts == "ssdp:alive" || headerType == 1) {
		if (this->mss.contains(uuid) || uuid.isEmpty()) {
			return;
		}
		if (this->mrs.contains(uuid) || uuid.isEmpty()) {
			return;
		}

		bool found = false;
		QString location;
		DeviceSales *device = NULL;
		QString type =
				(!response.contains("st")) ? response["nt"] : response["st"];

		foreach (QString acceptedType, MEDIA_SERVERS_URN)
		{
			if (type.startsWith(acceptedType)) {
				location = response["location"];
				device = new MediaServerDevice(uuid, location);
				this->mss.insert(uuid,
						(MediaServerDevice *) device);
				Q_ASSERT(connect(device, SIGNAL(deviceDescriptionReady(DeviceSales *)), this,
						SLOT(handleMediaServerReady(DeviceSales *))));
				found = true;
				break;
			}
		}

		if (!found) {
			foreach (QString acceptedType, MEDIA_RENDERER_URN)
			{
				if (type.startsWith(acceptedType)) {
					location = response["location"];
					device = new MediaRendererDevice(uuid,
							location);
					this->mrs.insert(uuid,
							(MediaRendererDevice *) device);
					connect(device, SIGNAL(deviceDescriptionReady(DeviceSales *)),
							this, SLOT(handleMediaRendererReady(DeviceSales *)));
					found = true;
					break;
				}
			}
		}

		if (found) {
			connect(device,
					SIGNAL(errorParsingDeviceDescription(DeviceSales *, quint8)),
					this,
					SLOT(handleErrorParsingDeviceDescription(DeviceSales *, quint8)));
			device->parseDescription();
		} else {
			//qDebug() << "DEVICE OF TYPE" << type << " NOT HANDLED LOCATION: " << location;
		}

	} else if (nts == "ssdp:byebye") {

		//TODO Testar o ssdp:bye-bye
		if (!this->mss.contains(uuid) && !this->mrs.contains(uuid)) {
			return;
		}

		bool found = false;
		QString type =
				(!response.contains("st")) ? response["nt"] : response["st"];

		DeviceSales *device = NULL;
		foreach (QString acceptedType, MEDIA_SERVERS_URN)
		{
			if (type.startsWith(acceptedType)) {
				device = this->mss.take(uuid);
				--this->mediaServerCount;
				emit leaveMediaServerDevice(uuid);
				found = true;
				break;
			}
		}

		if (!found) {
			foreach (QString acceptedType, MEDIA_RENDERER_URN)
			{
				if (type.startsWith(acceptedType)) {
					device = this->mrs.take(uuid);
					--this->mediaRendererCount;
					emit leaveMediaRendererDevice(uuid);
					found = true;
					break;
				}
			}
		}

		if (found) {
			delete device;
		}
	}
}

void MediaControlPoint::start(){
	 if (isRunning()) {
	        qDebug() << "Brisa Control Point: already started.";
	    } else {
	        this->ssdp->start();
	        this->msearch->start();
	        this->running = true;
	    }
}

void MediaControlPoint::stop(){
	 if (!isRunning()) {
	        qDebug() << "Brisa Control Point: already stopped.";
	    } else {
		this->ssdp->stop();
		this->msearch->stop();
		this->running = false;
		this->mediaServerCount = 0;
		this->mediaRendererCount = 0;
		this->getMediaRenderers().clear();
		this->getMediaServers().clear();
		this->mss.clear();
		this->mrs.clear();
	}
}

bool MediaControlPoint::isRunning() {
    return this->running;
}

const MediaServerDevice * MediaControlPoint::getMediaServer(
		const QString &uuid) {
	return this->mss.value(uuid);
}

const MediaRendererDevice * MediaControlPoint::getMediaRenderer(
		const QString &uuid) {
	return this->mrs.value(uuid);
}

QList<MediaServerDevice *> MediaControlPoint::getMediaServers() {
	return this->mss.values();
}

QList<MediaRendererDevice *> MediaControlPoint::getMediaRenderers() {
	return this->mrs.values();
}

void MediaControlPoint::handleSSDPSetupEvents(QString message) {
	Q_UNUSED(message);
	//qDebug() << message;
}

void MediaControlPoint::handleMediaServerReady(DeviceSales * device) {
	disconnect(device, SIGNAL(deviceDescriptionReady(DeviceSales *)), this,
			SLOT(handleMediaServerReady(DeviceSales *)));
	MediaServerDevice *ms = (MediaServerDevice *) device;
	++this->mediaServerCount;
	emit newMediaServerDevice(ms);
}

void MediaControlPoint::handleMediaRendererReady(DeviceSales * device) {
	disconnect(device, SIGNAL(deviceDescriptionReady(DeviceSales *)), this,
			SLOT(handleMediaRendererReady(DeviceSales*)));
	MediaRendererDevice *mr =
			(MediaRendererDevice *) device;
	++this->mediaRendererCount;
	emit newMediaRendererDevice(mr);
}

uint MediaControlPoint::getMediaServersCount() {
	return this->mediaServerCount;
}

uint MediaControlPoint::getMediaRenderersCount() {
	return this->mediaRendererCount;
}

void MediaControlPoint::handleErrorParsingDeviceDescription(DeviceSales *device,
		quint8 errorCode) {
	qDebug() << "Error parsing device description from "
			<< device->getAttribute("location") << " with error code "
			<< errorCode;
}

void MediaControlPoint::discover() {
    this->msearch->discover();
}

}  // namespace av
}
}
}
