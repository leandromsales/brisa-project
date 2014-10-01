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
#include <upnp/Action>
#include <upnp/device/Service>
#include <upnp/AbstractService>
#include <upnp/StateVariable>

#define SERVICE_TYPE "urn:schemas-upnp-org:service:SwitchPower:1"
#define SERVICE_ID "SwitchPower"
#define SERVICE_XML_PATH "/SwitchPower/SwitchPower-scpd.xml"
#define SERVICE_CONTROL "/SwitchPower/control"
#define SERVICE_EVENT_SUB "/SwitchPower/eventSub"

class SwitchPower : public brisa::upnp::device::Service
{
    Q_OBJECT
public:
    SwitchPower();

//public slots:

    Q_INVOKABLE OutArgument* GetStatus(InArgument * const inArguments, brisa::upnp::Action * const action);

    Q_INVOKABLE OutArgument* GetTarget(InArgument * const inArguments, brisa::upnp::Action * const action);

    Q_INVOKABLE OutArgument* SetTarget(InArgument* const, brisa::upnp::Action *const);
};

#endif /* SWITCHPOWER_HPP_ */
