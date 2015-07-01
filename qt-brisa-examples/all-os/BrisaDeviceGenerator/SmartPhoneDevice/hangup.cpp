#include "hangup.h"

HangUp::HangUp(BrisaService * service) :
    BrisaAction("HangUp", service) {
}

HangUp::~HangUp() {
}

QMap<QString, QString> HangUp::run(const QMap<QString, QString> &inArguments) {
    this->getStateVariable("Calling")->setAttribute(BrisaStateVariable::Value,
            0);
    QMap<QString, QString> outArguments;
    return outArguments;
}
