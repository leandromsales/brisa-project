/*
 * SwitchPower.hpp
 *
 *  Created on: Sep 21, 2013
 *      Author: rodrigopex
 */

#ifndef SWITCHPOWER_HPP_
#define SWITCHPOWER_HPP_

#include <QObject>
#include <QtCore>
#include <upnp/brisaaction.h>
#include <upnp/device/brisaservice.h>
#include <upnp/brisaabstractservice.h>
#include <upnp/brisastatevariable.h>

#define SERVICE_TYPE "urn:schemas-upnp-org:service:SwitchPower:1"
#define SERVICE_ID "SwitchPower"
#define SERVICE_XML_PATH "/SwitchPower/SwitchPower-scpd.xml"
#define SERVICE_CONTROL "/SwitchPower/control"
#define SERVICE_EVENT_SUB "/SwitchPower/eventSub"

using namespace Brisa;

class SwitchPower : public BrisaService
{
    Q_OBJECT
public:
    SwitchPower();

//public slots:

	Q_INVOKABLE BrisaOutArgument* GetStatus(BrisaInArgument * const inArguments,
                                BrisaAction * const action);

	Q_INVOKABLE BrisaOutArgument* GetTarget(BrisaInArgument * const inArguments,
                                BrisaAction * const action);

	Q_INVOKABLE BrisaOutArgument* SetTarget(BrisaInArgument*const, BrisaAction *const);
};

#endif /* SWITCHPOWER_HPP_ */
