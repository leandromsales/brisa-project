#ifndef DESKTOPCOMPUTERSERVICE_H_
#define DESKTOPCOMPUTERSERVICE_H_

#include <BrisaUpnp/BrisaService>

using namespace Brisa;

class PowerPcService: public BrisaService {
public:
    PowerPcService();
    virtual ~PowerPcService();
};

#endif /* DESKTOPCOMPUTERSERVICE_H_ */
