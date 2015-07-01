#include "getbatterycharge.h"

GetBatteryCharge::GetBatteryCharge(BrisaService * service) :
    BrisaAction("GetBatteryCharge", service) {
}

GetBatteryCharge::~GetBatteryCharge() {
}

QMap<QString, QString> GetBatteryCharge::run(
        const QMap<QString, QString> &inArguments) {
    Q_UNUSED(inArguments);

    QMap<QString, QString> outArguments;
    outArguments.insert("Charge",
            this->getStateVariable("BatteryCharge")->getAttribute(
                    BrisaStateVariable::Value));
    return outArguments;
}
