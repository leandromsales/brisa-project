#ifndef MEDIARENDERERDEVICESERVICE_H
#define MEDIARENDERERDEVICESERVICE_H

#include "upnp/device/service.h"
#include "upnp/action.h"

#define SERVICE_TYPE "urn:schemas-upnp-org:service:TVOperations:1"
#define SERVICE_ID "TVOperations"
#define SERVICE_XML_PATH "/TVOperations/TVOperations.xml"
#define SERVICE_CONTROL "/TVOperations/control"
#define SERVICE_EVENT_SUB "/TVOperations/eventSub"

namespace brisa {
namespace upnp {
namespace device {
namespace av {

class MediaRendererDeviceService : public Service
{
    Q_OBJECT
public:
    explicit MediaRendererDeviceService(QObject *parent = 0);
    Q_INVOKABLE
    int preAction(BrisaInArgument* const inArguments,
                  Action* const action,
                  QString &errorDescription);
    Q_INVOKABLE
    int postAction(BrisaInArgument* const inArguments,
                   BrisaOutArgument* const outArguments,
                   Action* const action,
                   QString &errorDescription);
    Q_INVOKABLE
    BrisaOutArgument* offon(BrisaInArgument* const inArguments,
                            Action* const action);
    Q_INVOKABLE
    BrisaOutArgument* setchanel(BrisaInArgument* const inArguments,
                                Action* const action);

signals:

public slots:

};

}  // namespace av
}
}
}

#endif // MEDIARENDERERDEVICESERVICE_H
