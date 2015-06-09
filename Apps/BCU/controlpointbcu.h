#ifndef CONTROLPOINTBCU_H
#define CONTROLPOINTBCU_H

#include "upnp/controlpoint/ControlPoint"

namespace brisa {
namespace upnp {
namespace controlpoint {

// The control point used on BCU is a custom control point
class ControlPointBCU: ControlPoint {
public:
    ControlPointBCU();
    ~ControlPointBCU();
};
} // end namespace brisa
} // end namespace upnp
} // end namespace controlpoint


#endif // CONTROLPOINTBCU_H
