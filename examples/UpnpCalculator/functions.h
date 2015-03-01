#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define SERVICE_TYPE "urn:schemas-upnp-org:service:Functions:1"
#define SERVICE_ID "Functions"
#define SERVICE_XML_PATH "/Functions/Functions.xml"
#define SERVICE_CONTROL "/Functions/control"
#define SERVICE_EVENT_SUB "/Functions/eventSub"

#include "upnp/action.h"
#include "upnp/device/service.h"
#include <QtDebug>

class Functions : public brisa::upnp::device::Service
{
    Q_OBJECT

public:
    Functions();

private slots:

    OutArgument* soma(InArgument * const inArguments, brisa::upnp::Action * const action);
    OutArgument* subtracao(InArgument * const inArguments, brisa::upnp::Action * const action);
    OutArgument* multiplicacao(InArgument * const inArguments, brisa::upnp::Action * const action);
    OutArgument* divisao(InArgument * const inArguments, brisa::upnp::Action * const action);


};

#endif // FUNCTIONS_H
