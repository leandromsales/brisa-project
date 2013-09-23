/*
 * devicelist.h
 *
 *  Created on: 16/09/2013
 *      Author: daniel
 */

#ifndef DEVICELIST_H_
#define DEVICELIST_H_

#include <QObject>
#include <QDebug>
#include <QList>
#include <QVariantMap>
#include "upnp/controlpoint/brisacontrolpointdevice.h"

using namespace Brisa;

class DeviceList : public QList<BrisaControlPointDevice *> {
public:
	static DeviceList *getDeviceListInstance() {
		static DeviceList *deviceList;
		if(!deviceList) {
			deviceList = new DeviceList();
		}
		return deviceList;
	}
	virtual ~DeviceList() {}
	BrisaControlPointDevice *getDevice(int index);
	BrisaControlPointDevice *getCurrentDevice();
private:
	DeviceList();
	BrisaControlPointDevice *currentDevice;
};

#endif /* DEVICELIST_H_ */
