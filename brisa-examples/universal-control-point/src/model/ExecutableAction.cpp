/*
 * ExecutableAction.cpp
 *
 *  Created on: 24/11/2013
 *      Author: daniel
 */

#include "ExecutableAction.hpp"

using namespace brisa::upnp;
using namespace brisa::upnp::controlpoint;

ExecutableAction::ExecutableAction(QObject *parent) :
		QObject(parent) {
	m_action = 0;
	m_service = 0;
	m_devices = new QStack<Device *>();
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

Action* ExecutableAction::getAction() {
	return m_action;
}

void ExecutableAction::setAction(Action* action) {
	m_action = action;
}

Device* ExecutableAction::getDevice() {
	return m_currentDevice;
}

void ExecutableAction::setDevice(Device* device) {
	m_action = 0;
	m_service = 0;
	if (m_currentDevice)
		m_devices->push(m_currentDevice);
	m_currentDevice = device;
}

Service* ExecutableAction::getService() {
	return m_service;
}

void ExecutableAction::setService(Service* service) {
	m_action = 0;
	m_service = service;
}

QString ExecutableAction::getDeviceFriendlyName() {
	return m_currentDevice->getAttribute(Device::friendlyName);
}

QString ExecutableAction::getServiceFriendlyName() {
	return friendlyNameFromServiceType(
			m_service->getAttribute(Service::ServiceType));
}

QString ExecutableAction::getActionName() {
	return m_action->getName();
}

QString ExecutableAction::friendlyNameFromServiceType(QString serviceType) {
	QStringList names = serviceType.split(":");
	return names[names.length() - 2];
}
