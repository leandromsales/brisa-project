/*
 * servicecontroller.h
 *
 *  Created on: 16/09/2013
 *      Author: daniel
 */

#ifndef SERVICECONTROLLER_H_
#define SERVICECONTROLLER_H_

#include <QObject>
#include <QDebug>
#include <QList>
#include <QVariant>
#include <QVariantList>
#include <QVariantMap>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/ArrayDataModel>
#include <bb/cascades/ListView>

#include "upnp/controlpoint/brisacontrolpoint.h"
#include "upnp/controlpoint/brisacontrolpointdevice.h"
#include "upnp/controlpoint/brisacontrolpointservice.h"

#include "devicelist.h"
#include "actioncontroller.h"

using namespace bb::cascades;
using namespace Brisa;


class ServiceController : public QObject {
	Q_OBJECT
public:
	ServiceController(QmlDocument* context, BrisaControlPoint *controlPoint);
	virtual ~ServiceController();
	Q_INVOKABLE void setDataModel(QVariantList deviceItem);
	Q_INVOKABLE ArrayDataModel *getDataModel();
	Q_INVOKABLE QString getDeviceName();
private:
	QString deviceName;
	DeviceList *deviceList;
	BrisaControlPoint *controlPoint;
	ArrayDataModel *servicesDataModel;
};

#endif /* SERVICECONTROLLER_H_ */
