#include "turnoff.h"

TurnOff::TurnOff(BrisaService * service) :
    BrisaAction("TurnOff", service) {
}

TurnOff::~TurnOff() {
}

QMap<QString, QString> TurnOff::run(const QMap<QString, QString> &inArguments) {
    Q_UNUSED(inArguments);
    bool change = this->getStateVariable("Status")->getValue().toBool() == true;
    this->getStateVariable("Status")->setAttribute(BrisaStateVariable::Value, 0);

    QMap<QString, QString> outArguments;
    outArguments.insert("Changed", change ? "1" : "0");
    return outArguments;
}
