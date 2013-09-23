/*
 * BinaryLight.cpp
 *
 *  Created on: 22/09/2013
 *      Author: wendell
 */

#include "BinaryLight.h"
#include "SwitchPower.hpp"

BinaryLight::BinaryLight(ApplicationUI *ui) {

	this->ui = ui;
	QString completeUUID = getCompleteUuid();
	qDebug() << "My UUID" << completeUUID;
	this->binaryLight = new Brisa::BrisaDevice(DEVICE_TYPE, DEVICE_FRIENDLY_NAME,
			DEVICE_MANUFACTURER, DEVICE_MANUFACTURER_URL,
			DEVICE_MODEL_DESCRIPTION, DEVICE_MODEL_NAME, DEVICE_MODEL_NUMBER,
			DEVICE_MODEL_URL, DEVICE_SERIAL_NUMBER, completeUUID);

	SwitchPower *switchPower = new SwitchPower();
	switchPower->setDescriptionFile("app/native/assets/SwitchPower-scpd.xml");

	this->binaryLight->addService(switchPower);
}

void BinaryLight::start() {

	this->binaryLight->start();

	this->status
	= binaryLight->getServiceByType(
			"urn:schemas-upnp-org:service:SwitchPower:1")->getVariable(
					"Status");
	this->target
	= binaryLight->getServiceByType(
			"urn:schemas-upnp-org:service:SwitchPower:1")->getVariable(
					"Target");

	bool x = connect(status, SIGNAL(changed(StateVariable *)), ui, SLOT(statechanged(StateVariable *)));
	qDebug() << "Connect: " << x;

	ui->statechanged(status);
}

