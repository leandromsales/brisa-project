/*
 * servicecontroller.cpp
 *
 *  Created on: 16/09/2013
 *      Author: daniel
 */

#include "servicecontroller.h"

ServiceController::ServiceController(QmlDocument* context, BrisaControlPoint *controlPoint) {
	// TODO Auto-generated constructor stub
	deviceList = DeviceList::getDeviceListInstance();
	this->controlPoint = controlPoint;
	servicesDataModel = new ArrayDataModel();
	context->documentContext()->setContextProperty("actionController", new ActionController());
}

ServiceController::~ServiceController() {
	// TODO Auto-generated destructor stub
}

void ServiceController::setDataModel(QVariantList deviceItem) {
	QList<BrisaControlPointService *> listService = deviceList->getDevice(deviceItem.at(0).toInt())->getServiceList();

	servicesDataModel->clear();

	    for (int i = 0; i < listService.size(); i++) {
//	        BrisaEventProxy *subscription = controlPoint->getSubscriptionProxy(
//	                listService[i]);

//	        connect(subscription,SIGNAL(eventNotification(BrisaEventProxy *,QMap<QString, QString>)),
//	                this, SLOT(changeEventLog(BrisaEventProxy *,QMap<QString, QString>)));
//	        subscription->subscribe(20);
//
//	        QList<QString> deviceAndService;
//
//	        deviceAndService.append(currentDev->getAttribute(
//	                BrisaControlPointDevice::FriendlyName));
//	        deviceAndService.append(listService[i]->getAttribute(
//	                BrisaControlPointService::ServiceType));
//
//	        eventToDevice[subscription->getId()] = deviceAndService;

	    	QVariantMap serviceItem;//new QTreeWidgetItem(deviceItem);
	        serviceItem["icon"] = QIcon("qrc:///assets/images/bt_view.png");
	        serviceItem["type"] = listService[i]->getAttribute(BrisaControlPointService::ServiceType);
	        servicesDataModel->append(serviceItem);
	    }
}

ArrayDataModel *ServiceController::getDataModel() {
	return servicesDataModel;
}

QString ServiceController::getDeviceName() {
	return deviceList->getCurrentDevice()->getAttribute(BrisaControlPointDevice::FriendlyName);
}
