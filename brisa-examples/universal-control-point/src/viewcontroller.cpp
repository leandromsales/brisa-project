#include "viewcontroller.h"

ViewController::ViewController(QmlDocument *context, QObject *parent) :
		QObject(parent) {
	this->uiCtx = context;

	this->currentDev = 0;
	this->currentDeviceItem = 0;

	devices = DeviceList::getDeviceListInstance();

	controlPointDataModel = new QListDataModel<QVariantMap>();

	controlPoint = new BrisaControlPoint();

    uiCtx->setContextProperty("serviceController", new ServiceController(context, controlPoint));

	connect(controlPoint, SIGNAL(deviceGone(QString)), this, SLOT(removeDevice(
					QString)));
	connect(controlPoint, SIGNAL(deviceFound(BrisaControlPointDevice*)), this,
			SLOT(deviceFoundDump(BrisaControlPointDevice*)));

	controlPoint->start();
	controlPoint->discover();

//    connect(treeWidgetCP, SIGNAL(itemClicked(QTreeWidgetItem * , int )),
//            this, SLOT(lineEnabled(QTreeWidgetItem * , int)));
//    connect(treeWidgetCP, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
//            this, SLOT(call(QTreeWidgetItem *, int)));

	connect(controlPoint, SIGNAL(multicastReceived(QString,QString)), this,
			SLOT(multicastEventReceived(QString,QString)));
	connect(controlPoint, SIGNAL(multicastReceivedRaw(BrisaOutArgument)), this,
			SLOT(multicastEventRawReceived(BrisaOutArgument)));

//    networkItem = new QTreeWidgetItem(treeWidgetCP);
//    networkItem->setIcon(0, QIcon(
//            "../pixmaps/brisa/network.png"));
//    networkItem->setText(0, "UPnP Network");
}

ViewController::~ViewController() {
	controlPoint->stop();
	delete controlPoint;
}

void ViewController::goesToQML() {
	this->uiCtx->setContextProperty("mainViewController", this);
}

void ViewController::onReadyDownloadIcons(BrisaControlPointDevice *device) {
	//Implementar uso de icones
    currentDeviceItem->insert("_icon", device->getIconList().at(0)->getIcon().name());
    addItem(currentDeviceItem);
}

//void ViewController::lineEnabled(QTreeWidgetItem* item, int collumn) {
//}

void ViewController::deviceFoundDump(BrisaControlPointDevice *device) {
	if (device->getAttribute(BrisaControlPointDevice::Udn).compare("") == 0)
		return;
	for (int i = 0; i < devices->size(); i++) {
		if ((*devices)[i]->getAttribute(BrisaControlPointDevice::Udn)
				== device->getAttribute(BrisaControlPointDevice::Udn)) {
			qDebug()
					<< device->getAttribute(
							BrisaControlPointDevice::FriendlyName) + " - "
							+ device->getAttribute(
									BrisaControlPointDevice::Udn);
			(*devices)[i] = device;
			return;
		}
	}
	devices->append(device);
	this->currentDev = device;
    createDeviceItem();
}

void ViewController::serviceCall(BrisaOutArgument qMap, QString qString) {
//    QString returnMessage = "";
//    QMapIterator<QString, QString> it(arguments);
//    while (it.hasNext()) {
//        it.next();
//        returnMessage.append(it.key() + " = " + it.value() + "\n");
//    }
//
//    textEdit->setText("Calling method: " + method + "\n\n" + "Returned: \n"
//                      + returnMessage);
}

void ViewController::requestError(QString errorMessage, QString methodName) {
}

void ViewController::removeDevice(QString usn) {
//    for (int i = 0; i < items.size(); i++) {
//        bool change;
//        if (items[i]->text(0).split(" - ")[1].compare(usn.split("::")[0]) == 0) {
//            change = false;
//            if ((treeWidgetCP->selectedItems().size() > 0)
//                    && (!treeWidgetCP->selectedItems().contains(items[i]))) {
//                for (int j = 0; j < items[i]->childCount(); j++) {
//                    if (change || treeWidgetCP->selectedItems().contains(
//                            items[i]->child(j))) {
//                        change = true;
//                        break;
//                    }
//                    for (int w = 0; w < items[i]->child(j)->childCount(); w++) {
//                        if (treeWidgetCP->selectedItems().contains(
//                                items[i]->child(j)->child(w))) {
//                            if (closeable)
//                                dialog->close();
//                            change = true;
//                            break;
//                        }
//                    }
//                }
//            }
//            if (change || treeWidgetCP->selectedItems().contains(items[i]))
//                doubleClick = false;
//
//            QString UDN = items[i]->text(0).split(" - ")[1];
//            devices.removeOne(getDeviceByUDN(UDN));
//            delete items[i];
//            items.removeAt(i);
//        }
//    }
}


void ViewController::call() {
//	Q_UNUSED(item);
//	    Q_UNUSED(collumn);
//	    if (doubleClick) {
//	        if (selected->parent() != NULL && selected->parent()->parent() != NULL
//	                && selected->parent()->parent()->parent()) {
//	            mountControlDialog();
//	            dialog->exec();
//	        }
//	    }
}

void ViewController::callMethod() {
//    QString action = selected->text(0);
//    QString service = selected->parent()->text(0);
//    QString udn = selected->parent()->parent()->text(0).split(" - ")[1];
//    BrisaControlPointDevice* dev = getDeviceByUDN(udn);
//    BrisaControlPointService* serv = dev->getServiceByType(service);
//    connect(serv, SIGNAL(requestFinished(BrisaOutArgument, QString)), this, SLOT(
//            serviceCall(BrisaOutArgument, QString)));
//    connect(serv, SIGNAL(requestError(QString, QString)), this, SLOT(
//            requestError(QString,QString)));
//
//    BrisaInArgument parameters;
//    for (int i = 0; i < labels.size(); i++) {
//        QString paramName = labels[i]->text().replace("       -", "");
//        QString value = editors[i]->text();
//        parameters[paramName] = value;
//    }
//    serv->call(action, parameters);
}

void ViewController::expandItems() {
}

void ViewController::collapseItems() {
}

void ViewController::aboutControlPoint() {
}

void ViewController::aboutUpnp() {
}

void ViewController::aboutBrisa() {
}

void ViewController::processSplashScreen() {
}

//void ViewController::call(QTreeWidgetItem* item, int collumn) {
//}

void ViewController::mountControlDialog() {
}

void ViewController::changeEventLog(BrisaEventProxy* subscription,
		QMap<QString, QString> eventVariables) {
//	Q_UNUSED(subscription);
//	    QList<QString> deviceService = eventToDevice[subscription->getId()];
//	    qDebug() << "Unicast event message received:";
//	    for (int i = 0; i < eventVariables.keys().size(); i++) {
//	        qDebug() << "State variable " << eventVariables.keys()[i] <<
//	                "changed value to " << eventVariables[eventVariables.keys()[i]];
//	        int row = tableWidget->rowCount() +1;
//	        tableWidget->setRowCount(row);
//	        QDateTime current = QDateTime::currentDateTime();
//	        QTableWidgetItem *timeItem = new QTableWidgetItem(current.toString());
//	        timeItem->setTextAlignment(Qt::AlignVCenter);
//	        QTableWidgetItem *deviceNameItem = new QTableWidgetItem(deviceService[0]);
//	        deviceNameItem->setTextAlignment(Qt::AlignVCenter);
//	        QTableWidgetItem *serviceNameItem = new QTableWidgetItem(deviceService[1]);
//	        serviceNameItem->setTextAlignment(Qt::AlignVCenter);
//	        QTableWidgetItem *variableNameItem = new QTableWidgetItem(eventVariables.keys()[i]);
//	        QTableWidgetItem *valueItem = new QTableWidgetItem(eventVariables[eventVariables.keys()[i]]);
//	        tableWidget->setItem(row-1, 0, timeItem);
//	        tableWidget->setItem(row-1, 1, deviceNameItem);
//	        tableWidget->setItem(row-1, 2, serviceNameItem);
//	        tableWidget->setItem(row-1, 3, variableNameItem);
//	        tableWidget->setItem(row-1, 4, valueItem);
//	        tableWidget->resizeColumnsToContents();
//	    }
}

void ViewController::clearEventLog() {
}

void ViewController::multicastEventReceived(QString variableName,
		QString newValue) {
}

void ViewController::multicastEventRawReceived(BrisaOutArgument raw) {
}

void ViewController::createDeviceItem() {

	currentDeviceItem = new QVariantMap();

	currentDeviceItem->insert("name", currentDev->getAttribute(BrisaControlPointDevice::FriendlyName));
	currentDeviceItem->insert("udn", currentDev->getAttribute(BrisaControlPointDevice::Udn));
	currentDeviceItem->insert("deviceType", currentDev->getAttribute(BrisaControlPointDevice::deviceType));

	controlPointDataModel->append(*currentDeviceItem);
// Colocar um item na list
//    if(currentDev->getIconList().size() > 0){
//        connect(currentDev, SIGNAL(onReadyDownloadIcons(BrisaControlPointDevice*)),
//                this, SLOT(onReadyDownloadIcons(BrisaControlPointDevice*)));
//        currentDev->downloadIcons();
//    } else {
////    	(*currentDeviceItem)["_icon"] = "qrc://assets/images/device.png";
//        addItem(currentDeviceItem);
//    }
}

void ViewController::createActions() {
}

void ViewController::createMenus() {
}

void ViewController::createToolBars() {
}

void ViewController::setUpTableWidget() {
}

void ViewController::addItem(QVariantMap *deviceItem) {
//    items.append(deviceItem);
//    deviceItem->insert("_services", new QList<QVariantMap>());
//    QList<BrisaControlPointService *> listService = currentDev->getServiceList();
//
//    for (int i = 0; i < listService.size(); i++) {
//        BrisaEventProxy *subscription = controlPoint->getSubscriptionProxy(
//                listService[i]);
//
//        connect(subscription,SIGNAL(eventNotification(BrisaEventProxy *,QMap<QString, QString>)),
//                this, SLOT(changeEventLog(BrisaEventProxy *,QMap<QString, QString>)));
//        subscription->subscribe(20);
//
//        QList<QString> deviceAndService;
//
//        deviceAndService.append(currentDev->getAttribute(
//                BrisaControlPointDevice::FriendlyName));
//        deviceAndService.append(listService[i]->getAttribute(
//                BrisaControlPointService::ServiceType));
//
//        eventToDevice[subscription->getId()] = deviceAndService;
//
//        QVariantMap *serviceItem = new QVariantMap();//new QTreeWidgetItem(deviceItem);
//        deviceItem->take("_services").toList().append(*serviceItem);
//        serviceItem->insert("_icon", QIcon("qrc:///assets/images/bt_view.png"));
//        serviceItem->insert("_title", listService[i]->getAttribute(BrisaControlPointService::ServiceType));
//        serviceItem->insert("_actions", new QList<QVariantMap>());
//
//        QList<BrisaAction *> listAction;
//        listAction = listService[i]->getActionList();
//
//        for (int j = 0; j < listAction.size(); j++) {
//            QVariantMap *actionItem = new QVariantMap();//new QTreeWidgetItem(serviceItem);
//            serviceItem->take("_actions").toList().append(*actionItem);
//            actionItem->insert("_icon", QIcon("qrc:///assets/images/bt_view.png"));
//            actionItem->insert("_title", listAction[j]->getName());
//        }
//    }
}

BrisaControlPointDevice *ViewController::getDeviceByUDN(QString UDN) {
//    for (int i = 0; i < devices.size(); i++) {
//        if (UDN.compare(devices[i]->getAttribute(BrisaControlPointDevice::Udn))
//                == 0)
//            return devices[i];
//    }
//    return NULL;
}

QListDataModel<QVariantMap> *ViewController::getControlPointModel() {
	return controlPointDataModel;
}
