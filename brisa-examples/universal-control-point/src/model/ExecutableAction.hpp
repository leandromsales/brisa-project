/*
 * ExecutableAction.hpp
 *
 *  Created on: 24/11/2013
 *      Author: daniel
 */

#ifndef EXECUTABLEACTION_HPP_
#define EXECUTABLEACTION_HPP_

#include <QStack>
#include <QObject>
#include <QString>
#include <QStringList>

#include "upnp/brisaaction.h"
#include "upnp/controlpoint/brisacontrolpointdevice.h"
#include "upnp/controlpoint/brisacontrolpointservice.h"

class ExecutableAction: public QObject {
	Q_OBJECT
public:
	static ExecutableAction *getInstance(QObject *parent = 0) {
		static ExecutableAction *m_executableAction;
		if(!m_executableAction) {
			m_executableAction = new ExecutableAction(parent);
		}
		return m_executableAction;
	}
	virtual ~ExecutableAction();
	bool isComplete();
	brisa::upnp::BrisaAction* getAction();
	void setAction(brisa::upnp::BrisaAction* action);
	brisa::upnp::controlpoint::BrisaControlPointDevice* getDevice();
	void setDevice(brisa::upnp::controlpoint::BrisaControlPointDevice* device);
	brisa::upnp::controlpoint::BrisaControlPointService* getService();
	void setService(brisa::upnp::controlpoint::BrisaControlPointService* service);
	Q_INVOKABLE QString getDeviceFriendlyName();
	Q_INVOKABLE QString getServiceFriendlyName();
	Q_INVOKABLE QString getActionName();
	Q_INVOKABLE QString whyIsFail();
	void popComponent();
	static QString friendlyNameFromServiceType(QString serviceType);

private:
	ExecutableAction(QObject *parent = 0);
	brisa::upnp::BrisaAction *m_action;
	brisa::upnp::controlpoint::BrisaControlPointService *m_service;
	brisa::upnp::controlpoint::BrisaControlPointDevice *m_currentDevice;
	QStack<brisa::upnp::controlpoint::BrisaControlPointDevice *> *m_devices;
};

#endif /* EXECUTABLEACTION_HPP_ */
