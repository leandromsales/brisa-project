#ifndef SWITCHLIGHTSERVICE_H_
#define SWITCHLIGHTSERVICE_H_

#include <BrisaUpnp/BrisaService>

using namespace Brisa;

class SwitchLightService: public BrisaService {
public:
    SwitchLightService();
    virtual ~SwitchLightService();
};

#endif /* SWITCHLIGHTSERVICE_H_ */
