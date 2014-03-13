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

#include "upnp/brisaaction.h"
#include "upnp/controlpoint/brisacontrolpointdevice.h"
#include "upnp/controlpoint/brisacontrolpointservice.h"

#include "model/ExecutableAction.hpp"

class DeviceList: public QList<brisa::upnp::controlpoint::BrisaControlPointDevice *> {
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
	brisa::upnp::controlpoint::BrisaControlPointDevice *getDevice(int index);
	brisa::upnp::controlpoint::BrisaControlPointDevice *getCurrentDevice();
	brisa::upnp::controlpoint::BrisaControlPointDevice *getDeviceByUDN(QString UDN);

private:
	DeviceList();
	brisa::upnp::controlpoint::BrisaControlPointDevice *m_currentDevice;
	ExecutableAction *m_executableAction;
};

#endif /* DEVICELIST_H_ */
