#ifndef GETBATTERYCHARGE_H_
#define GETBATTERYCHARGE_H_

#include <BrisaUpnp/BrisaAction>
#include <BrisaUpnp/BrisaService>

using namespace Brisa;

class GetBatteryCharge: public BrisaAction {
public:
    GetBatteryCharge(BrisaService * service);
    virtual ~GetBatteryCharge();

private:
    QMap<QString, QString> run(const QMap<QString, QString> &inArguments);
};

#endif /* GETBATTERYCHARGE_H_ */
