/*
 * ViewController.cpp
 *
 *  Created on: 09/09/2013
 *      Author: daniel
 */

#include "ViewController.hpp"

using namespace bb::cascades;
using namespace bb::system;

using namespace brisa;
using namespace brisa::upnp;
using namespace brisa::upnp::controlpoint;

ViewController::ViewController(QObject *parent) :
		QObject(parent) {

	m_eventLogModel = new GroupDataModel(
			QStringList() << "deviceName" << "service" << "newValue"
					<< "variableName" << "type", this);
    m_eventLogModel->setSortingKeys(QStringList() << "type");
    m_eventLogModel->setGrouping(ItemGrouping::ByFullValue);

	m_connected = false;
	emit onConnected();

	m_currentDev = 0;
	m_currentDeviceItem = 0;

	m_devices = DeviceList::getInstance();

	m_searchIng = false;
	m_searchingTimer = new QTimer(this);
	connect(m_searchingTimer, SIGNAL(timeout()), this, SLOT(hideSearching()));

	// TODO Verificar como usar apenas o sinal emptyList
	m_isEmptyList = true;
	emit onEmptyListChanged();
	emit emptyList(true);

	m_devicesModel = new ArrayDataModel();

	m_controlPoint = new BrisaControlPoint();

	connect(m_controlPoint, SIGNAL(deviceGone(QString)), this,
			SLOT(removeDevice(QString)));
	connect(m_controlPoint, SIGNAL(deviceFound(BrisaControlPointDevice*)), this,
			SLOT(deviceFoundDump(BrisaControlPointDevice*)));

	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(testConnection()));
	emit testConnection();

	connect(m_controlPoint, SIGNAL(multicastReceived(QString,QString)), this,
			SLOT(multicastEventReceived(QString,QString)));
	connect(m_controlPoint, SIGNAL(multicastReceivedRaw(BrisaOutArgument)),
			this, SLOT(multicastEventRawReceived(BrisaOutArgument)));
}

ViewController::~ViewController() {
	m_controlPoint->stop();
	delete m_controlPoint;
}

void ViewController::onReadyDownloadIcons(BrisaControlPointDevice *device) {
	//Implementar uso de icones
	m_currentDeviceItem->insert("_icon",
			device->getIconList().at(0)->getIcon().name());
}

//void ViewController::lineEnabled(QTreeWidgetItem* item, int collumn) {
//}

void ViewController::deviceFoundDump(BrisaControlPointDevice *device) {
	if (device->getAttribute(BrisaControlPointDevice::Udn).compare("") == 0)
		return;
	// TODO Verificar se podemos usar esse metodo de verificacao de duplicidade
//	if (m_devices->getDeviceByUDN(
//			device->getAttribute(BrisaControlPointDevice::Udn))) {
//		(*m_devices)[i] = device;
//		return;
//	}
	for (int i = 0; i < m_devices->size(); i++) {
		if ((*m_devices)[i]->getAttribute(BrisaControlPointDevice::Udn)
				== device->getAttribute(BrisaControlPointDevice::Udn)) {
			(*m_devices)[i] = device;
			return;
		}
	}
	m_devices->append(device);
	this->m_currentDev = device;
	createDeviceItem();
	// TODO Verificar como usar apenas o sinal emptyList
	m_isEmptyList = false;
	emit onEmptyListChanged();
	emit emptyList(false);
}

void ViewController::removeDevice(QString udn) {
	for (int i = 0; i < m_devices->size(); i++) {
		if (m_devices->at(i)->getAttribute(BrisaControlPointDevice::udn).compare(
				udn.split("::")[0]) == 0) {
			m_devices->removeAt(i);
			m_devicesModel->removeAt(i);
			break;
		}
	}
	if (m_devices->isEmpty()) {
		// TODO Verificar como usar apenas o sinal emptyList
		m_isEmptyList = true;
		emit onEmptyListChanged();
		emit emptyList(true);
	}
}

void ViewController::multicastEventReceived(QString variableName,
		QString newValue) {
	qDebug() << "ViewController::multicastEventReceived:";
	qDebug() << "Multicast event message received:";
	qDebug() << "State variable: " << variableName << " changed value to "
			<< newValue;
}

void ViewController::multicastEventRawReceived(BrisaOutArgument raw) {
	qDebug() << "ViewController::multicastEventRawReceived:";
	qDebug() << "Multicast event message raw received:";
	qDebug() << "Raw: " << raw;

	QVariantMap model;
	model.insert("type", "multicast");
	model.insert("deviceName", raw["USN"]);
	model.insert("service", raw["SVCID"]);
	model.insert("newValue", raw["newValue"]);
	model.insert("variableName", raw["variableName"]);
	m_eventLogModel->insert(model);
	emit onEventLogModelChanged();
}

void ViewController::unicastEventReceived(BrisaEventProxy* subscription,
		QMap<QString, QString> eventVariables) {
	QList<QString> deviceService = m_eventToDevice[subscription->getId()];
	qDebug() << "Unicast event message received:";
	for (int i = 0; i < eventVariables.keys().size(); i++) {
		qDebug() << "State variable " << eventVariables.keys()[i]
				<< "changed value to "
				<< eventVariables[eventVariables.keys()[i]];
	}
}

void ViewController::startControlPoint() {
	m_controlPoint->start();
	m_controlPoint->discover();
	m_controlPointRunning = true;
}

void ViewController::stopControlPoint() {
	m_controlPoint->stop();
	m_devices->clear();
	m_devicesModel->clear();
	// TODO Verificar como usar apenas o sinal emptyList
	m_isEmptyList = true;
	emit onEmptyListChanged();
	emit emptyList(true);
	m_controlPointRunning = false;
}

void ViewController::refreshList() {
	stopControlPoint();
	startControlPoint();
}

bool ViewController::isConnected() {
	return m_connected;
}

void ViewController::callBBMCard() {
	InvokeManager* invokeManager = new InvokeManager();
	InvokeRequest cardRequest;
	cardRequest.setTarget("sys.bbm.sharehandler"); // Aqui define o target;
	cardRequest.setAction("bb.action.SHARE");
	cardRequest.setMimeType("text/plain");
	cardRequest.setData(
			"Use Universal Control Point App for BlackBerry 10 to control your network devices.");
	InvokeTargetReply* reply = invokeManager->invoke(cardRequest);
	reply->setParent(this);
}

void ViewController::callSettingsCard() {
	InvokeManager* invokeManager = new InvokeManager();
	InvokeRequest cardRequest;
	cardRequest.setTarget("sys.settings.target"); // Aqui define o target;
	cardRequest.setAction("bb.action.OPEN");
	cardRequest.setMimeType("settings/view");
	cardRequest.setUri("settings://networkconnections");
	InvokeTargetReply* reply = invokeManager->invoke(cardRequest);
	reply->setParent(this);
}

void ViewController::startSearchingTimer() {
	m_searchingTimer->start(5000);
}

void ViewController::hideSearching() {
	emit onSearching();
}

bool ViewController::searching() {
	m_searchIng = !m_searchIng;
	return m_searchIng;
}

GroupDataModel* ViewController::eventLogModel() {
	return m_eventLogModel;
}

void ViewController::clearEventLog() {
	m_eventLogModel->clear();
	emit onEventLogModelChanged();
}

void ViewController::removeEvent(QVariantMap data) {
	m_eventLogModel->remove(data);
}

bool ViewController::isEmptyList() {
	return m_isEmptyList;
}

void ViewController::onPopPage() {
	m_devices->popCurrentItem();
}

void ViewController::createDeviceItem() {

	m_currentDeviceItem = new QVariantMap();

	m_currentDeviceItem->insert("friendlyName",
			m_currentDev->getAttribute(BrisaControlPointDevice::FriendlyName));
	m_currentDeviceItem->insert("typeName",
			m_currentDev->getAttribute(BrisaControlPointDevice::deviceType));
	m_currentDeviceItem->insert("type", 0);
	m_currentDeviceItem->insert("id",
			m_currentDev->getAttribute(BrisaControlPointDevice::Udn));

	m_devicesModel->append(*m_currentDeviceItem);
// TODO Verficiar como colocar um icone remoto no item da lista
//    if(currentDev->getIconList().size() > 0){
//        connect(currentDev, SIGNAL(onReadyDownloadIcons(BrisaControlPointDevice*)),
//                this, SLOT(onReadyDownloadIcons(BrisaControlPointDevice*)));
//        currentDev->downloadIcons();
//    } else {
////    	(*currentDeviceItem)["_icon"] = "qrc://assets/images/device.png";
//        addItem(currentDeviceItem);
//    }
}

ArrayDataModel* ViewController::getGenericDataModelByID(QString ID) {
	ArrayDataModel *arrayDataModel = new ArrayDataModel();
	int type = m_devices->setCurrentItemByID(ID);
	ExecutableAction *executableAction = ExecutableAction::getInstance();
	qDebug()
			<< "======================================================================";
	qDebug() << "ViewController::getGenericDataModelByID:" << type;
	switch (type) {
	case DeviceList::DEVICE: {
		BrisaControlPointDevice *device = executableAction->getDevice();
		qDebug() << "ViewController::getGenericDataModelByID => DEVICE:"
				<< device->getAttribute(BrisaControlPointDevice::friendlyName)
				<< "EmbeddedDevices:" << device->getEmbeddedDeviceList().size()
				<< "Services:" << device->getServiceList().size();
		foreach (BrisaControlPointDevice *tempDevice, device->getEmbeddedDeviceList())
		{
			qDebug()
					<< "ViewController::getGenericDataModelByID => DEVICE/DEVICE:"
					<< tempDevice->getAttribute(
							BrisaControlPointDevice::friendlyName);
			QVariantMap map;
			map.insert("friendlyName",
					tempDevice->getAttribute(
							BrisaControlPointDevice::friendlyName));
			map.insert("typeName",
					tempDevice->getAttribute(
							BrisaControlPointDevice::deviceType));
			map.insert("type", 0);
			map.insert("id",
					tempDevice->getAttribute(BrisaControlPointDevice::udn));
			arrayDataModel->append(map);
		}
		foreach (BrisaControlPointService *tempService, device->getServiceList())
		{
			qDebug()
					<< "ViewController::getGenericDataModelByID => DEVICE/SERVICE:"
					<< ExecutableAction::friendlyNameFromServiceType(
							tempService->getAttribute(
									BrisaControlPointService::ServiceType));

			// ==================== Subscription in unicastEvent ====================
			BrisaEventProxy *subscription =
					m_controlPoint->getSubscriptionProxy(tempService);
			connect(subscription,
					SIGNAL(eventNotification(BrisaEventProxy *,QMap<QString, QString>)),
					this,
					SLOT(unicastEventReceived(BrisaEventProxy *,QMap<QString, QString>)));
			subscription->subscribe(20);

			QList<QString> deviceAndService;

			deviceAndService.append(
					device->getAttribute(
							BrisaControlPointDevice::FriendlyName));
			deviceAndService.append(
					tempService->getAttribute(
							BrisaControlPointService::ServiceType));

			m_eventToDevice[subscription->getId()] = deviceAndService;

			// ==================== end ====================

			QVariantMap map;
			map.insert("friendlyName",
					ExecutableAction::friendlyNameFromServiceType(
							tempService->getAttribute(
									BrisaControlPointService::ServiceType)));
			map.insert("typeName",
					tempService->getAttribute(
							BrisaControlPointService::ServiceType));
			map.insert("type", 1);
			map.insert("id",
					tempService->getAttribute(
							BrisaControlPointService::ServiceId));
			arrayDataModel->append(map);
		}
		break;
	}
	case DeviceList::SERVICE: {
		BrisaControlPointService *service = executableAction->getService();
		qDebug() << "ViewController::getGenericDataModelByID => SERVICE:"
				<< ExecutableAction::friendlyNameFromServiceType(
						service->getAttribute(
								BrisaControlPointService::ServiceType))
				<< "Actions:" << service->getActionList().size();
		foreach (BrisaAction *tempAction, service->getActionList())
		{
			qDebug()
					<< "ViewController::getGenericDataModelByID => SERVICE/ACTION:"
					<< tempAction->getName();
			QVariantMap map;
			map.insert("friendlyName", tempAction->getName());
			map.insert("type", 2);
			arrayDataModel->append(map);
		}
		break;
	}
	case DeviceList::ACTION:
	default:
		break;
	}
	qDebug()
			<< "======================================================================";
	return arrayDataModel;
}

ArrayDataModel *ViewController::devicesModel() {
	return m_devicesModel;
}

void ViewController::testConnection() {
	QNetworkConfigurationManager mgr;
	QList<QNetworkConfiguration> activeConfigs = mgr.allConfigurations(
			QNetworkConfiguration::Active);

	m_connected = (activeConfigs.count() > 0) ? mgr.isOnline() : false;

	if (m_connected) {
		if (!m_controlPointRunning)
			startControlPoint();
		qDebug() << "Succeeded.";
	} else {
		if (m_controlPointRunning)
			stopControlPoint();
		qDebug() << "Fail";
	}

	emit onConnected();

	m_timer->start(1000);
}
