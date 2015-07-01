#include "call.h"

Call::Call(BrisaService * service) :
    BrisaAction("Call", service) {
}

Call::~Call() {
}

QMap<QString, QString> Call::run(const QMap<QString, QString> &inArguments) {
    QString number = inArguments["Number"];

    bool sucess = !this->isCalling() && validNumber(number);
    if (sucess) {
        this->getStateVariable("NumberCalling")->setAttribute(
                BrisaStateVariable::Value, inArguments["Number"]);
        this->getStateVariable("Calling")->setAttribute(
                BrisaStateVariable::Value, 1);
    }

    QMap<QString, QString> outArguments;
    outArguments.insert("Sucess", sucess ? "1" : "0");
    return outArguments;
}

bool Call::validNumber(QString number) {
    int digitsNumber = 0;
    for (int i = 0; i < number.size(); i++) {
        if (number[i].isDigit())
            digitsNumber++;
    }
    return digitsNumber == number.size() && digitsNumber >= 5 && digitsNumber
            <= 13;
}

bool Call::isCalling() {
    return this->getStateVariable("Calling")->getValue().toBool();
}
