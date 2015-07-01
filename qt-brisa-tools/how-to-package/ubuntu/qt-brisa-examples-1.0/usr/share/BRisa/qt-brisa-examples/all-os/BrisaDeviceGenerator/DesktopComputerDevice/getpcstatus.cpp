#include "getpcstatus.h"

GetPcStatus::GetPcStatus(BrisaService * service) :
    BrisaAction("GetStatus", service) {
}

GetPcStatus::~GetPcStatus() {
}

QMap<QString, QString> GetPcStatus::run(
        const QMap<QString, QString> &inArguments) {
    Q_UNUSED(inArguments);

    QMap<QString, QString> outArguments;
    outArguments.insert("Status",
            this->getStateVariable("Status")->getAttribute(
                    BrisaStateVariable::Value));
    return outArguments;
}
