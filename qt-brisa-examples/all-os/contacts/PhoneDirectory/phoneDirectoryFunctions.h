#ifndef PHONEDIRECTORYFUNCTIONS_H
#define PHONEDIRECTORYFUNCTIONS_H

#define SERVICE_TYPE "urn:schemas-upnp-org:service:PhoneDirectoryFunctions:1"
#define SERVICE_ID "PhoneDirectoryFunctions"
#define SERVICE_XML_PATH "/PhoneDirectoryFunctions/PhoneDirectoryFunctions.xml"
#define SERVICE_CONTROL "/PhoneDirectoryFunctions/control"
#define SERVICE_EVENT_SUB "/PhoneDirectoryFunctions/eventSub"

#include <BrisaUpnp/BrisaAction>
#include <BrisaUpnp/BrisaService>
#include <BrisaUpnp/brisastatevariable.h>
#include <QtDebug>

using Brisa::BrisaStateVariable;
using Brisa::BrisaService;
using Brisa::BrisaAction;

class PhoneDirectoryFunctions : public BrisaService
{
    Q_OBJECT

public:
    PhoneDirectoryFunctions();
    Q_INVOKABLE
    BrisaOutArgument* search(BrisaInArgument* const inArguments, BrisaAction* const action);    
};

#endif // PHONEDIRECTORYFUNCTIONS_H
