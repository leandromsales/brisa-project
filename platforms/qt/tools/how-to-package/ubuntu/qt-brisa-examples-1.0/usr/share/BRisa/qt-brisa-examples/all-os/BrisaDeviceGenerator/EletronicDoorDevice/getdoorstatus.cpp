#include "getdoorstatus.h"

GetDoorStatus::GetDoorStatus(BrisaService * service) :
    BrisaAction("GetStatus", service) {
}

GetDoorStatus::~GetDoorStatus() {
}

QMap<QString, QString> GetDoorStatus::run(
        const QMap<QString, QString> &inArguments) {
    Q_UNUSED(inArguments);
    QMap<QString, QString> outArguments;
    outArguments.insert("Status",
            this->getStateVariable("Opened")->getAttribute(
                    BrisaStateVariable::Value));
    return outArguments;
}

