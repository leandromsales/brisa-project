#include "on.h"

On::On(BrisaService * service) :
    BrisaAction("On", service) {
}

On::~On() {
}

QMap<QString, QString> On::run(const QMap<QString, QString> &inArguments) {
    Q_UNUSED(inArguments);
    this->getStateVariable("Status")->setAttribute(BrisaStateVariable::Value, 1);
    QMap<QString, QString> outArguments;
    return outArguments;
}
