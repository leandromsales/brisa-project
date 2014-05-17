/*
 * BinaryLight.cpp
 *
 *  Created on: 22/09/2013
 *      Author: wendell
 */

#include "BinaryLight.h"
#include "SwitchPower.hpp"
#include "upnp/brisautils.h"

using namespace brisa::upnp;
using namespace brisa::upnp::device;

BinaryLight::BinaryLight(ApplicationUI *ui) {

	m_ui = ui;
	QString completeUUID = getCompleteUuid();
	qDebug() << "My UUID" << completeUUID;
	m_binaryLight = new Device(DEVICE_TYPE,
			DEVICE_FRIENDLY_NAME, DEVICE_MANUFACTURER, DEVICE_MANUFACTURER_URL,
			DEVICE_MODEL_DESCRIPTION, DEVICE_MODEL_NAME, DEVICE_MODEL_NUMBER,
			DEVICE_MODEL_URL, DEVICE_SERIAL_NUMBER, completeUUID);

	SwitchPower *switchPower = new SwitchPower();
	switchPower->setDescriptionFile("app/native/assets/SwitchPower-scpd.xml");

	m_binaryLight->addService(switchPower);
}

void BinaryLight::start() {

	m_binaryLight->start();

	m_status = m_binaryLight->getServiceByType(
			"urn:schemas-upnp-org:service:SwitchPower:1")->getVariable(
			"Status");
	m_target = m_binaryLight->getServiceByType(
			"urn:schemas-upnp-org:service:SwitchPower:1")->getVariable(
			"Target");

	bool isOk = connect(m_status, SIGNAL(changed(StateVariable *)), m_ui,
			SLOT(stateChanged(StateVariable *)));
	Q_ASSERT(isOk);
	Q_UNUSED(isOk);
}

void BinaryLight::uiStateChanged(bool check) {
	if (m_ui->isConnected()) {
		if (check) {
			m_status->setAttribute(StateVariable::Value, "1");
			m_target->setAttribute(StateVariable::Value, "1");
		} else {
			m_status->setAttribute(StateVariable::Value, "0");
			m_target->setAttribute(StateVariable::Value, "0");
		}
	}
}

void BinaryLight::byebye() {
	m_binaryLight->stop();
}
