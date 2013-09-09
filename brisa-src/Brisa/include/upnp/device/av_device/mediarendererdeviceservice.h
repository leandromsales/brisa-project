#ifndef MEDIARENDERERDEVICESERVICE_H
#define MEDIARENDERERDEVICESERVICE_H

#include "upnp/device/brisaservice.h"
#include "upnp/brisaaction.h"

#define SERVICE_TYPE "urn:schemas-upnp-org:service:TVOperations:1"
#define SERVICE_ID "TVOperations"
#define SERVICE_XML_PATH "/TVOperations/TVOperations.xml"
#define SERVICE_CONTROL "/TVOperations/control"
#define SERVICE_EVENT_SUB "/TVOperations/eventSub"

using namespace Brisa;

class MediaRendererDeviceService : public BrisaService
{
    Q_OBJECT
public:
    explicit MediaRendererDeviceService(QObject *parent = 0);
    Q_INVOKABLE
    int preAction(BrisaInArgument* const inArguments,
                  BrisaAction* const action,
                  QString &errorDescription);
    Q_INVOKABLE
    int postAction(BrisaInArgument* const inArguments,
                   BrisaOutArgument* const outArguments,
                   BrisaAction* const action,
                   QString &errorDescription);
    Q_INVOKABLE
    BrisaOutArgument* offon(BrisaInArgument* const inArguments,
                            BrisaAction* const action);
    Q_INVOKABLE
    BrisaOutArgument* setchanel(BrisaInArgument* const inArguments,
                                BrisaAction* const action);
    
signals:
    
public slots:
    
};

#endif // MEDIARENDERERDEVICESERVICE_H
