#include "getcallstatus.h"

GetCallStatus::GetCallStatus(BrisaService * service) :
    BrisaAction("GetCallStatus", service) {
}

GetCallStatus::~GetCallStatus() {
}

QMap<QString, QString> GetCallStatus::run(
        const QMap<QString, QString> &inArguments) {
    Q_UNUSED(inArguments);

    QMap<QString, QString> outArguments;
    outArguments.insert("Calling",
            this->getStateVariable("Calling")->getAttribute(
                    BrisaStateVariable::Value));
    return outArguments;
}
