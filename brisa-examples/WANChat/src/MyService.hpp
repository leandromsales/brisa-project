/*
 * MyService.hpp
 *
 *  Created on: Sep 21, 2013
 *      Author: rodrigopex
 */

#ifndef MYSERVICE_HPP_
#define MYSERVICE_HPP_

#include <QObject>
#include <QtCore>
#include <upnp/brisaaction.h>
#include <upnp/device/brisaservice.h>
#include <upnp/brisaabstractservice.h>

#define SERVICE_TYPE "urn:schemas-upnp-org:service:MyService:1"
#define SERVICE_ID "MyService"

using namespace Brisa;

class MyService: public BrisaService {
	Q_OBJECT
public:
	MyService(QObject * parent = 0) :
			BrisaService(SERVICE_TYPE, SERVICE_ID) {
	}
	Q_INVOKABLE
	BrisaOutArgument* actionName1(BrisaInArgument* const inArguments,
			BrisaAction* const action);
private:
	Q_INVOKABLE
	BrisaOutArgument* actionName2(BrisaInArgument* const inArguments,
			BrisaAction* const action);
private slots:
	BrisaOutArgument* actionName3(BrisaInArgument* const inArguments,
			BrisaAction* const action);
};

#endif /* MYSERVICE_HPP_ */
