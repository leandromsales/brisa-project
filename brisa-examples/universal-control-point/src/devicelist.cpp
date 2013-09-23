/*
 * devicelist.cpp
 *
 *  Created on: 16/09/2013
 *      Author: daniel
 */

#include "devicelist.h"
DeviceList::DeviceList() {
	currentDevice = 0;
}

BrisaControlPointDevice* DeviceList::getDevice(int index) {
	currentDevice = this->at(index);
	return currentDevice;
}

BrisaControlPointDevice* DeviceList::getCurrentDevice() {
	return currentDevice;
}
