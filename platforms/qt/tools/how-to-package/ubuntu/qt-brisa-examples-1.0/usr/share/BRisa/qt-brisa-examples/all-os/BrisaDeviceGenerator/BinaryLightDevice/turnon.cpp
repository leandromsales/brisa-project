#include "turnon.h"

TurnOn::TurnOn(BrisaService * service) :
    BrisaAction("TurnOn", service) {
}

TurnOn::~TurnOn() {
}

QMap<QString, QString> TurnOn::run(const QMap<QString, QString> &inArguments) {
    Q_UNUSED(inArguments);
    bool change = this->getStateVariable("Status")->getValue().toBool()
            == false;
    this->getStateVariable("Status")->setAttribute(BrisaStateVariable::Value, 1);

    QMap<QString, QString> outArguments;
    outArguments.insert("Changed", change ? "1" : "0");
    return outArguments;
}
