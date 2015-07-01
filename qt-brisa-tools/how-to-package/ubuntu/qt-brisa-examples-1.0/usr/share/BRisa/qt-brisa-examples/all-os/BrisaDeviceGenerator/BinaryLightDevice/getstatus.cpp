#include "getstatus.h"

GetStatus::GetStatus(BrisaService * service) :
    BrisaAction("GetStatus", service) {
}

GetStatus::~GetStatus() {
}

QMap<QString, QString> GetStatus::run(const QMap<QString, QString> &inArguments) {
    Q_UNUSED(inArguments);

    QMap<QString, QString> outArguments;
    outArguments.insert("Status",
            this->getStateVariable("Status")->getAttribute(
                    BrisaStateVariable::Value));
    return outArguments;
}
