#include "close.h"

Close::Close(BrisaService * service) :
    BrisaAction("Close", service) {
}

Close::~Close() {
}

QMap<QString, QString> Close::run(const QMap<QString, QString> &inArguments) {
    Q_UNUSED(inArguments);
    bool change = this->getStateVariable("Opened")->getValue().toBool() == true;
    this->getStateVariable("Opened")->setAttribute(BrisaStateVariable::Value, 0);

    QMap<QString, QString> outArguments;
    outArguments.insert("Changed", change ? "1" : "0");
    return outArguments;
}
