#ifndef TVOPERATIONS_H
#define TVOPERATIONS_H

#define SERVICE_TYPE "urn:schemas-upnp-org:service:TVOperations:1"
#define SERVICE_ID "TVOperations"
#define SERVICE_XML_PATH "/TVOperations/TVOperations.xml"
#define SERVICE_CONTROL "/TVOperations/control"
#define SERVICE_EVENT_SUB "/TVOperations/eventSub"
#include <BRisa/BrisaUpnp/BrisaAction>
#include <BRisa/BrisaUpnp/BrisaService>
#include <BRisa/BrisaUpnp/brisastatevariable.h>
#include <QtDebug>

using namespace Brisa;


class TVOperations : public BrisaService
{
        Q_OBJECT

    public:
        TVOperations();
        Q_INVOKABLE
        int preAction(BrisaInArgument* const inArguments, BrisaAction* const action, QString &errorDescription);
        Q_INVOKABLE
        int postAction(BrisaInArgument* const inArguments, BrisaOutArgument* const outArguments, BrisaAction* const action, QString &errorDescription);
        Q_INVOKABLE
        BrisaOutArgument* offon(BrisaInArgument* const inArguments, BrisaAction* const action);
        Q_INVOKABLE
        BrisaOutArgument* setchanel(BrisaInArgument* const inArguments, BrisaAction* const action);


    };

#endif // TVOPERATIONS_H
