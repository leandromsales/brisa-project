/*
 * ExecutableAction.cpp
 *
 *  Created on: 24/11/2013
 *      Author: daniel
 */

#include "ExecutableAction.hpp"

using namespace brisa::upnp;

ExecutableAction::ExecutableAction(QObject *parent) :
		QObject(parent) {
	m_action = 0;
	m_service = 0;
	m_devices = new QStack<BrisaControlPointDevice *>();
	m_currentDevice = 0;
}

ExecutableAction::~ExecutableAction() {
	if (m_action)
		delete m_action;
	if (m_service)
		delete m_service;
	if (m_devices)
		delete m_devices;
	if (m_currentDevice)
		delete m_currentDevice;
	m_action = 0;
	m_service = 0;
	m_devices = 0;
	m_currentDevice = 0;
}

QString ExecutableAction::whyIsFail() {
	if (!m_currentDevice)
		return "Falta o device.";
	else if (!m_service)
		return "Falta o service.";
	else if (!m_action)
		return "Falta o action.";
	return "Todos ok.";
}

void ExecutableAction::popComponent() {
	if (m_action)
		m_action = 0;
	else if (m_service)
		m_service = 0;
	else if (m_currentDevice) {
		if (!m_devices->isEmpty())
			m_currentDevice = m_devices->pop();
		else
			m_currentDevice = 0;
	}
}

bool ExecutableAction::isComplete() {
	if (m_action != 0 && m_service != 0 && m_currentDevice != 0)
		return true;
	else
		return false;
}

BrisaAction* ExecutableAction::getAction() {
	return m_action;
}

void ExecutableAction::setAction(BrisaAction* action) {
	m_action = action;
}

BrisaControlPointDevice* ExecutableAction::getDevice() {
	return m_currentDevice;
}

void ExecutableAction::setDevice(BrisaControlPointDevice* device) {
	m_action = 0;
	m_service = 0;
	if (m_currentDevice)
		m_devices->push(m_currentDevice);
	m_currentDevice = device;
}

BrisaControlPointService* ExecutableAction::getService() {
	return m_service;
}

void ExecutableAction::setService(BrisaControlPointService* service) {
	m_action = 0;
	m_service = service;
}

QString ExecutableAction::getDeviceFriendlyName() {
	return m_currentDevice->getAttribute(BrisaControlPointDevice::friendlyName);
}

QString ExecutableAction::getServiceFriendlyName() {
	return friendlyNameFromServiceType(
			m_service->getAttribute(BrisaControlPointService::ServiceType));
}

QString ExecutableAction::getActionName() {
	return m_action->getName();
}

QString ExecutableAction::friendlyNameFromServiceType(QString serviceType) {
	QStringList names = serviceType.split(":");
	return names[names.length() - 2];
}
