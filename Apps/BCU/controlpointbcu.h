#ifndef CONTROLPOINTBCU_H
#define CONTROLPOINTBCU_H

#include "upnp/controlpoint/ControlPoint"

namespace brisa {
namespace upnp {
namespace controlpoint {

// The control point used on BCU is a custom control point
class ControlPointBCU: ControlPoint {
public:
    ControlPointBCU(QObject *parent = 0, QString st = "ssdp:all", int mx = 5);
    ~ControlPointBCU();

private slots:
    void replyFinished(QNetworkReply *reply);


};

} // end namespace brisa
} // end namespace upnp
} // end namespace controlpoint


#endif // CONTROLPOINTBCU_H
