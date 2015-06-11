#ifndef CONTROLPOINTBCU_H
#define CONTROLPOINTBCU_H

#include "upnp/controlpoint/ControlPoint"

#include <QtCore>
#include <QtDebug>
#include <QNetworkAccessManager>

namespace brisa {
namespace upnp {
namespace controlpoint {

// The control point used on BCU is a custom control point
class ControlPointBCU : public ControlPoint {
    Q_OBJECT
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
