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

#include "upnp/Action"
#include "upnp/controlpoint/Device"
#include "upnp/controlpoint/Service"

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
	brisa::upnp::Action* getAction();
	void setAction(brisa::upnp::Action* action);
	brisa::upnp::controlpoint::Device* getDevice();
	void setDevice(brisa::upnp::controlpoint::Device* device);
	brisa::upnp::controlpoint::Service* getService();
	void setService(brisa::upnp::controlpoint::Service* service);
	Q_INVOKABLE QString getDeviceFriendlyName();
	Q_INVOKABLE QString getServiceFriendlyName();
	Q_INVOKABLE QString getActionName();
	Q_INVOKABLE QString whyIsFail();
	void popComponent();
	static QString friendlyNameFromServiceType(QString serviceType);

private:
	ExecutableAction(QObject *parent = 0);
	brisa::upnp::Action *m_action;
	brisa::upnp::controlpoint::Service *m_service;
	brisa::upnp::controlpoint::Device *m_currentDevice;
	QStack<brisa::upnp::controlpoint::Device *> *m_devices;
};

#endif /* EXECUTABLEACTION_HPP_ */
