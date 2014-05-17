/*
 * DeviceList.hpp
 *
 *  Created on: 16/09/2013
 *      Author: daniel
 */

#ifndef DEVICELIST_H_
#define DEVICELIST_H_

#include <QList>
#include <QDebug>
#include <QString>
#include <QObject>
#include <QVariantMap>

#include "upnp/action.h"
#include "upnp/controlpoint/cpdevice.h"
#include "upnp/controlpoint/cpservice.h"

#include "model/ExecutableAction.hpp"

class DeviceList: public QList<brisa::upnp::controlpoint::Device *> {
public:
	enum types {
		DEVICE, SERVICE, ACTION
	};

	static DeviceList *getInstance() {
		static DeviceList *deviceList;
		if (!deviceList) {
			deviceList = new DeviceList();
		}
		return deviceList;
	}
	virtual ~DeviceList() {}
	void popCurrentItem();
	int setCurrentItemByID(QString ID);
	brisa::upnp::controlpoint::Device *getDevice(int index);
	brisa::upnp::controlpoint::Device *getCurrentDevice();
	brisa::upnp::controlpoint::Device *getDeviceByUDN(QString UDN);

private:
	DeviceList();
	brisa::upnp::controlpoint::Device *m_currentDevice;
	ExecutableAction *m_executableAction;
};

#endif /* DEVICELIST_H_ */
