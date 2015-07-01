#ifndef SWITCHDOORSERVICE_H_
#define SWITCHDOORSERVICE_H_

#include <BrisaUpnp/BrisaAction>
#include <BrisaUpnp/BrisaService>
#include <QtDebug>

using namespace Brisa;

class SwitchDoorService: public BrisaService {
public:
    SwitchDoorService();
    virtual ~SwitchDoorService();
};

#endif /* SWITCHDOORSERVICE_H_ */
