/*
 * actioncontroller.cpp
 *
 *  Created on: 17/09/2013
 *      Author: daniel
 */

#include "actioncontroller.h"

ActionController::ActionController() {
	// TODO Auto-generated constructor stub
	actionsDataModel = new ArrayDataModel();
	deviceList = DeviceList::getDeviceListInstance();
}

ActionController::~ActionController() {
	// TODO Auto-generated destructor stub
}

void ActionController::setDataModel(QVariantMap serviceMap) {
	serviceType.clear();
	serviceType.append(serviceMap["type"].toString());
	BrisaControlPointService* service = deviceList->getCurrentDevice()->getServiceByType(serviceType);
	QList<BrisaAction *> listAction = service->getActionList();

	actionsDataModel->clear();
	for (int j = 0; j < listAction.size(); j++) {
		QVariantMap actionItem; //new QTreeWidgetItem(serviceItem);
		actionItem["icon"] = QIcon("asset:///images/bt_view.png");
		actionItem["name"] = listAction[j]->getName();
		actionsDataModel->append(actionItem);
	}
}

ArrayDataModel* ActionController::getDataModel() {
	return actionsDataModel;
}

QString ActionController::getServiceType() {
	return serviceType;
}

void ActionController::setCurrentAction(QString action) {
	currentAction = deviceList->getCurrentDevice()->getServiceByType(serviceType)->getAction(action);
}

QVariantMap ActionController::getArgumentsMap() {
//	if(currentAction) {
//	    QList<BrisaArgument *> inArguments;
//	    for (int i = 0; i < currentAction->getArgumentList().size(); i++) {
//	        if ((currentAction->getArgumentList()[i])->getAttribute(
//	                BrisaArgument::Direction).compare("in") == 0) {
//	            inArguments.append(currentAction->getArgumentList()[i]);
//	        }
//	    }
//	    if (inArguments.size() > 0) {
//	        QmlDocument *qml = QmlDocument::create("asset:///qml/ControlSheet.qml");
//	        qml->documentContext()->setContextProperty("actionController", this);
//	        Page *root = qml->createRootObject<Page>();
//	        Container *rootContainer = root->findChild<Container *>("root_container");
//	        if(!rootContainer)
//	        	qDebug() << "Eu sou 0";
//	    	Label *a = new Label();
//	        a->setText("  ");
//	    	Label *in = new Label();
//	        in->setText("'in' Arguments:");
//	        rootContainer->con//	        vertical->addWidget(a);
////	        vertical->addWidget(in);
//	        for (int i = 0; i < inArguments.size(); i++) {
//		    	Label *parameter = new Label();
//	            parameter->setText("       - " + inArguments[i]->getAttribute(
//	                    BrisaArgument::ArgumentName));
////	            labels.append(parameter);
//
//	            TextField lineEdit = new TextField();
////	            editors.append(lineEdit);
//
////	            QHBoxLayout *temp = new QHBoxLayout();
////	            temp->addWidget(parameter);
////	            temp->addWidget(lineEdito);
////	            vertical->addLayout(temp);
//	        }
//	    }
//		return new QVariantMap();
//	}
	return QVariantMap();
}
