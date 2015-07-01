#ifndef PCSYSTEMTIMESERVICE_H_
#define PCSYSTEMTIMESERVICE_H_

#include <BrisaUpnp/BrisaService>

using namespace Brisa;

class PcSystemTimeService: public BrisaService {
public:
    PcSystemTimeService();
    virtual ~PcSystemTimeService();
};

#endif /* PCSYSTEMTIMESERVICE_H_ */
