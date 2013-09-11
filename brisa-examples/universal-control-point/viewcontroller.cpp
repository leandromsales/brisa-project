#include "viewcontroller.h"
#include "upnp/controlpoint/av_controlpoint/didl/didllite.h"


ViewController::ViewController(QmlDocument *context, QObject *parent) :
    QObject(parent)
{
	this->uiCtx = context;
	this->currentMediaServer = NULL;
	this->currentMediaRenderer = NULL;

	this->mediaControlPoint = new MediaControlPoint(this);
	this->mediaRendererDevice = new MediaRendererDevice(this);

	connect(mediaControlPoint, SIGNAL(newMediaServerDevice(const MediaServerControlPointDevice *)), this, SLOT(newMediaServerDevice(const MediaServerControlPointDevice *)));
	connect(mediaControlPoint, SIGNAL(newMediaRendererDevice(const MediaRendererControlPointDevice *)), this, SLOT(newMediaRendererDevice(const MediaRendererControlPointDevice *)));
	connect(mediaControlPoint, SIGNAL(leaveMediaServerDevice(QString)), this, SLOT(leaveMediaServerDevice(QString)));
	connect(mediaControlPoint, SIGNAL(leaveMediaRendererDevice(QString)), this, SLOT(leaveMediaRendererDevice(QString)));

	this->mediaServerModel = new GroupDataModel();
	this->mediaRendererModel = new GroupDataModel();
	this->mediaServerContentModel = new ListModel(new MediaContentItem, this);
	this->navigationContentModel = new ListModel(new MediaContentItem, this);
	this->mediaPlayerModel = new MediaPlayerModel();
	/*this->mediaPlayerModel->setMedia(QUrl("http://localhost/linux.mp4"));
	this->mediaPlayerModel->play();
	this->mediaPlayerModel->setVisible(true);*/
	//this->mediaRendererModel->insert(new MediaItemLocal("Local"));

	uiCtx->setContextProperty("_mediaPlayerModel", static_cast<QObject *>(this->mediaPlayerModel));

	this->uiCtx->setContextProperty("mediaServerModel", mediaServerModel);
	this->uiCtx->setContextProperty("mediaRendererModel", mediaRendererModel);
	this->uiCtx->setContextProperty("mediaServerContentModel", mediaServerContentModel);
	this->uiCtx->setContextProperty("navigationContentModel", navigationContentModel);
	this->uiCtx->setContextProperty("MediaPlayerModel", mediaPlayerModel);
	//this->mediaRendererDevice->start();
	this->m_playlocal = true;

	qDebug() << "Starting...";
}

ViewController::~ViewController() {
    //delete this->uiCtx; // not necessary because qqmlengine take care of this.
    delete this->mediaServerModel;
    delete this->mediaRendererModel;
    delete this->mediaServerContentModel;
    delete this->navigationContentModel;
    delete this->mediaPlayerModel;
}

void ViewController::goesToQML()
{
    this->uiCtx->setContextProperty("mainViewController", this);
}

void ViewController::init()
{
    qDebug() << "Initializing from cpp...[ok]";
}

void ViewController::changeVolume(int value)
{
    if(this->currentMediaRenderer) {
        Service * service = this->currentMediaRenderer->getService("urn:upnp-org:serviceId:RenderingControl");
        this->executeSetVolume(service, QString::number(value));
    } else {
		this->mediaPlayerModel->setVolume(value);
	}
}

void ViewController::onStopButtonPressed() {
    if (this->currentMediaRenderer) {
        Service * service = this->currentMediaRenderer->getService("urn:upnp-org:serviceId:AVTransport");
        this->executeStop(service);
        emit changeStatusToPlay(false);
    } else {
		this->mediaPlayerModel->setVisible(false);
		this->mediaPlayerModel->stop();
	}
}

void ViewController::onBackButtonPressed()
{
    int length = this->navigationContentModel->rowCount();
    //qDebug() << "Back touched!" << length;
    if(length > 1) {
        MediaContentItem * item = static_cast<MediaContentItem *>(this->navigationContentModel->takeRow(length - 1));
        Service * service = this->currentMediaServer->getService("urn:upnp-org:serviceId:ContentDirectory");
        this->executeBrowse(service, item->parentId());
        delete item;
    }
}

void ViewController::onPlayPausePressed(bool isPlaying)
{
    if(!isPlaying) {
        if (this->currentMediaRenderer && !this->m_playlocal) {
            Service * service = this->currentMediaRenderer->getService("urn:upnp-org:serviceId:AVTransport");
            this->executePause(service);
        } else {
			this->playLocal();
		}
    } else {
        if (this->currentMediaRenderer && !this->m_playlocal) {
            Service * service = this->currentMediaRenderer->getService("urn:upnp-org:serviceId:AVTransport");
            //this->executeStop(service);
            this->executePlay(service);
        } else {
			this->playLocal();
		}
    }
}

void ViewController::onChangeSelectionForServerContentList(QString objectId)
{
    MediaContentItem * item = static_cast<MediaContentItem *>(this->mediaServerContentModel->find(objectId));
    if(item->isContainer()) {
        Service * service = this->currentMediaServer->getService("urn:upnp-org:serviceId:ContentDirectory");
        MediaContentItem * itemNavigator = new MediaContentItem(item->objectId(), item->parentId(), item->title(), item->resource(), item->parent());
        this->navigationContentModel->appendRow(itemNavigator);
        this->executeBrowse(service, objectId);
    } else {
        QUrl urlTester(item->resource());
        if (!urlTester.isValid()) {
            qDebug() << "Invalid media URI";
            return;
        }
        this->currentURI = urlTester.toString().toUtf8();
        if (this->currentMediaRenderer && !this->m_playlocal) {
            Service * service = this->currentMediaRenderer->getService("urn:upnp-org:serviceId:AVTransport");
            //            this->executeStop(service);
            this->executeSetAVTransportURI(service);
        } else {
            this->playLocal();
        }
    }
}
void ViewController::onSelectionChangedFolderNavigator(QString objectId)
{
    ListItem * item = this->navigationContentModel->find(objectId);
    if(item){
        QModelIndex index = this->navigationContentModel->indexFromItem(item);
        this->navigationContentModel->removeRows(index.row() + 1, this->navigationContentModel->rowCount() - 1 - index.row());
    }
    Service * service = this->currentMediaServer->getService("urn:upnp-org:serviceId:ContentDirectory");
    this->executeBrowse(service, objectId);
}

void ViewController::onChangeSelectionForServerList(QString uuid)
{
	//TODO Tratar evento onChangeSelectionForServerList
    //qDebug() << "Selection changed!" << uuid;
   /* MediaItem * currentServerItem = static_cast<MediaItem *>(mediaServerModel->find(uuid));
    this->currentMediaServer = currentServerItem->getDevice();
    Service * service = this->currentMediaServer->getService("urn:upnp-org:serviceId:ContentDirectory");
    this->executeBrowse(service, "0");
    this->navigationContentModel->clear();
    this->navigationContentModel->appendRow(new MediaContentItem("0", "0", "Root", "", true));
    this->mediaServerContentModel->clear();*/
}

void ViewController::onChangeSelectionForRendererList(QString uuid)
{
    //qDebug() << "Selection changed Renderer!" << uuid;
    /*if (uuid != "local") {
        MediaItem * currentRendererItem = static_cast<MediaItem *>(mediaRendererModel->find(uuid));
        this->currentMediaRenderer = currentRendererItem->getDevice();
        Service * service = this->currentMediaRenderer->getService("urn:upnp-org:serviceId:AVTransport");
        this->executeGetTransportInfo(service);
        this->m_playlocal = false;
    } else {
        this->m_playlocal = true;
        this->playLocal();
    }*/
}

void ViewController::newMediaServerDevice(const MediaServerControlPointDevice *device) {
    //this->mediaServerModel->appendRow(new MediaItem((Device *)device, mediaServerModel));
	QVariantMap newServer;
	newServer["name"] = device->getAttribute("friendlyName");
	newServer["uuid"] = device->getAttribute("uuid");
	this->mediaServerModel->insert(newServer);
     //qDebug() << "server count " << mediaServerModel->rowCount();
    qDebug() << "server added! " << device->getAttribute("friendlyName") << device->getAttribute("uuid");
}

void ViewController::newMediaRendererDevice(const MediaRendererControlPointDevice *device) {
    //this->mediaRendererModel->appendRow(new MediaItem((Device *)device, mediaRendererModel));
	QVariantMap newRenderer;
	newRenderer["name"] = device->getAttribute("friendlyName");
	newRenderer["uuid"] = device->getAttribute("uuid");
	this->mediaRendererModel->insert(newRenderer);
    qDebug() << "renderer add! " << device->getAttribute("friendlyName") << device->getAttribute("uuid");
}
void ViewController::leaveMediaServerDevice(const QString &uuid)
{
    /*ListItem * item = this->mediaServerModel->removeById(uuid);
    if (item) {
        delete item;
    }
    //qDebug() << "server removed!" << uuid;*/
	qDebug() << "remove Server";
}

void ViewController::leaveMediaRendererDevice(const QString &uuid)
{
    /*ListItem * item = this->mediaRendererModel->removeById(uuid);
    if (item) {
        delete item;
    }*/
    //qDebug() << "renderer removed!" << uuid;
}

void ViewController::executeBrowse(Service *service, const QString &objectId) {
    if (service) {
        connect(service,
                SIGNAL(executionActionReply(QString, QHash<QString, ActionArgument *>, Service *)),
                this,
                SLOT(handleExecutionActionReply(QString, QHash<QString, ActionArgument *>, Service *)));
        QHash<QString, QString> args;
        QString id = (objectId.isEmpty()) ? "0" : objectId;
        args["ObjectID"] = id;
        args["BrowseFlag"] = "BrowseDirectChildren";
        args["Filter"] = "*";
        args["StartingIndex"] = "0";
        args["RequestedCount"] = "0";
        args["SortCriteria"] = "";
        QString error = service->executeAction("Browse", args);
        if (!error.isEmpty()) {
            qDebug() << error;
        }
    }
}

void ViewController::executeSetAVTransportURI(Service *service) {
    if (service) {
        connect(service,
                SIGNAL(executionActionReply(QString, QHash<QString, ActionArgument *>, Service *)),
                this,
                SLOT(handleExecutionActionReply(QString, QHash<QString, ActionArgument *>, Service *)));
        QHash<QString, QString> args;
        args["InstanceID"] = "0";
        // TODO: se uri nao tiver definido, fazer browse(id, 'BrowseMetadata', '*', 0, 1, '')
        args["CurrentURI"] = this->currentURI;
        args["CurrentURIMetaData"] = "";
        QString error = service->executeAction("SetAVTransportURI", args);
        if (!error.isEmpty()) {
            qDebug() << error;
        }
    }
}

void ViewController::executeGetTransportInfo(Service *service) {
    if (service) {
        connect(service,
                SIGNAL(executionActionReply(QString, QHash<QString, ActionArgument *>, Service *)),
                this,
                SLOT(handleExecutionActionReply(QString, QHash<QString, ActionArgument *>, Service *)));
        QHash<QString, QString> args;
        args["InstanceID"] = "0";
        QString error = service->executeAction("GetTransportInfo", args);
        if (!error.isEmpty()) {
            qDebug() << error;
        }
    }
}

void ViewController::handleExecutionActionReply(QString actionName,
                                                    QHash<QString, ActionArgument *> response,
                                                    Service *service) {
    Q_UNUSED(service)
    //qDebug() << "Method name:"<< actionName;
    if (actionName == "Browse") {
        DIDLLite didl;
        QString didlText = response.value("Result")->getAttribute("value");
        Container *container = didl.parse(didlText);
        QList<Container *> containers = container->getContainers();
        QList<Item *> items = container->getItems();

        this->mediaServerContentModel->clear();
        emit emptyMediaServerContentList();

        foreach (Container *container, containers) {
            this->mediaServerContentModel->appendRow(new MediaContentItem(container->getProperty("id"),
                                                                          container->getProperty("parentId"),
                                                                          container->getAttribute("title"),
                                                                          "", true));

        }
        Res *res;
        foreach (Item *item, items) {
            res = item->getRes();
            if (res != NULL) {
                this->mediaServerContentModel->appendRow(new MediaContentItem(item->getProperty("id"),
                                                                          item->getAttribute("parentId"),
                                                                          item->getAttribute("title"),
                                                                          res->getValue(), false));
            }

        }

        delete container;

        if (containers.length() == 0 && items.length() == 0) {
            this->mediaServerContentModel->appendRow(new MediaContentItem("",
                                                                          "",
                                                                          "Empty folder",
                                                                          "", false));
        }

    } else if (actionName == "SetAVTransportURI") {
        //        if (this->currentMediaRenderer != NULL) {
        //            this->executePlay(service);
        //        }
    } else if (actionName == "Play") {
        //qDebug() << "Playing...";
        if (this->currentMediaRenderer != NULL) {
            this->executePlay(service);
        }
    } else if (actionName == "Stop") {
        //qDebug() << "Stopped...";
        if (this->currentMediaRenderer != NULL) {
            this->executeStop(service);
        }
    } else if (actionName == "Pause") {
        //qDebug() << "Paused...";
        if (this->currentMediaRenderer != NULL) {
            this->executePause(service);
        }
    } else if (actionName == "SetVolume") {
        //qDebug() << "Volume changed...";
    } else if (actionName == "GetVolume") {
        //this->updateVolume = false;
        //        this->ui->s_volume->setValue(response.value("CurrentVolume")->getAttribute("value").toInt());
    } else if (actionName == "GetTransportInfo") {
        QString transportState = response.value("CurrentTransportState")->getAttribute("value");
        if(transportState=="PLAYING"){
            emit changeStatusToPlay(true);
        } else if(transportState=="STOPPED") {
            emit changeStatusToPlay(false);
        } else if(transportState=="PAUSED_PLAYBACK") {
            emit changeStatusToPlay(false);
        }
    }
    disconnect(service,
               SIGNAL(executionActionReply(QString, QHash<QString, ActionArgument *>, Service *)),
               this,
               SLOT(handleExecutionActionReply(QString, QHash<QString, ActionArgument *>, Service *)));
}

void ViewController::executePlay(Service *service) {
    if (service) {
        connect(service,
                SIGNAL(executionActionReply(QString, QHash<QString, ActionArgument *>, Service *)),
                this,
                SLOT(handleExecutionActionReply(QString, QHash<QString, ActionArgument *>, Service *)));
        QHash<QString, QString> args;
        args["InstanceID"] = "0";
        args["Speed"] = "1";
        QString error = service->executeAction("Play", args);
        if (!error.isEmpty()) {
            qDebug() << error;
        }
    }
}

void ViewController::executeStop(Service *service) {
    if (service) {
        connect(service,
                SIGNAL(executionActionReply(QString, QHash<QString, ActionArgument *>, Service *)),
                this,
                SLOT(handleExecutionActionReply(QString, QHash<QString, ActionArgument *>, Service *)));
        QHash<QString, QString> args;
        args["InstanceID"] = "0";
        QString error = service->executeAction("Stop", args);
        if (!error.isEmpty()) {
            qDebug() << error;
        }
    }
}

void ViewController::executePause(Service *service) {
    if (service) {
        connect(service,
                SIGNAL(executionActionReply(QString, QHash<QString, ActionArgument *>, Service *)),
                this,
                SLOT(handleExecutionActionReply(QString, QHash<QString, ActionArgument *>, Service *)));
        QHash<QString, QString> args;
        args["InstanceID"] = "0";
        QString error = service->executeAction("Pause", args);
        if (!error.isEmpty()) {
            qDebug() << error;
        }
    }
}

void ViewController::executeSetVolume(Service *service, const QString &desiredVolume) {
    if (service) {
        connect(service,
                SIGNAL(executionActionReply(QString, QHash<QString, ActionArgument *>, Service *)),
                this,
                SLOT(handleExecutionActionReply(QString, QHash<QString, ActionArgument *>, Service *)));
        QHash<QString, QString> args;
        args["InstanceID"] = "0";
        args["Channel"] = "Master";
        args["DesiredVolume"] = desiredVolume;

        QString error = service->executeAction("SetVolume", args);
        if (!error.isEmpty()) {
            qDebug() << error;
        }
    }
}

void ViewController::positionChanged(qint64 position) {
    Q_UNUSED(position);
    qDebug() << "Current position is: " << position;
}

void ViewController::playLocal() {
    this->mediaPlayerModel->setMedia(QUrl(this->currentURI));
    switch (this->mediaPlayerModel->state()) {
    	case MediaPlayerModel::PlayingState:
        	this->mediaPlayerModel->pause();
            this->mediaPlayerModel->setVisible(false);
        	break;
    	default:
        	this->mediaPlayerModel->play();
			this->mediaPlayerModel->setVisible(true);
        	break;
	}
}

GroupDataModel * ViewController::getMediaServerModel() {
	qDebug() << "get Model";
	return this->mediaServerModel;
}

ListModel * ViewController::getMediaServerContentModel() {
	return this->mediaServerContentModel;
}

GroupDataModel * ViewController::getMediaRendererModel() {
	return this->mediaRendererModel;
}

ListModel * ViewController::getNavigationContentModel() {
	return this->navigationContentModel;
}
