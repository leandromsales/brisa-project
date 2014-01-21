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
	Brisa::BrisaAction* getAction();
	void setAction(Brisa::BrisaAction* action);
	Brisa::BrisaControlPointDevice* getDevice();
	void setDevice(Brisa::BrisaControlPointDevice* device);
	Brisa::BrisaControlPointService* getService();
	void setService(Brisa::BrisaControlPointService* service);
	Q_INVOKABLE QString getDeviceFriendlyName();
	Q_INVOKABLE QString getServiceFriendlyName();
	Q_INVOKABLE QString getActionName();
	Q_INVOKABLE QString whyIsFail();
	void popComponent();
	static QString friendlyNameFromServiceType(QString serviceType);

private:
	ExecutableAction(QObject *parent = 0);
	Brisa::BrisaAction *m_action;
	Brisa::BrisaControlPointService *m_service;
	Brisa::BrisaControlPointDevice *m_currentDevice;
	QStack<Brisa::BrisaControlPointDevice *> *m_devices;
};

#endif /* EXECUTABLEACTION_HPP_ */
