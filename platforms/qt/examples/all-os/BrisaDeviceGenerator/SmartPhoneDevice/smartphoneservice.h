#ifndef SMARTPHONESERVICE_H_
#define SMARTPHONESERVICE_H_

#include <BrisaUpnp/BrisaService>

using namespace Brisa;

class SmartPhoneService: public BrisaService {
public:
    SmartPhoneService();
    virtual ~SmartPhoneService();
};

#endif /* SMARTPHONESERVICE_H_ */
