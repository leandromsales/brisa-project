/*
 * DeviceList.cpp
 *
 *  Created on: 16/09/2013
 *      Author: daniel
 */

#include "DeviceList.hpp"

using namespace brisa::upnp;
using namespace brisa::upnp::controlpoint;

DeviceList::DeviceList() {
	m_currentDevice = 0;
	m_executableAction = ExecutableAction::getInstance();
}

int DeviceList::setCurrentItemByID(QString ID) {
	if (!m_executableAction->getDevice()) {
		BrisaControlPointDevice *tempDevice = getDeviceByUDN(ID);
		if (tempDevice != 0) {
			m_executableAction->setDevice(tempDevice);
			return DEVICE;
		}
	} else {
		BrisaControlPointDevice *tempDevice = m_executableAction->getDevice();
		foreach (BrisaControlPointDevice * currDevice, tempDevice->getEmbeddedDeviceList()) {
			if (ID.compare(currDevice->getAttribute(BrisaControlPointDevice::Udn))
					== 0) {
				m_executableAction->setDevice(currDevice);
				return DEVICE;
			}
		}
		BrisaControlPointService *tempService = tempDevice->getServiceById(ID);
		if (tempService) {
			m_executableAction->setService(tempService);
			return SERVICE;
		}
	}
	if (m_executableAction->getService()) {
		BrisaControlPointService *tempService = m_executableAction->getService();
		BrisaAction *tempAction = tempService->getAction(ID);
		if (tempAction) {
			m_executableAction->setAction(tempAction);
			return ACTION;
		}
	}
	return -1;
}

void DeviceList::popCurrentItem() {
	m_executableAction->popComponent();
}

BrisaControlPointDevice* DeviceList::getDevice(int index) {
	m_currentDevice = this->at(index);
	return m_currentDevice;
}

BrisaControlPointDevice* DeviceList::getCurrentDevice() {
	return m_currentDevice;
}

BrisaControlPointDevice *DeviceList::getDeviceByUDN(QString UDN) {
	for (int i = 0; i < this->size(); i++) {
		if (UDN.compare(this->at(i)->getAttribute(BrisaControlPointDevice::Udn))
				== 0)
			return this->at(i);
	}
	return NULL;
}
