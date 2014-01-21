#include "mediacontrolpoint.h"

namespace brisa {
namespace upnp {

MediaControlPoint::MediaControlPoint(QObject *parent, QString st, int mx) :
		QObject(parent) {
	this->mediaServerCount = 0;
	this->mediaRendererCount = 0;
	this->running = false;

    this->ssdp = new BrisaSSDPClient(this);
    msearch = new BrisaMSearchClientCP(this, st, mx);

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
		Device *device = NULL;
		QString type =
				(!response.contains("st")) ? response["nt"] : response["st"];

		foreach (QString acceptedType, MEDIA_SERVERS_URN)
		{
			if (type.startsWith(acceptedType)) {
				location = response["location"];
				device = new MediaServerControlPointDevice(uuid, location);
				this->mss.insert(uuid,
						(MediaServerControlPointDevice *) device);
				Q_ASSERT(connect(device, SIGNAL(deviceDescriptionReady(Device *)), this,
						SLOT(handleMediaServerReady(Device *))));
				found = true;
				break;
			}
		}

		if (!found) {
			foreach (QString acceptedType, MEDIA_RENDERER_URN)
			{
				if (type.startsWith(acceptedType)) {
					location = response["location"];
					device = new MediaRendererControlPointDevice(uuid,
							location);
					this->mrs.insert(uuid,
							(MediaRendererControlPointDevice *) device);
					connect(device, SIGNAL(deviceDescriptionReady(Device *)),
							this, SLOT(handleMediaRendererReady(Device *)));
					found = true;
					break;
				}
			}
		}

		if (found) {
			connect(device,
					SIGNAL(errorParsingDeviceDescription(Device *, quint8)),
					this,
					SLOT(handleErrorParsingDeviceDescription(Device *, quint8)));
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

		Device *device = NULL;
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

const MediaServerControlPointDevice * MediaControlPoint::getMediaServer(
		const QString &uuid) {
	return this->mss.value(uuid);
}

const MediaRendererControlPointDevice * MediaControlPoint::getMediaRenderer(
		const QString &uuid) {
	return this->mrs.value(uuid);
}

QList<MediaServerControlPointDevice *> MediaControlPoint::getMediaServers() {
	return this->mss.values();
}

QList<MediaRendererControlPointDevice *> MediaControlPoint::getMediaRenderers() {
	return this->mrs.values();
}

void MediaControlPoint::handleSSDPSetupEvents(QString message) {
	Q_UNUSED(message);
	//qDebug() << message;
}

void MediaControlPoint::handleMediaServerReady(Device * device) {
	disconnect(device, SIGNAL(deviceDescriptionReady(Device *)), this,
			SLOT(handleMediaServerReady(Device *)));
	MediaServerControlPointDevice *ms = (MediaServerControlPointDevice *) device;
	++this->mediaServerCount;
	emit newMediaServerDevice(ms);
}

void MediaControlPoint::handleMediaRendererReady(Device * device) {
	disconnect(device, SIGNAL(deviceDescriptionReady(Device *)), this,
			SLOT(handleMediaRendererReady(Device*)));
	MediaRendererControlPointDevice *mr =
			(MediaRendererControlPointDevice *) device;
	++this->mediaRendererCount;
	emit newMediaRendererDevice(mr);
}

uint MediaControlPoint::getMediaServersCount() {
	return this->mediaServerCount;
}

uint MediaControlPoint::getMediaRenderersCount() {
	return this->mediaRendererCount;
}

void MediaControlPoint::handleErrorParsingDeviceDescription(Device *device,
		quint8 errorCode) {
	qDebug() << "Error parsing device description from "
			<< device->getAttribute("location") << " with error code "
			<< errorCode;
}

void MediaControlPoint::discover() {
    this->msearch->discover();
}

}
}
