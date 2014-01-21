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

class DeviceList: public QList<Brisa::BrisaControlPointDevice *> {
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
	Brisa::BrisaControlPointDevice *getDevice(int index);
	Brisa::BrisaControlPointDevice *getCurrentDevice();
	Brisa::BrisaControlPointDevice *getDeviceByUDN(QString UDN);

private:
	DeviceList();
	Brisa::BrisaControlPointDevice *m_currentDevice;
	ExecutableAction *m_executableAction;
};

#endif /* DEVICELIST_H_ */
